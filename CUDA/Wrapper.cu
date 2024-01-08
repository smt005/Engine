
#include "Wrapper.h"
#include <thread>
#include <vector>

int CUDA::deviceCount = -1;
int CUDA::warpSize = 0;

#if ENABLE_CUDA

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdio.h>

__global__
void saxpy(int n, float a, float* x, float* y)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) y[i] = a * x[i] + y[i];
}

void testCUDA(void)
{
    int N = 1 << 20;
    float* x, * y, * d_x, * d_y;
    x = (float*)malloc(N * sizeof(float));
    y = (float*)malloc(N * sizeof(float));

    cudaMalloc(&d_x, N * sizeof(float));
    cudaMalloc(&d_y, N * sizeof(float));

    for (int i = 0; i < N; i++) {
        x[i] = 1.0f;
        y[i] = 2.0f;
    }

    cudaMemcpy(d_x, x, N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_y, y, N * sizeof(float), cudaMemcpyHostToDevice);

    // Perform SAXPY on 1M elements
    saxpy << <(N + 255) / 256, 256 >> > (N, 2.0f, d_x, d_y);

    cudaMemcpy(y, d_y, N * sizeof(float), cudaMemcpyDeviceToHost);

    float maxError = 0.0f;
    for (int i = 0; i < N; i++)
        maxError = std::max(maxError, std::abs(y[i] - 4.0f));
    printf("Max error: %f\n", maxError);

    cudaFree(d_x);
    cudaFree(d_y);
    free(x);
    free(y);
}

void CUDA::GetProperty() {
    if (deviceCount == -1) {
        cudaGetDeviceCount(&deviceCount);

        if (deviceCount == 0) {
            return;
        }

        cudaDeviceProp deviceProp;
        cudaGetDeviceProperties(&deviceProp, 0);

        warpSize = deviceProp.warpSize;
    }
}

void CUDA::PrintInfo() {
    if (deviceCount == -1) {
        GetProperty();
    }

    printf("CUDA: deviceCount: %i\n", deviceCount);

    if (deviceCount == 0) {
        return;
    }

    cudaDeviceProp deviceProp;
    cudaGetDeviceProperties(&deviceProp, 0);

    printf("CUDA: warpSize: %i\n", deviceProp.warpSize);
}

namespace {
    void GetForceCPU(int count, int offset, float* masses, float* positionsX, float* positionsY, float* forcesX, float* forcesY, int threadId) {
        double _constGravity = 0.01f;
        int statIndex = threadId * offset;
        int endIndex = statIndex + offset;
        if (endIndex > count) {
            endIndex = count;
        }

        int sizeData = count;

        for (int index = statIndex; index < endIndex; ++index) {
            for (size_t otherIndex = 0; otherIndex < sizeData; ++otherIndex) {
                if (index == otherIndex) {
                    continue;
                }

                float gravityVecX = positionsX[otherIndex] - positionsX[index];
                float gravityVecY = positionsY[otherIndex] - positionsY[index];

                double dist = sqrt(gravityVecX * gravityVecX + gravityVecY * gravityVecY);
                gravityVecX /= dist;
                gravityVecY /= dist;

                double force = _constGravity * (masses[index] * masses[otherIndex]) / (dist * dist);
                gravityVecX *= force;
                gravityVecY *= force;

                forcesX[index] += gravityVecX;
                forcesY[index] += gravityVecY;
            }
        }
    }
}

__global__
void GetForceGPU(int* count, int* offset, float* masses, float* positionsX, float* positionsY, float* forcesX, float* forcesY) {
    double _constGravity = 0.01f;
    int statIndex = blockIdx.x * *offset;
    int endIndex = statIndex + *offset;
    if (endIndex > *count) {
        endIndex = *count;
    }

    int sizeData = *count;

    for (int index = statIndex; index < endIndex; ++index) {
        for (size_t otherIndex = 0; otherIndex < sizeData; ++otherIndex) {
            if (index == otherIndex) {
                continue;
            }

            float gravityVecX = positionsX[otherIndex] - positionsX[index];
            float gravityVecY = positionsY[otherIndex] - positionsY[index];

            double dist = sqrt(gravityVecX * gravityVecX + gravityVecY * gravityVecY);
            gravityVecX /= dist;
            gravityVecY /= dist;

            double force = _constGravity * (masses[index] * masses[otherIndex]) / (dist * dist);
            gravityVecX *= force;
            gravityVecY *= force;

            forcesX[index] += gravityVecX;
            forcesY[index] += gravityVecY;
        }
    }
}

void CUDA::GetForcesStaticTest(int count, float* masses, float* positionsX, float* positionsY, float* forcesX, float* forcesY) {
    unsigned int counThread = static_cast<double>(std::thread::hardware_concurrency());
    unsigned int sizeB = count;

    if ((sizeB * 2) > counThread) {
        int offst = sizeB / counThread;
        if ((sizeB % counThread) > 0) {
            ++offst;
        }

        std::vector<std::thread> threads;
        threads.reserve(counThread);

        for (unsigned int threadId = 0; threadId < counThread; ++threadId) {
            threads.emplace_back([&]() {
                GetForceCPU(count, offst, masses, positionsX, positionsY, forcesX, forcesY, threadId);
            });
        }

        for (std::thread& th : threads) {
            th.join();
        }
    }
    else
    {
        GetForceCPU(count, count, masses, positionsX, positionsY, forcesX, forcesY, 0);
    }
}

void CUDA::GetForcesStatic(int count, float* masses, float* positionsX, float* positionsY, float* forcesX, float* forcesY) {
    unsigned int counThread = CUDA::warpSize;
    if (counThread == 0) {
        return;
    }

    int offst = count / counThread;
    if ((count % counThread) > 0) {
        ++offst;
    }

    //...
    int* devCount;
    int* devOffset;
    float* devMasses;
    float* devPositionsX;
    float* devPositionsY;
    float* devForcesX;
    float* devForcesY;

    cudaMalloc(&devCount,               sizeof(float));
    cudaMalloc(&devOffset,              sizeof(float));
    cudaMalloc(&devMasses,      count * sizeof(float));
    cudaMalloc(&devPositionsX,  count * sizeof(float));
    cudaMalloc(&devPositionsY,  count * sizeof(float));
    cudaMalloc(&devForcesX,     count * sizeof(float));
    cudaMalloc(&devForcesY,     count * sizeof(float));

    cudaMemcpy(devCount,        &count,     count * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(devOffset,       &offst,     count * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(devMasses,       masses,     count * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(devPositionsX,   positionsX, count * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(devPositionsY,   positionsY, count * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(devForcesX,      forcesX,    count * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(devForcesY,      forcesY,    count * sizeof(float), cudaMemcpyHostToDevice);

    GetForceGPU <<<counThread, 1>>> (devCount, devOffset, devMasses, devPositionsX, devPositionsY, devForcesX, devForcesY);

    cudaMemcpy(devForcesX, forcesX, count * sizeof(float), cudaMemcpyDeviceToHost);
    cudaMemcpy(devForcesY, forcesY, count * sizeof(float), cudaMemcpyDeviceToHost);

    cudaFree(devCount);
    cudaFree(devOffset);
    cudaFree(devMasses);
    cudaFree(positionsX);
    cudaFree(positionsY);
    cudaFree(devForcesX);
    cudaFree(devForcesY);
}

//...
#else
    void testCUDA(void) {}

    void CUDA::GetProperty() {}
    void CUDA::PrintInfo() {}
#endif
