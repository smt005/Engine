#pragma once

#include <memory>
#include <map>
#include <vector>
#include <string>

namespace Engine {
	class Core;

	template<typename T>
	std::string GetClassName() {
		std::string className = typeid(T).name();
		return className.substr(6, (className.length() - 6)); // 'slass ' length == 6
	}
}

class UI {
	friend class Engine::Core;

public:
	class Window {
		friend class UI;

	public:
		typedef std::shared_ptr<Window> Ptr;
		typedef std::weak_ptr<Window> Wptr;

	public:
		template <typename T>
		Window(T* tPtr)
			: _closeBtn(true)
			, _visible(false)
			, _window_flags(NULL)
			, _alpha(1.f)
			, _id(Engine::GetClassName<T>())
		{}

		virtual ~Window() = default;

		virtual void Draw() {}
		virtual void OnOpen() {}
		virtual void OnClose() {}
		virtual void Update() {}

		inline void SetId(const std::string& id) { _id = id; }
		inline void SetFlag(int window_flags) { _window_flags = window_flags; }
		inline void SetAlpha(float alpha) { _alpha = alpha; }

		inline const std::string& Id() { return _id; }
		inline void VisibleCloseBtn(const bool value) { _closeBtn = value; }
		void Close() { UI::closedWindows.emplace_back(this); }

	private:
		bool _closeBtn;
		bool _visible;
		float _alpha;
		int _window_flags;
		std::string _id;
	};

	typedef std::map<std::string, UI::Window::Ptr> Windows;

private:
	static void Init(void* window);
	static void Cleanup();
	static void Render();

public:	
	template <typename T, typename... Args>
	static const std::string ShowWindow(Args&&...args) {
		UI::Window* window = new T(std::forward<Args>(args)...);
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

	template <typename T>
	static bool ShowingWindow() { return windows.find(Engine::GetClassName<T>()) != windows.end(); }

	template <typename T>
	static void CloseWindowT() { CloseWindow(Engine::GetClassName<T>()); };

private:
	static void Update() { for (const std::pair<std::string, UI::Window::Ptr>& windowPair : windows) { windowPair.second->Update(); }; }

private:
	static Windows windows;
	static std::vector<const Window*> closedWindows;
};
