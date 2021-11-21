#include "RoundedRectangle.h"

RoundedRectangle::RoundedRectangle()
{
	setRadius(10);
}

void RoundedRectangle::setParams()
{
	_cTopLeft.setPosition(_position.x, _position.y);
	_cTopLeft.setRadius(_radius);
	_cTopLeft.setFillColor(_color);
	_cTopRight.setPosition(_position.x + _width - (2 * _radius), _position.y);
	_cTopRight.setRadius(_radius);
	_cTopRight.setFillColor(_color);
	_cDownLeft.setPosition(_position.x, _position.y + _height - (2 * _radius));
	_cDownLeft.setRadius(_radius);
	_cDownLeft.setFillColor(_color);
	_cDownRight.setPosition(_position.x + _width - (2 * _radius), _position.y + _height - (2 * _radius));
	_cDownRight.setRadius(_radius);
	_cDownRight.setFillColor(_color);
	_rectWidth.setSize(sf::Vector2f(_width, _height - (2 * _radius)));
	_rectWidth.setPosition(sf::Vector2f(_position.x, _position.y + _radius));
	_rectWidth.setFillColor(_color);
	_rectHeight.setSize(sf::Vector2f(_width - (2 * _radius), _height));
	_rectHeight.setPosition(sf::Vector2f(_position.x + _radius, _position.y));
	_rectHeight.setFillColor(_color);
}

sf::Color RoundedRectangle::getColor() const
{
	return _color;
}

void RoundedRectangle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_cDownLeft);
	target.draw(_cDownRight);
	target.draw(_cTopLeft);
	target.draw(_cTopRight);
	target.draw(_rectHeight);
	target.draw(_rectWidth);
}

bool RoundedRectangle::contains(int x, int y) const
{
	if (_cDownLeft.getGlobalBounds().contains(x, y))
		return true;
	else if (_cDownRight.getGlobalBounds().contains(x, y))
		return true;
	else if (_cTopRight.getGlobalBounds().contains(x, y))
		return true;
	else if (_cTopLeft.getGlobalBounds().contains(x, y))
		return true;
	else if (_rectHeight.getGlobalBounds().contains(x, y))
		return true;
	else if (_rectWidth.getGlobalBounds().contains(x, y))
		return true;
	else
		return false;
}

void RoundedRectangle::setRadius(int r)
{
	_radius = r;
	setParams();
}

void RoundedRectangle::setColor(const sf::Color& color)
{
	_color = color;
	setParams();
}

void RoundedRectangle::setSize(int width, int height)
{
	_width = width;
	_height = height;
	setParams();
}

void RoundedRectangle::setPosition(sf::Vector2i pos)
{
	_position = pos;
	setParams();
}

sf::Vector2i RoundedRectangle::getPosition() const
{
	return _position; 
}

sf::Vector2i RoundedRectangle::getSize() const
{
	return sf::Vector2i(_width, _height);
}