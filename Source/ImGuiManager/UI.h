#pragma once

#include <memory>
#include <map>
#include <vector>
#include <string>

namespace Engine {
	class Core;
}

class UI {
	friend class Engine::Core;

public:
	class Window {
		friend class UI;

	public:
		typedef std::shared_ptr<Window> Ptr;

	public:
		Window() : _closeBtn(true), _visible(false), _window_flags(NULL) {}
		virtual ~Window() = default;

		virtual void Draw() {}
		virtual void OnOpen() {}
		virtual void OnClose() {}
		virtual void Update() {}

		inline void SetId(const std::string& id) { _id = id; }
		inline void SetFlag(int window_flags) { _window_flags = window_flags; }
		inline const std::string& Id() { return _id; }
		inline void VisibleCloseBtn(const bool value) { _closeBtn = value; }
		void Close() { UI::closedWindows.emplace_back(this); }

	private:
		bool _closeBtn;
		bool _visible;
		int _window_flags;
		std::string _id;
	};

	typedef std::map<std::string, UI::Window::Ptr> Windows;

private:
	static void Init(void* window);
	static void Cleanup();
	static void Render();

public:	
	template <typename T>
	static const std::string ShowWindow() {
		T* window = new T();
		std::string id = window->_id;
		if (windows.find(id) != windows.end()) {
			delete window;
			return id;
		}

		windows.emplace(window->_id, T::Ptr(window));
		return id;
	}

	static bool WindowDisplayed() { return !windows.empty(); }
	static bool ShowingWindow(const std::string& id) { return windows.find(id) != windows.end(); }
	static void CloseWindow(const std::string& id);
	static void CloseWindow(const Window* windowPtr) { closedWindows.emplace_back(windowPtr); }
	static void CloseWindow(const Window::Ptr windowPtr) { closedWindows.emplace_back(windowPtr.get()); }
	static Window::Ptr GetWindow(const std::string& id);

private:
	static void Update() { for (const std::pair<std::string, UI::Window::Ptr>& windowPair : windows) { windowPair.second->Update(); }; }

private:
	static Windows windows;
	static std::vector<const Window*> closedWindows;
};
