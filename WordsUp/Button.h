#pragma once
#include <SFML/Graphics.hpp>
#include "Label.h"
#include "RoundedRectangle.h"

/**
* \brief Button
*/
class Button : public sf::Drawable
{
public:
	enum AutoSize { YES, NO };
	void setSize(int width, int height);
	void setPosition(const sf::Vector2i& position);
	void setString(const std::wstring& str, AutoSize autoSize = YES);
	void setTextSize(int size, AutoSize autoSize = YES);
	void setTextColor(sf::Color color);
	void setColor(sf::Color color);
	sf::Vector2i getSize() const;
	sf::Vector2i getPosition() const;
	std::wstring getString() const;
	bool contains(float x, float y) const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	Label _label;
	RoundedRectangle _rect;
};