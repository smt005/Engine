
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
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();

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
				windows.erase(itErase);
			}
		}

		closedWindows.clear();
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	for (auto& pair : windows) {
		if (!pair.second) {
			continue;
		}

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
		ImGui::Begin(pair.second->_title.c_str(), &pair.second->_open, window_flags);
		pair.second->Draw();
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
