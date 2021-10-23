#include "FindTranslation.h"

namespace
{
	const int wordSize = 25;    // search for words +/- wordSize % long from the original
	const int numberOfOptions = 6;
	const int questionSize = 30;
	const int rectVerticalBorders = 50;
	const int rectHorizontalBorders = 50;
	const sf::Color rectColor = sf::Color(153, 217, 234);
	const sf::Color questionColor = sf::Color(0, 0, 0);
	const sf::Color buttonTextColor = sf::Color(0, 0, 0);
	const sf::Color correctButtonColor = sf::Color(27, 190, 40);
	const sf::Color failButtonColor = sf::Color(200, 45, 45);
	const sf::Color neutralButtonColor = sf::Color(239, 228, 176);
	const int questionPosY = 100;
	const int buttonTextSize = 30;
	const int buttonHeight = buttonTextSize * 2;
	const int firstButtonPosY = 200;
	const int verticalDistanceBetweenButtons = 30;
	const int horizontalDistanceBetweenButtons = 50;
}

FindTranslation::FindTranslation()
{
	for (int i = 0; i < numberOfOptions; ++i)
	{
		_answerOptionsButtons.push_back(new Button);
	}
	_answerIsGiven = false;
}

FindTranslation::~FindTranslation()
{
	while (!_answerOptionsButtons.empty())
	{
		delete _answerOptionsButtons.back();
		_answerOptionsButtons.pop_back();
	}
}

void FindTranslation::setWidth(int width)
{
	_maximumWidth = width;
}

void FindTranslation::newTranslation(const Word* word, const std::deque<Word*>* dictionary, translateDirection td)
{
	_answerIsGiven = false;
	_guessingWord = word;
	_isCorrect = false;
	_question.setTextSize(questionSize);
	_question.setTextColor(questionColor);
	_rectangle.setColor(rectColor);
	for (int i = 0; i < _answerOptionsButtons.size(); ++i)
	{
		_answerOptionsButtons[i]->setColor(neutralButtonColor);
		_answerOptionsButtons[i]->setTextSize(buttonTextSize);
		_answerOptionsButtons[i]->setTextColor(buttonTextColor);
	}
	if (td == FindTranslation::translateDirection::EN_RU)
	{
		_question.setString(word->getOriginal());
		std::vector<int> answerOptions = findSimilarOptions(word, dictionary, FindTranslation::translateDirection::EN_RU);
		for (int i = 0; i < numberOfOptions; ++i)
		{
			_answerOptionsButtons.at(i)->setString(dictionary->at(answerOptions.at(i))->getFirstTranslation());
		}
	}
	if (td == FindTranslation::translateDirection::RU_EN)
	{
		_question.setString(word->getFirstTranslation());
		std::vector<int> answerOptions = findSimilarOptions(word, dictionary, FindTranslation::translateDirection::RU_EN);
		for (int i = 0; i < numberOfOptions; ++i)
		{
			_answerOptionsButtons.at(i)->setString(dictionary->at(answerOptions.at(i))->getOriginal());
		}
	}
	_question.setPosition((_maximumWidth - _question.getGlobalBounds().width) / 2, questionPosY);
	int widestButton = _answerOptionsButtons.at(0)->getSize().x;
	for (auto i : _answerOptionsButtons)
	{
		if (i->getSize().x > widestButton)
			widestButton = i->getSize().x;
	}
	int posX, posY;
	for (int i = 0; i < _answerOptionsButtons.size(); ++i)
	{
		_answerOptionsButtons.at(i)->setSize(widestButton, buttonHeight);
		if (i % 2 == 0)
		{
			posX = (_maximumWidth / 2) - ((horizontalDistanceBetweenButtons / 2) + widestButton);
		}
		else
		{
			posX = (_maximumWidth / 2) + (horizontalDistanceBetweenButtons / 2);
		}
		posY = firstButtonPosY + ((i / 2) * (_answerOptionsButtons.at(0)->getSize().y + verticalDistanceBetweenButtons));
		_answerOptionsButtons.at(i)->setPosition(sf::Vector2i(posX, posY));
	}
	int rectangleWidth = widestButton * 2 + rectHorizontalBorders * 2 + horizontalDistanceBetweenButtons;
	int rectangleHeight = _answerOptionsButtons.back()->getPosition().y + _answerOptionsButtons.back()->getSize().y - questionPosY + rectVerticalBorders * 2;
	posX = _maximumWidth / 2 - rectangleWidth / 2;
	posY = questionPosY - rectVerticalBorders;
	_rectangle.setSize(rectangleWidth, rectangleHeight);
	_rectangle.setPosition(sf::Vector2i(posX, posY));
	if(td == FindTranslation::translateDirection::EN_RU)
		word->say();
}

