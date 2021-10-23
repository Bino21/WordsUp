#include "Button.h"

namespace
{
	int borders = 20;
}

void Button::setTextColor(sf::Color color)
{
	_label.setTextColor(color);
}

void Button::setColor(sf::Color color)
{
	_rect.setColor(color);
}

sf::Vector2i Button::getSize() const
{
	return _rect.getSize();
}

sf::Vector2i Button::getPosition() const
{
	return _rect.getPosition();
}

std::wstring Button::getString() const
{
	return _label.getString(); 
}

bool Button::contains(float x, float y) const
{
	return _rect.contains(x, y);
}

void Button::setSize(int width, int height)
{
	_rect.setSize(width, height);
	int textX = _rect.getPosition().x + ((width - _label.getGlobalBounds().width) / 2);
	int textY = _rect.getPosition().y + ((height - _label.getGlobalBounds().height) / 5);
	_label.setPosition(float(textX), float(textY));
}

void Button::setPosition(const sf::Vector2i& position)
{
	_rect.setPosition(position);
	int textX = _rect.getPosition().x + ((_rect.getSize().x - _label.getGlobalBounds().width) / 2);
	int textY = _rect.getPosition().y + ((_rect.getSize().y - _label.getGlobalBounds().height) / 5);
	_label.setPosition(float(textX), float(textY));
}

void Button::setString(const std::wstring& str, AutoSize autoSize)
{
	_label.setString(str);
	int textX = _rect.getPosition().x + ((_rect.getSize().x - _label.getGlobalBounds().width) / 2);
	int textY = _rect.getPosition().y + ((_rect.getSize().y - _label.getGlobalBounds().height) / 5);
	_label.setPosition(float(textX), float(textY));
	if (autoSize == Button::AutoSize::YES)
		_rect.setSize(_label.getGlobalBounds().width + (borders * 2), _rect.getSize().y);
}

void Button::setTextSize(int size, AutoSize autoSize)
{
	_label.setTextSize(size);
	int textX = _rect.getPosition().x + ((_rect.getSize().x - _label.getGlobalBounds().width) / 2);
	int textY = _rect.getPosition().y + ((_rect.getSize().y - _label.getGlobalBounds().height) / 5);
	_label.setPosition(float(textX), float(textY));
	if (autoSize == Button::AutoSize::YES)
		_rect.setSize(_label.getGlobalBounds().width + (borders * 2), _label.getGlobalBounds().height + (borders * 2));
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_rect);
	target.draw(_label);
}
