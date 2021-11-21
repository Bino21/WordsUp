#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Label.h"

/**
* \brief MultiLineText
*/
class MultiLineText : public sf::Drawable
{
public:
	MultiLineText();
	void setTextColor(const sf::Color& color);
	void setTextSize(int size);
	void setWidth(int width);
	void setString(const std::wstring& str);
	void setPosition(int x, int y);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	void setParams();

private:
	std::vector<std::wstring> _string;
	std::vector<std::unique_ptr<Label> > _labels;
	int _width;
	int _size;
	sf::Color _color;
	sf::Vector2i _position;
};
