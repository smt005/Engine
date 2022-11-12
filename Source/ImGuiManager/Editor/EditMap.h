#pragma once

#include "ImGuiManager/UI.h"
#include "Common/CommonPopupModal.h"
#include "Common/Common.h"
#include "Object/Object.h"

namespace Editor {
	class MapEditor final : public UI::Window, public CommonPopupModal::Wptr {
	public:
		MapEditor();
		void Draw() override;
		void OnOpen() override;
		void OnClose() override;

	private:
		void ListDisplay();
		void EditDisplay();
		void ButtonDisplay();

		void Load();
		void Save();

	private:
		int _guiId;
		Object* _selectObjectPtr = nullptr;

		Editor::TextChar _name;
		Editor::TextChar _model;
	};
}