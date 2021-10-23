#pragma once
#include <SFML/Graphics.hpp>

/**
* \brief RoundedRectangle shape
*/
class RoundedRectangle : public sf::Drawable
{
public:
	RoundedRectangle();
	void setRadius(int r);
	void setColor(const sf::Color& color);
	virtual void setSize(int width, int height);
	virtual void setPosition(sf::Vector2i pos);
	sf::Vector2i getPosition() const;
	sf::Vector2i getSize() const;
	sf::Color getColor() const;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool contains(int x, int y) const;

protected:
	void setParams();

protected:
	int _radius;
	sf::Color _color;
	sf::Vector2i _position;
	int _width;
	int _height;
	sf::RectangleShape _rectWidth;
	sf::RectangleShape _rectHeight;
	sf::CircleShape _cTopLeft;
	sf::CircleShape _cTopRight;
	sf::CircleShape _cDownLeft;
	sf::CircleShape _cDownRight;
};