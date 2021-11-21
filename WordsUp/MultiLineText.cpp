#include "MultiLineText.h"

MultiLineText::MultiLineText()
{
	_size = 0;
	_position.x = 0;
	_position.y = 0;
	_width = 0;
}

void MultiLineText::setParams()
{
	_labels.clear();
	int x, y; // coordinates relative to the position of the object MultiLineText
	if (_size > 0 && _width > 0)
	{
		for (int i = 0; i < _string.size(); ++i)
		{
			std::unique_ptr<Label> pLabel(new Label);
			pLabel->setTextSize(_size);
			pLabel->setString(_string[i]);
			pLabel->setTextColor(_color);
			_labels.push_back(std::move(pLabel));
		}
		int j = 0;
		for (int i = 0; i < _labels.size(); ++i)
		{
			if (i == 0)
			{
				x = 0;
				y = 0;
			}
			else
			{
				x += _labels[i - 1]->getGlobalBounds().width;
				if (x + _labels[i]->getGlobalBounds().width > _width)
				{
					++j;
					x = 0;
				}
				y = _size * j;
			}
			_labels[i]->setPosition(x + _position.x, y + _position.y);
		}
	}
}

void MultiLineText::setTextColor(const sf::Color& color)
{
	_color = color;
	setParams();
}

void MultiLineText::setTextSize(int size)
{
	_size = size;
	setParams();
}

void MultiLineText::setWidth(int width)
{
	_width = width;
	setParams();
}

void MultiLineText::setString(const std::wstring& str)
{
	_string.clear();
	std::wstring word;
	for (auto it = str.begin(); it != str.end(); ++it)
	{
		word.push_back(*it);
		if (*it == *L" " || it == str.end() - 1)
		{
			_string.push_back(word);
			word.clear();
		}
	}
	setParams();
}

void MultiLineText::setPosition(int x, int y)
{
	_position.x = x;
	_position.y = y;
	setParams();
}

void MultiLineText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < _labels.size(); ++i)
	{
		target.draw(*_labels[i]);
	}
}
