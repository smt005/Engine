
#include "Vrapper.h"

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

#else
    void testCUDA(void) {}

    void CUDA::GetProperty() {}
    void CUDA::PrintInfo() {}
#endif
