#pragma once
#include "SFML/Graphics.hpp"
#include "Button.h"
#include "TextEdit.h"
#include "Label.h"
#include "Word.h"

/**
* \brief Creates an exercise where you need to write a translation of the word
*
* Set item positions, processes events, and displays items
*/
class WriteAnswer : public sf::Drawable
{
public:
	WriteAnswer();
	void setWidth(int width);
	void newWord(const Word& newWord);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void onClick(float x, float y);
	void onWrite(const sf::Event& event);
	void onPressEnterKey();
	bool answerIsCorrect();
	bool isAnswered() const;

private:
	std::wstring toLower(const std::wstring& str) const;

private:
	RoundedRectangle _rectangle;
	TextEdit _textField;
	Button _checkButton;
	Label _question;
	Label _rezult;
	Label _hint;
	Word _word;
	int _rectangleWidth;
	int _rectangleHeight;
	bool _answerIsGiven;
	bool _isCorrect;
	int _maxWidth;
};