#pragma once

void testCUDA(void);

class CUDA final {
public:
	static void GetProperty();
	static void PrintInfo();

public:
	static int deviceCount;
	static int warpSize;
};
