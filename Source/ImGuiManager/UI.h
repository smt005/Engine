#pragma once

#include <memory>
#include <vector>
#include <string>
#include <functional>

namespace UI {
	class Window {
	public:
		typedef std::shared_ptr<Window> Ptr;

	public:
		inline const std::string& Id() { return _id; };
		inline std::function<bool()> Content() { return _content; };

		void SetId(const std::string& id) { _id = id; }
		void SetContent(std::function<bool()> content) { _content = content; }

	private:
		std::string _id;
		std::function<bool()> _content;
	};

	typedef std::vector<UI::Window::Ptr> Windows;

	void Init(void* window);
	void Cleanup();
	void Render();
	void AddWindow(Window::Ptr& window);
};
