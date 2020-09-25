
#pragma once

#include <vector>

#include "UISample.h"

namespace UI {
	class Screen {
	public:
		static void init();
		static void prepare();
		static void draw();
		static void draw(Sample& sample);

	public:
		//static std::vector<Sample> _samples;
	};
}