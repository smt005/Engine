
#pragma once

#include "UIData.h"
#include "Object/Triangle.h"
#include "Object/Texture.h"
#include "Callback/Callback.h"

namespace UI {
	class Element;
	typedef std::shared_ptr<Element> ElementPtr;

	class Element : public Engine::Callback {
	public:
		Element();
		~Element();

		bool hit();
		bool hit(const glm::vec2& mousePos);

		void set(const int count, Triangle* triangle, TexturePtr texturePtr);
		bool load(const std::string& name);

	//private:
	public:
		int _count;
		Triangle* _triangles;
		std::vector<ElementPtr> _childs;

		UIDataPtr dataPtr;

	public:
		static ElementPtr make(const UIDataPtr& dataPtr, const std::string& name = std::string());
		static ElementPtr make(const glm::vec2& size, const std::string& texteruName, const std::string& name = std::string());
	};
}
