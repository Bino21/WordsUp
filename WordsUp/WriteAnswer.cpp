#include "WriteAnswer.h"

namespace
{
	const int rectBounds = 50;
	const int questionPosY = 100;
	const int questionTextSize = 30;
	const int btnTextSize = 30;
	const int btnWidth = 100;
	const int textFieldPosY = 170;
	const int textFieldHeight = 50;
	const int rezultPosY = 250;
	const int rezultTextSize = 40;
	const int hintPosY = 300;
	const int hintTextSize = 20;
	const sf::Color rectColor = sf::Color(153, 217, 234);
	const sf::Color questionColor = sf::Color(0, 0, 0);
	const sf::Color correctRezultColor = sf::Color(0, 155, 0);
	const sf::Color failRezultColor = sf::Color(200, 0, 0);
	const sf::Color hintColor = sf::Color(0, 0, 0);
	const sf::Color btnColor = sf::Color(0, 135, 0);
	const sf::Color btnTextColor = sf::Color(255, 242, 0);
	const int textFieldWidth = 500;
	const int indent = 50;
}

WriteAnswer::WriteAnswer()
{
	_rectangleWidth = textFieldWidth + indent + btnWidth + rectBounds * 2;
	_rectangleHeight = hintPosY - questionPosY + hintTextSize + rectBounds * 2;
	_rectangle.setSize(_rectangleWidth, _rectangleHeight);
	_rectangle.setColor(rectColor);
	_question.setTextColor(questionColor);
	_question.setTextSize(questionTextSize);
	_textField.setSize(textFieldWidth, textFieldHeight);
	_checkButton.setSize(btnWidth, textFieldHeight);
	_checkButton.setTextSize(btnTextSize, Button::AutoSize::NO);
	_checkButton.setString(L"Готово", Button::AutoSize::NO);
	_checkButton.setColor(btnColor);
	_checkButton.setTextColor(btnTextColor);
	_rezult.setTextSize(rezultTextSize);
	_hint.setTextColor(hintColor);
	_hint.setTextSize(hintTextSize);
	_answerIsGiven = false;
}

void WriteAnswer::newWord(const Word& newWord)
{
	_word = newWord;
	_textField.setString(L"");
	_answerIsGiven = false;
	_isCorrect = false;
	int bigerWidth;
	_question.setString(_word.getAllTranslation());
	_question.setPosition(_maxWidth / 2 - _question.getGlobalBounds().width / 2, questionPosY);
	_textField.setPosition(sf::Vector2i(_maxWidth / 2 - (textFieldWidth + indent + btnWidth) / 2, textFieldPosY));
	_checkButton.setPosition(sf::Vector2i(_textField.getTextPosition().x + textFieldWidth + indent, textFieldPosY));
	if (_question.getGlobalBounds().width > (_rectangleWidth - rectBounds * 2))
	{
		bigerWidth = _question.getGlobalBounds().width + rectBounds * 2;
		_rectangle.setSize(bigerWidth, _rectangleHeight);
	}
	else
	{
		_rectangle.setSize(_rectangleWidth, _rectangleHeight);
	}
	_rectangle.setPosition(sf::Vector2i(_maxWidth / 2 - _rectangleWidth / 2, questionPosY - rectBounds));
	_rezult.setString(L"");
	_hint.setString(L"");
	onClick(_textField.getTextPosition().x, _textField.getTextPosition().y);
}

void WriteAnswer::setWidth(int width)
{
	_maxWidth = width;

}

void WriteAnswer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_rectangle);
	target.draw(_question);
	target.draw(_textField);
	target.draw(_checkButton);
	target.draw(_rezult);
	target.draw(_hint);
}

std::wstring WriteAnswer::toLower(const std::wstring& str) const
{
	std::wstring rezult;
	for (auto ch : str)
	{
		if (int(ch) >= 97 && int(ch) <= 122)
			rezult.push_back(ch - 32);
		else if ((int(ch) >= 65 && int(ch) <= 90) || (int(ch) == 39))
			rezult.push_back(ch);
	}
	return rezult;
}

void WriteAnswer::onClick(float x, float y)
{
	if (!_answerIsGiven)
	{
		if (_checkButton.contains(x, y))
		{
			std::wstring textLower = toLower(_textField.getText());
			std::wstring answerLower = toLower(_word.getOriginal());
			if (textLower == answerLower)
			{
				_rezult.setString(L"Правильно");
				_rezult.setTextColor(correctRezultColor);
				_isCorrect = true;
			}
			else
			{
				_rezult.setString(L"Ошибка");
				_rezult.setTextColor(failRezultColor);
				_isCorrect = false;
				_hint.setString(_word.getOriginal() + L" - " + _word.getFirstTranslation());
				_hint.setPosition(_maxWidth / 2 - _hint.getGlobalBounds().width / 2, hintPosY);
			}
			_rezult.setPosition(_maxWidth / 2 - _rezult.getGlobalBounds().width / 2, rezultPosY);
			_answerIsGiven = true;
			_word.say();
		}
		_textField.onClick(x, y);
	}
}

void WriteAnswer::onWrite(const sf::Event& event)
{
	_textField.onWrite(event);
}

void WriteAnswer::onPressEnterKey()
{
	onClick(_checkButton.getPosition().x + _checkButton.getSize().x / 2, _checkButton.getPosition().y + _checkButton.getSize().y / 2);
}

bool WriteAnswer::answerIsCorrect()
{
	return _isCorrect;
}

bool WriteAnswer::isAnswered() const
{
	return _answerIsGiven;
}
		
