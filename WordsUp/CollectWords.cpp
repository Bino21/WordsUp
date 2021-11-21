#include "CollectWords.h"

namespace
{
	const int radiusRoundedRect = 10;
	const int heightTopRect = 200;
	const int heightBottomRect = 110;
	const int topRectY = 50;
	const int bottomRectY = 370;
	const int roundedRectBorders = 50;
	const int questionTextSize = 30;
	const int resultTextSize = 50;
	const int hintTextSize = 25;
	const int hintY = 405;
	const int questionY = 70;
	const int resultY = 275;
	const int sortY = 150;    // позиция y для отсортированых букв
	const int notSortY = 400; // позиция y для неотсортированых букв
	const float spaceBetweenLetters = 0.07f; //зазор между квадратами букв
	const sf::Color questionColor = sf::Color::Black;
	const sf::Color correctResultColor = sf::Color(10, 255, 10);
	const sf::Color failResultColor = sf::Color(200, 0, 0);
	const sf::Color failLettersColor = sf::Color(248, 128, 7);
	const sf::Color rectColor = sf::Color(153, 217, 234);
	const sf::Color hintColor = sf::Color::Black;
}


CollectWords::CollectWords()
{
	_question.setTextColor(questionColor);
	_question.setTextSize(questionTextSize);
	_result.setTextSize(resultTextSize);
	_hint.setTextSize(hintTextSize);
	_hint.setTextColor(hintColor);
	_rRectTop.setRadius(radiusRoundedRect);
	_rRectDown.setRadius(radiusRoundedRect);
	_rRectTop.setColor(rectColor);
	_rRectDown.setColor(rectColor);
	_answerIsGiven = false;
}



void CollectWords::setWidth(int width)
{
	_maximumWidth = width;
}

void CollectWords::newShufleWord(const Word& newWord)
{
	for (auto it = _shufleWord.begin(); it != _shufleWord.end(); ++it)
	{
		delete* it;
	}
	_shufleWord.clear();
	_word = newWord;
	_isCorrect = false;
	_answerIsGiven = false;
	for (auto ch : _word.getOriginal())
	{
		_shufleWord.push_back(new Letter(ch));
	}
	int seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(_shufleWord.begin(), _shufleWord.end(), std::default_random_engine(seed));
	int distanceBetweenLetters = Letter::size + (Letter::size * spaceBetweenLetters);
	int posX_firstLetter = (_maximumWidth / 2) - (_word.getOriginal().length() * distanceBetweenLetters / 2);
	int count = 0;
	for (auto it = _shufleWord.begin(); it != _shufleWord.end(); ++it)
	{
		(*it)->setPosition(posX_firstLetter + (distanceBetweenLetters * count), notSortY);
		++count;
	}
	_countSortPositionLetters = 0;
	_question.setString(_word.getAllTranslation());
	_question.setPosition((_maximumWidth - _question.getGlobalBounds().width) / 2, questionY);
	_result.setString(L"");
	_hint.setString(L"");
	int rectWidth = _word.getOriginal().length() * distanceBetweenLetters + (roundedRectBorders * 2);
	if (_question.getGlobalBounds().width + (roundedRectBorders * 2) > rectWidth)
		rectWidth = _question.getGlobalBounds().width + (roundedRectBorders * 2);
	_rRectTop.setSize(rectWidth, heightTopRect);
	_rRectDown.setSize(rectWidth, heightBottomRect);
	_rRectTop.setPosition(sf::Vector2i((_maximumWidth / 2) - (_rRectTop.getSize().x / 2), topRectY));
	_rRectDown.setPosition(sf::Vector2i((_maximumWidth / 2) - (_rRectDown.getSize().x / 2), bottomRectY));
}

void CollectWords::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_rRectTop);
	target.draw(_rRectDown);
	for (auto it = _shufleWord.begin(); it != _shufleWord.end(); ++it)
	{
		target.draw(*(*it));
	}
	target.draw(_question);
	target.draw(_result);
	target.draw(_hint);
}

void CollectWords::onClick(float x, float y)
{
	int distanceBetweenLetters = Letter::size + (Letter::size * spaceBetweenLetters);
	int posX_firstLetter = (_maximumWidth / 2) - (_word.getOriginal().length() * distanceBetweenLetters / 2);
	for (auto it = _shufleWord.begin(); it != _shufleWord.end(); ++it)
	{
		if ((*it)->getGlobalBounds().contains(x, y))
		{
			if ((*it)->getPosition().y == notSortY)
			{
				(*it)->setPosition(posX_firstLetter + (distanceBetweenLetters * _countSortPositionLetters), sortY);
				++_countSortPositionLetters;
				if (_countSortPositionLetters == _word.getOriginal().length())
					wordAudit();
			}
		}
	}
}

void CollectWords::wordAudit()
{ 
	std::wstring answer;
	std::sort(_shufleWord.begin(), _shufleWord.end(), [](const Letter* l1, const Letter* l2)
		{
			return l1->getPosition().x < l2->getPosition().x;
		});
	for (Letter* letter : _shufleWord)
	{
		answer.push_back(letter->getLetter());
	}
	if (answer == _word.getOriginal())
	{
		_result.setTextColor(correctResultColor);
		_result.setString(L"ПРАВИЛЬНО!");
		_result.setPosition((_maximumWidth - _result.getGlobalBounds().width) / 2, resultY);
		_isCorrect = true;
	}
	else
	{
		_result.setTextColor(failResultColor);
		_result.setString(L"ОШИБКА!");
		_isCorrect = false;
		_result.setPosition((_maximumWidth - _result.getGlobalBounds().width) / 2, resultY);
		_hint.setString(_word.getOriginal() + L" - " + _word.getAllTranslation());
		_hint.setPosition((_maximumWidth - _hint.getGlobalBounds().width) / 2, hintY);
		for (int i = 0; i < _word.getOriginal().size(); ++i)
		{
			if (answer[i] != _word.getOriginal()[i])
				_shufleWord[i]->setTextColor(failLettersColor);
		}
	}
	_answerIsGiven = true;
	_word.say();
}

bool CollectWords::answerIsCorrect() const
{
	return _isCorrect;
}

bool CollectWords::isAnswered() const
{
	return _answerIsGiven;
}
