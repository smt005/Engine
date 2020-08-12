
#pragma once

#include <vector>
#include "UISample.h"
#include "Callback/Callback.h"

namespace UI {
	class Screen {
	public:
		static void init();
		static void prepare();
		static void draw();
		static void draw(Sample& sample);
		static bool hit(const float mouseX, const float mouseY);
		static bool hit(Sample& sample, const float mouseX, const float mouseY);

	private:
		static Engine::CallbackPtr _callbackPtr;
		static glm::mat4x4 _matProjectionView;
		static float _mouseX;
		static float _mouseY;
	};
}