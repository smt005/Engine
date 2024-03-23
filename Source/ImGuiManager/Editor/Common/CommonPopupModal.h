// ◦ Xyz ◦
#pragma once

#include <functional>
#include <memory>
#include <string>

namespace Editor {
	class CommonPopupModal {
	public:
		class Wptr {
		public:
			inline std::shared_ptr<bool> GetPtr() { if (!_sharedWndPtr) { _sharedWndPtr = std::make_shared<bool>(true); } return _sharedWndPtr; }
			inline void Reset() { _sharedWndPtr.reset(); }

		private:
			std::shared_ptr<bool> _sharedWndPtr;
		};

	public:
		static void Show(const std::weak_ptr<bool>& weakWndPtr, const std::function<void(void)>& content, const std::string& titlePopupModal = "Window");
		static void Hide();
		static void Draw();

	private:
		static std::weak_ptr<bool>			_weakWndPtr;
		static std::function<void(void)>	_content;
		static std::string					_titlePopupModal;
		static bool							_statePopupModal;
	};
}
