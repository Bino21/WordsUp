#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

/**
* \brief Text label
*/
class Label : public sf::Drawable
{
public:
	Label();
	virtual void setString(const std::wstring& str);
	virtual void setTextSize(int size);
	virtual void setTextColor(sf::Color color);
	void setPosition(int x, int y);
	virtual std::wstring getString() const;
	sf::FloatRect getGlobalBounds() const;
	sf::Vector2f getPosition() const;
	int getTextSize() const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::Color getColor() const;

protected:
	static sf::Font _font;
	const std::string _path = "fonts/SF-Pro-Display-Regular.otf";
	sf::Text _label;
};