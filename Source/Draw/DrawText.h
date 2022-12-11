
#pragma once

namespace Engine {
class Text;
}

class DrawText
{
public:
	static void Prepare();
	static void draw(Engine::Text& text);
};
