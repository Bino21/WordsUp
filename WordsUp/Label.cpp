#include "Label.h"

sf::Font Label::_font;

Label::Label()
{
	if (_font.getInfo().family.empty())
	{
		if (!_font.loadFromFile(_path))
			std::cout << "class Label: font load error" << std::endl;
	}
	_label.setFont(_font);
}

void Label::setString(const std::wstring& str)
{
	_label.setString(str);
}

void Label::setTextSize(int size)
{
	_label.setCharacterSize(size); 
}

void Label::setTextColor(sf::Color color)
{
	_label.setFillColor(color); 
}

void Label::setPosition(int x, int y)
{
	_label.setPosition(x, y);
}

std::wstring Label::getString() const
{
	return _label.getString(); 
}

sf::FloatRect Label::getGlobalBounds() const
{
	return _label.getGlobalBounds(); 
}

sf::Vector2f Label::getPosition() const
{
	return _label.getPosition(); 
}

int Label::getTextSize() const
{
	return _label.getCharacterSize();
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_label); 
}

sf::Color Label::getColor() const
{
	return _label.getFillColor();
}