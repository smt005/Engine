
#include "UI.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <set>

UI::Windows UI::windows;
std::vector<const UI::Window*> UI::closedWindows;

// UI
void UI::Init(void* window) {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void UI::Cleanup() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UI::Render() {
	if (windows.empty()) {
		return;
	}
	
	if (!closedWindows.empty()) {
		for (const Window* windowPtr : closedWindows) {
			auto itErase = std::find_if(windows.begin(), windows.end(), [windowPtr](const auto& pair) { return pair.second.get() == windowPtr; });
			if (itErase != windows.end()) {
				itErase->second->OnClose();
				windows.erase(itErase);
			}
		}

		closedWindows.clear();
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	for (auto& pair : windows) {
		Window* window = pair.second.get();
		if (!window) {
			continue;
		}
		
		if (window->_alpha < 1.f) {
			ImGui::SetNextWindowBgAlpha(window->_alpha);
		}

		ImGui::Begin(window->_id.c_str(), window->_closeBtn ? &window->_closeBtn : nullptr, window->_window_flags);
			if (!window->_visible) {
				window->OnOpen();
				window->_visible = true;
			}
			if (!window->_closeBtn) {
				CloseWindow(window);
			}
			window->Draw();
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::CloseWindow(const std::string& id) {
	auto itErase = windows.find(id);
	if (itErase != windows.end()) {
		closedWindows.emplace_back(itErase->second.get());
	}
}

UI::Window::Ptr UI::GetWindow(const std::string& id) {
	auto it = windows.find(id);
	if (it != windows.end()) {
		return it->second;
	}
	return nullptr;
}

