#pragma once
#include <SFML/Graphics.hpp>
#include "Label.h"

/**
* \brief TextEdit
*/
class TextEdit : public sf::Drawable
{
public:
	TextEdit();
	void setLabel(const std::wstring& str);
	void setFocus(bool focus);
	void setLabelSize(int size);
	void setPosition(const sf::Vector2i& position);
	void setSize(int width, int height);
	void setLabelColor(const sf::Color& color);
	void setTextColor(const sf::Color& color);
	void setTextFiedColor(const sf::Color& color);
	void onClick(float x, float y);
	void onWrite(const sf::Event& event);
	bool onFocus() const;
	void setString(const std::wstring& str);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	std::wstring getText();
	sf::Vector2i getTextPosition() const;

private:
	Label _label;
	sf::Vector2i _position;
	int _height = 30;
	sf::RectangleShape _textField;
	Label _text;
	sf::String _str;
	bool _focus = false;
};