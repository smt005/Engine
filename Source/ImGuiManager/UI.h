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
		Window() : _open(true) {}
		virtual ~Window() = default;

		virtual void Draw() {}
		virtual void OnOpen() {}
		virtual void OnClose() {}

		inline void SetId(const std::string& id) { _id = id; }
		inline void SetTitle(const std::string& title) { _title = title; }

		void Close() { UI::closedWindows.emplace_back(this); }

	private:
		bool _open;
		std::string _id;
		std::string _title;
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
		window->OnOpen();
		return id;
	}
	static void CloseWindow(const std::string& id);
	static void CloseWindow(const Window* windowPtr) { closedWindows.emplace_back(windowPtr); }
	static void CloseWindow(const Window::Ptr windowPtr) { closedWindows.emplace_back(windowPtr.get()); }

private:
	static Windows windows;
	static std::vector<const Window*> closedWindows;
};
