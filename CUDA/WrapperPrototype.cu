
#include  "WrapperPrototype.h"
#include "Wrapper.h"
#include <thread>
#include <vector>
#include <iostream>

#if ENABLE_CUDA

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdio.h>

namespace {
    void GetForceCPU(int *count, int *offset, float* masses, float* positionsX, float* positionsY, float* forcesX, float* forcesY, int threadId) {
        double _constGravity = 0.01f;
        int statIndex = threadId * *offset;
        int endIndex = statIndex +* offset;
        if (endIndex > *count) {
            endIndex = *count;
        }

        int sizeData = *count;
        float gravityVecX;
        float gravityVecY;
        double dist;
        double force;

        for (int index = statIndex; index < endIndex; ++index) {
            float& fX = forcesX[index];
            float& fY = forcesY[index];

            for (size_t otherIndex = 0; otherIndex < sizeData; ++otherIndex) {
                if (index == otherIndex) {
                    continue;
                }

                gravityVecX = positionsX[otherIndex] - positionsX[index];
                gravityVecY = positionsY[otherIndex] - positionsY[index];

                dist = sqrt(gravityVecX * gravityVecX + gravityVecY * gravityVecY);
                gravityVecX /= dist;
                gravityVecY /= dist;

                force = _constGravity * (masses[index] * masses[otherIndex]) / (dist * dist);
                gravityVecX *= force;
                gravityVecY *= force;

                fX += gravityVecX;
                fY += gravityVecY;
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
                    GetForceCPU(&count, &offst, masses, positionsX, positionsY, forcesX, forcesY, threadId);
                    });
            }

            for (std::thread& th : threads) {
                th.join();
            }
        }
        else
        {
            GetForceCPU(&count, &count, masses, positionsX, positionsY, forcesX, forcesY, 0);
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // GPU ///////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    
namespace {
    __global__
    void GetForceGPU(int* count, int* offset, float* masses, float* positionsX, float* positionsY, float* forcesX, float* forcesY) {
        double _constGravity = 0.01f;
        int statIndex = (threadIdx.x + blockIdx.x * blockDim.x) * *offset;
        int endIndex = statIndex + *offset;
        if (endIndex > *count) {
            endIndex = *count;
        }

        int sizeData = *count;
        float gravityVecX = 0;
        float gravityVecY = 0;
        double dist;
        double force;

        for (int index = statIndex; index < endIndex; ++index) {
            float& fX = forcesX[index];
            float& fY = forcesY[index];

            for (size_t otherIndex = 0; otherIndex < sizeData; ++otherIndex) {
                if (index == otherIndex) {
                    continue;
                }

                gravityVecX = positionsX[otherIndex] - positionsX[index];
                gravityVecY = positionsY[otherIndex] - positionsY[index];

                dist = sqrt(gravityVecX * gravityVecX + gravityVecY * gravityVecY);
                gravityVecX /= dist;
                gravityVecY /= dist;

                force = _constGravity * (masses[index] * masses[otherIndex]) / (dist * dist);
                gravityVecX *= force;
                gravityVecY *= force;

                fX += gravityVecX;
                fY += gravityVecY;
            }
        }
    }
}

void CUDA_Prototype::GetForcesGPUStatic(int count, float* masses, float* positionsX, float* positionsY, float* forcesX, float* forcesY) {
    unsigned int counThread = count < CUDA::maxThreadsPerBlock ? count : CUDA::maxThreadsPerBlock;

    unsigned int countBlock = (count + counThread - 1) / counThread;
    countBlock = countBlock > CUDA::maxGridSize[1] ? CUDA::maxGridSize[1] : countBlock;

    int offset = count / (counThread * countBlock);
    if ((count % (counThread * countBlock)) > 0) {
        ++offset;
    }

    //...
    int* devCount;
    int* devOffset;
    float* devMasses;
    float* devPositionsX;
    float* devPositionsY;
    float* devForcesX;
    float* devForcesY;

    cudaMalloc(&devCount,               sizeof(int));
    cudaMalloc(&devOffset,              sizeof(int));
    cudaMalloc(&devMasses,      count * sizeof(float));
    cudaMalloc(&devPositionsX,  count * sizeof(float));
    cudaMalloc(&devPositionsY,  count * sizeof(float));
    cudaMalloc(&devForcesX,     count * sizeof(float));
    cudaMalloc(&devForcesY,     count * sizeof(float));

    cudaMemcpy(devCount,        &count,             sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(devOffset,       &offset,             sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(devMasses,       masses,     count * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(devPositionsX,   positionsX, count * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(devPositionsY,   positionsY, count * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(devForcesX,      forcesX,    count * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(devForcesY,      forcesY,    count * sizeof(float), cudaMemcpyHostToDevice);

    GetForceGPU <<<countBlock, counThread>>> (devCount, devOffset, devMasses, devPositionsX, devPositionsY, devForcesX, devForcesY);

    cudaMemcpy(forcesX, devForcesX, count * sizeof(float), cudaMemcpyDeviceToHost);
    cudaMemcpy(forcesY, devForcesY, count * sizeof(float), cudaMemcpyDeviceToHost);

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
