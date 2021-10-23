#pragma once
#include <vector>
#include <deque>
#include <random>
#include <chrono>
#include <algorithm>
#include "Button.h"
#include "RoundedRectangle.h"
#include "Word.h"

/**
* \brief Creates an exercise where you need to choose the correct translation of the word
* 
* Controls item positions, processes events, and displays items
*/
class FindTranslation : public sf::Drawable
{
public:
	enum class translateDirection { EN_RU, RU_EN };

public:
	FindTranslation();
	~FindTranslation();
	void setWidth(int width);
	void newTranslation(const Word* word, const std::deque<Word*>* dictionary, translateDirection td);
	void onClick(float x, float y);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool answerIsCorrect();
	bool isAnswered() const;

private:
	int getAnswerId(const Word* word, const std::deque<Word*>* dictionary);
	int getBoundsOfWordSize(const Word* word, translateDirection td);
	std::vector<int> getSimilarFirstLetter(const Word* word, const std::deque<Word*>* dictionary, translateDirection td);
	std::vector<int> getSimilarWordLength(const std::vector<int>& similarFirstLetter, const Word* word, const std::deque<Word*>* dictionary, translateDirection td);
	std::vector<int> findSimilarOptions(const Word* word, const std::deque<Word*>* dictionary, translateDirection td);

private:
	
	Label _question;
	std::vector<Button*> _answerOptionsButtons;
	const Word* _guessingWord;
	RoundedRectangle _rectangle;
	bool _answerIsGiven;
	bool _isCorrect;
	int _maximumWidth;
};

