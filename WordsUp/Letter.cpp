#include "Letter.h"

namespace
{
	const float textSize = 0.8f;
	const sf::Color rectangleColor = sf::Color(68, 102, 148);
	const sf::Color textColor = sf::Color(255, 242, 0);
}

Letter::Letter(wchar_t ch)
{
	_value = ch;
	_rectangle.setSize(sf::Vector2f(float(size), float(size)));
	_rectangle.setFillColor(rectangleColor);
	_text.setTextSize(unsigned int(size * textSize));
	_text.setString(sf::String(ch));
	_text.setTextColor(textColor);
	updatePositionElements();
}

void Letter::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_rectangle);
	target.draw(_text);
}

void Letter::setPosition(int x, int y)
{
	_position.x = x;
	_position.y = y;
	updatePositionElements();
}

void Letter::setPosition(const sf::Vector2i pos)
{
	_position = pos;
	updatePositionElements();
}

void Letter::updatePositionElements()
{
	_rectangle.setPosition(_position.x, _position.y);
	float textPositionX = _position.x + ((size - _text.getGlobalBounds().width) / 2);
	float textPositionY = _position.y - ((size - _text.getGlobalBounds().height) / 5);
	_text.setPosition(textPositionX, textPositionY);
}

sf::FloatRect Letter::getGlobalBounds() const
{
	return _rectangle.getGlobalBounds();
}

wchar_t Letter::getLetter() const
{
	return _value;
}

void Letter::setTextColor(sf::Color color)
{
	_text.setTextColor(color);
}

sf::Vector2i Letter::getPosition() const
{
	return _position;
}