
#pragma once

namespace Engine {
class Text;
}

class DrawText
{
public:
	static void ClearColor();
	static void Viewport();
	static void Prepare();
	static void Draw(Engine::Text& text);

private:
	static unsigned int curentBufer;
	static unsigned int currentTexture;
};
