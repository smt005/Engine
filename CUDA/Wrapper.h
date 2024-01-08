#pragma once

void testCUDA(void);

class CUDA final {
public:
	static void GetProperty();
	static void PrintInfo();

	static void GetForcesStaticTest(int count, float* masses, float* positionsX, float* positionsY, float* forcesX, float* forcesY);

public:
	static int deviceCount;
	static int warpSize;
};
