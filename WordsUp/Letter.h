/*
 * This file is part of WordsUp program.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <SFML/Graphics.hpp>
#include "Label.h"

 /**
 * Describes the letter for further use in the class CollectWords
 *
 * The letter is a rectangle with a letter inscribed in it
 */
class Letter : public sf::Drawable
{
public:
	Letter(wchar_t ch);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	/// updates the rectangle and letter positions
	void updatePositionElements();

	void setPosition(int x, int y);
	void setPosition(const sf::Vector2i pos);
	sf::Vector2i getPosition() const;
	sf::FloatRect getGlobalBounds() const;
	wchar_t getLetter() const;
	void setTextColor(sf::Color color);

private:
	Label _text;
	sf::RectangleShape _rectangle;
	sf::Vector2i _position;
	sf::Vector2i _size;
	wchar_t _value;

public:
	static const int size = 50;
};