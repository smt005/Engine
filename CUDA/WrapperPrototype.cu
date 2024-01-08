
#include  "WrapperPrototype.h"
#include "Wrapper.h"
#include <thread>
#include <vector>

#if ENABLE_CUDA

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdio.h>

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
}

void CUDA_Prototype::GetForcesCPUStatic(int count, float* masses, float* positionsX, float* positionsY, float* forcesX, float* forcesY) {
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

void CUDA_Prototype::GetForcesGPUStatic(int count, float* masses, float* positionsX, float* positionsY, float* forcesX, float* forcesY) {
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
