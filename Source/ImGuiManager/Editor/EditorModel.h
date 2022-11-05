#pragma once

#include "ImGuiManager/UI.h"
#include "Common/Help.h"
#include "Common/CommonPopupModal.h"
#include "Common/Common.h"
#include <vector>

namespace Editor {
	class ModelEditor final : public UI::Window, public CommonPopupModal::Wptr {

	public:
		ModelEditor();
		void Draw() override;
		void OnOpen() override;
		void OnClose() override;

		void ListDisplay();
		void EditDisplay();
		void ButtonDisplay();

		void Select(std::string& name);
		void Remove(std::string& name);
		void Add(std::string& name);
		void Apply();
		void Load();
		void Save();

	private:
		int _guiId;
		std::string* _selectNamePtr = nullptr;
		std::vector<std::string> _modelNames;

		Editor::TextChar _name;
		Editor::TextChar _shape;
		Editor::TextChar _texture;
		//Editor::TextChar _scale;
	};
}