std::vector<int> FindTranslation::findSimilarOptions(const Word* word, const std::deque<Word*>* dictionary, translateDirection td)
{
	std::vector<int> similarFirstLetter = getSimilarFirstLetter(word, dictionary, td);
	int answerId = getAnswerId(word, dictionary);
	std::vector<int> similarWordLength = getSimilarWordLength(similarFirstLetter, word, dictionary, td);
	int seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(similarFirstLetter.begin(), similarFirstLetter.end(), std::default_random_engine(seed));
	shuffle(similarWordLength.begin(), similarWordLength.end(), std::default_random_engine(seed));
	std::vector<int> rezult;
	rezult.push_back(answerId);
	for (int i : similarWordLength)      // rezult is filled with indexes from similarNumberOfLetters
	{
		if (rezult.size() == numberOfOptions)
			break;
		bool itSynonym = false;
		for (int rez : rezult)
		{
			if ((*dictionary)[rez]->getFirstTranslation() == (*dictionary)[i]->getFirstTranslation())
				itSynonym = true;
		}
		if(!itSynonym)
			rezult.push_back(i);
	}
	for (auto i : similarFirstLetter)        // if there is a place in result - it is filled from similarFirstLetter
	{
		if (rezult.size() == numberOfOptions)
			break;
		bool itSynonym = false;
		for (int rez : rezult)
		{
			if ((*dictionary)[rez]->getFirstTranslation() == (*dictionary)[i]->getFirstTranslation())
				itSynonym = true;
		}
		if (!itSynonym)
			rezult.push_back(i);
	}
	while (rezult.size() < numberOfOptions)    // if the result is not complete - it is replenished with random indices
	{
		int index = std::rand() % dictionary->size();
		if (dictionary->size() > numberOfOptions)
		{
			bool itSynonym = false;
			for (int rez : rezult)
			{
				if ((*dictionary)[rez]->getFirstTranslation() == (*dictionary)[index]->getFirstTranslation())
					itSynonym = true;
			}
			if (!itSynonym)
				rezult.push_back(index);
		}
		else
			rezult.push_back(index);
		
	}
	shuffle(rezult.begin(), rezult.end(), std::default_random_engine(seed));
	return rezult;
}

void FindTranslation::onClick(float x, float y)
{
	if (!_answerIsGiven)
	{
		for (auto i : _answerOptionsButtons)
		{
			if (i->contains(x, y))
			{
				if ((i->getString() == _guessingWord->getOriginal()) || (i->getString() == _guessingWord->getFirstTranslation()))
				{
					i->setColor(correctButtonColor);
					_isCorrect = true;
				}
				else
				{
					i->setColor(failButtonColor);
					_isCorrect = false;
					for (auto i : _answerOptionsButtons)
					{
						if ((i->getString() == _guessingWord->getOriginal()) || (i->getString() == _guessingWord->getFirstTranslation()))
							i->setColor(correctButtonColor);
					}
				}
				_answerIsGiven = true;
				_guessingWord->say();
			}
		}
	}
}

void FindTranslation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_rectangle);
	target.draw(_question);
	for (auto i : _answerOptionsButtons)
		target.draw(*i);
}

bool FindTranslation::answerIsCorrect()
{
	return _isCorrect;
}

bool FindTranslation::isAnswered() const
{
	return _answerIsGiven;
}

int FindTranslation::getAnswerId(const Word* word, const std::deque<Word*>* dictionary)
{
	for (int i = 0; i < dictionary->size(); ++i)
	{
		if ((*dictionary)[i]->getOriginal() == word->getOriginal() && (*dictionary)[i]->getAllTranslation() == word->getAllTranslation())
		{
			return i;
		}
	}
}

int FindTranslation::getBoundsOfWordSize(const Word* word, translateDirection td)
{
	switch (td)
	{
	case FindTranslation::translateDirection::EN_RU:
		return float(word->getFirstTranslation().size()) / 100.0f * wordSize;
		break;
	case FindTranslation::translateDirection::RU_EN:
		return float(word->getOriginal().size()) / 100.0f * wordSize;
		break;
	}
}

std::vector<int> FindTranslation::getSimilarFirstLetter(const Word* word, const std::deque<Word*>* dictionary, translateDirection td)
{
	std::vector<int> rezult;
	switch (td)
	{
	case FindTranslation::translateDirection::EN_RU:
		for (int i = 0; i < dictionary->size(); ++i)
		{
			if ((*dictionary)[i]->getFirstTranslation().at(0) == word->getFirstTranslation().at(0))
			{
				rezult.push_back(i);
			}
		}
		break;
	case FindTranslation::translateDirection::RU_EN:
		for (int i = 0; i < dictionary->size(); ++i)
		{
			if ((*dictionary)[i]->getOriginal().at(0) == word->getOriginal().at(0))
			{
				rezult.push_back(i);
			}
		}
		break;
	default:
		break;
	}
	return rezult;
}

std::vector<int> FindTranslation::getSimilarWordLength(const std::vector<int>& similarFirstLetter, const Word* word, const std::deque<Word*>* dictionary, translateDirection td)
{
	std::vector<int> rezult;
	int boundsOfWordSize = getBoundsOfWordSize(word, td);
	switch (td)
	{
	case FindTranslation::translateDirection::EN_RU:
		for (int i = 0; i < similarFirstLetter.size(); ++i)
		{
			if (((*dictionary)[similarFirstLetter[i]]->getFirstTranslation().size() > word->getFirstTranslation().size() - boundsOfWordSize) &&
				((*dictionary)[similarFirstLetter[i]]->getFirstTranslation().size() < word->getFirstTranslation().size() + boundsOfWordSize))
			{
				rezult.push_back(similarFirstLetter[i]);
			}
		}
		break;
	case FindTranslation::translateDirection::RU_EN:
		for (int i = 0; i < similarFirstLetter.size(); ++i)
		{
			if (((*dictionary)[similarFirstLetter[i]]->getOriginal().size() > word->getOriginal().size() - boundsOfWordSize) &&
				((*dictionary)[similarFirstLetter[i]]->getOriginal().size() < word->getOriginal().size() + boundsOfWordSize))
			{
				rezult.push_back(similarFirstLetter[i]);
			}
		}
		break;
	default:
		break;
	}
	return rezult;
}
