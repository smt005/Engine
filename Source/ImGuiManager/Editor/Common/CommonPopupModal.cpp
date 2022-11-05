
#include <ImGuiManager/Editor/Common/CommonPopupModal.h>
#include "imgui.h"

namespace Editor {

	std::weak_ptr<bool> CommonPopupModal::_weakWndPtr;
	std::function<void(void)> CommonPopupModal::_content = nullptr;
	std::string CommonPopupModal::_titlePopupModal;
	bool CommonPopupModal::_statePopupModal = false;

	void CommonPopupModal::Show(const std::weak_ptr<bool>& weakWndPtr, const std::function<void(void)>& content, const std::string& titlePopupModal) {
		_weakWndPtr = weakWndPtr;
		_content = content;
		_titlePopupModal = titlePopupModal;
		_statePopupModal = true;
	}

	void CommonPopupModal::Hide() {
		ImGui::CloseCurrentPopup();

		_content = nullptr;
		_weakWndPtr.reset();
		_titlePopupModal.clear();
	}

	void CommonPopupModal::Draw() {
		if (!_content) {
			return;
		}

		if (_weakWndPtr.expired()) {
			Hide();
			return;
		}

		if (_statePopupModal) {
			ImGui::OpenPopup(_titlePopupModal.c_str());
			_statePopupModal = false;
		}

		if (ImGui::BeginPopupModal(_titlePopupModal.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			_content();
			ImGui::EndPopup();
		}
	}
}
