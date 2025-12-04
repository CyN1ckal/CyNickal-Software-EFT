#pragma once

class ColorPicker
{
public:
	static void Render();

private:
	static void MyColorPicker(const char* label, ImColor& color);

public:
	static inline ImColor m_EnemyColor{ ImColor(200,0,0) };
	static inline ImColor m_ScavColor{ ImColor(200,200,0) };
};