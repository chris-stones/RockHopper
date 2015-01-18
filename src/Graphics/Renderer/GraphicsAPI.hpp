
#pragma once

namespace RH { namespace Graphics {

class GraphicsAPI {
public:
	GraphicsAPI();
	~GraphicsAPI();

	void Begin(UI::Window * window);
	void Render(Abstract::Object * object, const glm::mat4 & MVP, float alpha);
	void End();

	class Impl;

private:

	Impl * impl;
};

}}

