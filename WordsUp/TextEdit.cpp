#include "TextEdit.h"

namespace
{
	const float textSize = 0.7f;
	const float airSpaceUnderLabel = 1.2f;
	const sf::Vector2i bordersInsideTextField = sf::Vector2i(4, 3);
	const sf::Color descriptionColor = sf::Color(0, 0, 0);
	const sf::Color textFieldColor = sf::Color(251, 247, 215);
	const sf::Color textColor = sf::Color(0, 0, 0);
}

TextEdit::TextEdit()
{
	_text.setTextSize(_height * textSize);
	_text.setTextColor(textColor);
	_label.setTextColor(descriptionColor);
	_textField.setFillColor(textFieldColor);
	_str = sf::String("");
	_text.setString(_str);
}

void TextEdit::setLabel(const std::wstring& str)
{
	_label.setString(str);
	setPosition(sf::Vector2i(_label.getPosition().x, _label.getPosition().y));
}

void TextEdit::setFocus(bool focus)
{
	_focus = focus;
	if (_focus)
	{
		if (!_str.isEmpty())
		{
			if (_str[_str.getSize() - 1] != sf::String(L"_"))
			{
				_str += L"_";
			}
		}
		else
			_str += L"_";
		_text.setString(_str);
	}
	else
	{
		if (!_str.isEmpty())
		{
			if (_str[_str.getSize() - 1] == sf::String(L"_"))
				_str.erase(_str.getSize() - 1);
		}
		_text.setString(_str);
	}
}

void TextEdit::setLabelSize(int size)
{
	_label.setTextSize(size);
	setPosition(sf::Vector2i(_label.getPosition().x, _label.getPosition().y));
}

void TextEdit::setPosition(const sf::Vector2i& position)
{
	if (_label.getString() == L"")
	{
		_textField.setPosition(position.x, position.y);
		_text.setPosition(_textField.getPosition().x + bordersInsideTextField.x, _textField.getPosition().y + bordersInsideTextField.y);
	}
	else
	{
		_label.setPosition(position.x, position.y);
		_textField.setPosition(position.x, position.y + _label.getTextSize() * airSpaceUnderLabel);
		_text.setPosition(_textField.getPosition().x + bordersInsideTextField.x, _textField.getPosition().y + bordersInsideTextField.y);
	}
}

void TextEdit::setSize(int width, int height)
{
	_height = height;
	_text.setTextSize(height * textSize);
	_textField.setSize(sf::Vector2f(width, height));
}

void TextEdit::setLabelColor(const sf::Color& color)
{
	_label.setTextColor(color);
}

void TextEdit::setTextColor(const sf::Color& color)
{
	_text.setTextColor(color);
}

void TextEdit::setTextFiedColor(const sf::Color& color)
{
	_textField.setFillColor(color);
}

void TextEdit::onClick(float x, float y)
{
	if (_label.getGlobalBounds().contains(x, y) || _textField.getGlobalBounds().contains(x, y))
		setFocus(true);
	else
		setFocus(false);
}

void TextEdit::onWrite(const sf::Event& event)
{
	if (_focus)
	{
		if (event.type == sf::Event::TextEntered)
		{
			if (event.text.unicode == 8)  // backspace
			{
				if (_str.getSize() > 1)
				{
					_str.erase(_str.getSize() - 2);
				}
			}
			else
			{
				if (_text.getGlobalBounds().width + _text.getTextSize() < _textField.getGlobalBounds().width)
				{
					if (!_str.isEmpty())
						_str.insert(_str.getSize() - 1, event.text.unicode);
				}
			}
			_text.setString(_str);
		}
	}
}

bool TextEdit::onFocus() const
{
	return _focus;
}

void TextEdit::setString(const std::wstring& str)
{
	_str = str;
	_text.setString(_str);
}


void TextEdit::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_label);
	target.draw(_textField);
	target.draw(_text);
}

std::wstring TextEdit::getText()
{
	std::wstring rezult = _str.toWideString();
	if (!rezult.empty())
	{
		if (rezult.at(rezult.size() - 1) == wchar_t('_'))
			rezult.erase(rezult.size() - 1);
	}
	else
		rezult.erase();
	return rezult;
}

sf::Vector2i TextEdit::getTextPosition() const
{
	return sf::Vector2i(_textField.getPosition());
}