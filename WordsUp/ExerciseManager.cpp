#include "ExerciseManager.h"

namespace
{
	const std::wstring labelBtnNext = L"Дальше >";
	const int textSizeBtnNext = 30;
	const sf::Vector2i sizeBtnNext = sf::Vector2i(270, 55);
	sf::Vector2i positionBtnNext = sf::Vector2i(800, 525);
	const sf::Color colorBtnNext = sf::Color(0, 135, 0);
	const sf::Color textColorBtnNext = sf::Color(255, 242, 0);
	const sf::Color background = sf::Color(112, 146, 190);
}

ExerciseManager::ExerciseManager()
{
	_user = nullptr;
	_window = nullptr;
}

ExerciseManager::Answers::Answers(int word, int answers)
{
	_wordID = word;
	_answers = answers;
}

void ExerciseManager::startExercises(User* user, sf::RenderWindow* window)
{
	init(user, window);
	int windowWidth = _window->getSize().x;
	_buttonNext.setString(labelBtnNext);
	_buttonNext.setTextSize(textSizeBtnNext);
	_buttonNext.setSize(sizeBtnNext.x, sizeBtnNext.y);
	_buttonNext.setColor(colorBtnNext);
	_buttonNext.setTextColor(textColorBtnNext);
	positionBtnNext.x = windowWidth / 2 - _buttonNext.getSize().x / 2;
	_buttonNext.setPosition(positionBtnNext);
	_findTrans.setWidth(windowWidth);
	_collect.setWidth(windowWidth);
	_writeAns.setWidth(windowWidth);

    if (user != nullptr)
    {
        _mode = Mode::EN_RU;
		createExerciseProgressLine();
		setExercise();
    }
	_isStarted = true;
	while (_window->isOpen() && (exercisesStarted() && !exercisesOver()))
	{
		sf::Event event;
		while (_window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				_window->close();
			}
			processingEvents(event);
		}
		_window->clear(background);
		_window->draw(*this);
		_window->display();
	}
}

void ExerciseManager::setUser(User* user)
{
	_user = user;
}

void ExerciseManager::setWindow(sf::RenderWindow* window)
{
	_window = window;
}

void ExerciseManager::init(User* user, sf::RenderWindow* window)
{
	setUser(user);
	setWindow(window);
}

void ExerciseManager::createExerciseProgressLine()
{
	_userDictionary = _user->getDictionary();
	if (!_repeatMode)
	{
		_firstRepeat = _user->getFirstRepeatDictionary();
		_secondRepeat = _user->getSecondRepeatDictionary();
		int numWordsToStudy = _user->getNumberOfWordsToStudy();
		for (int i = 0; i < numWordsToStudy; ++i)
			_exerciseProgress.push_back(Answers(i, _user->getRepeatCount()));
		for (int i = _exerciseProgress.size(); i < numWordsToStudy + _firstRepeat->size(); ++i)
			_exerciseProgress.push_back(Answers(i, _user->getFirstRepeatCount()));
		for (int i = _exerciseProgress.size(); i < _exerciseDictionary.size(); ++i)
			_exerciseProgress.push_back(Answers(i, _user->getSecondRepeatCount()));
	}
	else
	{
		for (int i = 0; i < _exerciseDictionary.size(); ++i)
			_exerciseProgress.push_back(Answers(i, _user->getRepeatCount()));
	}
}

void ExerciseManager::setExercise()
{
	srand(time(NULL));
	_wordId = std::rand() % _exerciseProgress.size();
	_pastWord = _exerciseDictionary.at(_exerciseProgress[_wordId]._wordID);
	_tempDictionary.insert(_tempDictionary.begin(), _userDictionary->begin(), _userDictionary->end());
	_tempDictionary.insert(_tempDictionary.end(), _exerciseDictionary.begin(), _exerciseDictionary.end());
	_findTrans.newTranslation(_exerciseDictionary.at(_exerciseProgress[_wordId]._wordID), &_tempDictionary, FindTranslation::translateDirection::EN_RU);
}

void ExerciseManager::processingEvents(sf::Event& event)
{
	if (!_exerciseProgress.empty())
	{
		if (event.type == sf::Event::Closed)
		{
			_window->close();
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				switch (_mode)
				{
				case Mode::EN_RU:
				case Mode::RU_EN:
					_findTrans.onClick(event.mouseButton.x, event.mouseButton.y);
					break;
				case Mode::COLLECT:
					_collect.onClick(event.mouseButton.x, event.mouseButton.y);
					break;
				case Mode::WRITE:
					_writeAns.onClick(event.mouseButton.x, event.mouseButton.y);
					break;
				}
				onClickNext(event.mouseButton.x, event.mouseButton.y);
			}
		}
		if (event.type == sf::Event::TextEntered)
		{
			if (_mode == Mode::WRITE)
				_writeAns.onWrite(event);
		}
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Enter)
			{
				if (_mode == Mode::WRITE)
					_writeAns.onPressEnterKey();
			}
		}
	}
}

void ExerciseManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.clear(background);
	switch (_mode)
	{
	case Mode::EN_RU:
	case Mode::RU_EN:
		target.draw(_findTrans);
		if (_findTrans.isAnswered())
			target.draw(_buttonNext);
		break;
	case Mode::COLLECT:
		target.draw(_collect);
		if (_collect.isAnswered())
			target.draw(_buttonNext);
		break;
	case Mode::WRITE:
		target.draw(_writeAns);
		if (_writeAns.isAnswered())
			target.draw(_buttonNext);
		break;
	}
}

bool ExerciseManager::exercisesOver() const
{
	if (_exerciseProgress.empty())
		return true;
	return false;
}

bool ExerciseManager::exercisesStarted() const
{
	return _isStarted;
}

void ExerciseManager::setExerciseDictionary(std::deque<Word*>& exDict)
{
	_exerciseDictionary = exDict;
}

void ExerciseManager::setRepeatMode(bool repeatMode)
{
	_repeatMode = repeatMode;
}

void ExerciseManager::onClickNext(float x, float y)
{
	if (_buttonNext.contains(x, y))
	{
		switch (_mode)
		{
		case Mode::EN_RU:
			if (_findTrans.isAnswered())
			{
				if (_findTrans.answerIsCorrect())
					--_exerciseProgress[_wordId]._answers;
				if (_exerciseProgress[_wordId]._answers == 0)
					_exerciseProgress.erase(_exerciseProgress.begin() + _wordId);
				if (!_exerciseProgress.empty())
				{
					nextRandomWord();
					_findTrans.newTranslation(_exerciseDictionary.at(_exerciseProgress[_wordId]._wordID), &_tempDictionary, FindTranslation::translateDirection::EN_RU);
				}
				else
				{
					prepareToNextExecise();
					_findTrans.newTranslation(_exerciseDictionary.at(_exerciseProgress[_wordId]._wordID), &_tempDictionary, FindTranslation::translateDirection::RU_EN);
				}
			}
			break;
		case Mode::RU_EN:
			if (_findTrans.isAnswered())
			{
				if (_findTrans.answerIsCorrect())
					--_exerciseProgress[_wordId]._answers;
				if (_exerciseProgress[_wordId]._answers == 0)
					_exerciseProgress.erase(_exerciseProgress.begin() + _wordId);
				if (!_exerciseProgress.empty())
				{
					nextRandomWord();
					_findTrans.newTranslation(_exerciseDictionary.at(_exerciseProgress[_wordId]._wordID), &_tempDictionary, FindTranslation::translateDirection::RU_EN);
				}
				else
				{
					prepareToNextExecise();
					_collect.newShufleWord(*(_exerciseDictionary.at(_exerciseProgress[_wordId]._wordID)));
				}
			}
			break;
		case Mode::COLLECT:
			if (_collect.isAnswered())
			{
				if (_collect.answerIsCorrect())
					--_exerciseProgress[_wordId]._answers;
				if (_exerciseProgress[_wordId]._answers == 0)
					_exerciseProgress.erase(_exerciseProgress.begin() + _wordId);
				if (!_exerciseProgress.empty())
				{
					nextRandomWord();
					_collect.newShufleWord(*(_exerciseDictionary.at(_exerciseProgress[_wordId]._wordID)));
				}
				else
				{
					prepareToNextExecise();
					_writeAns.newWord(*(_exerciseDictionary.at(_exerciseProgress[_wordId]._wordID)));
				}
			}
			break;
		case Mode::WRITE:
			if (_writeAns.isAnswered())
			{
				if (_writeAns.answerIsCorrect())
					--_exerciseProgress[_wordId]._answers;
				if (_exerciseProgress[_wordId]._answers == 0)
					_exerciseProgress.erase(_exerciseProgress.begin() + _wordId);
				if (!_exerciseProgress.empty())
				{
					nextRandomWord();
					_writeAns.newWord(*(_exerciseDictionary.at(_exerciseProgress[_wordId]._wordID)));
				}
				else
				{
					if (!_repeatMode)
					{
						_user->changeDictionarysAfterExercises(&_exerciseDictionary);
						_user->saveDictionarys();
					}
					delete _user;
					_user = nullptr;
				}
			}
			break;
		}
	}
}

void ExerciseManager::nextRandomWord()
{
	_wordId = std::rand() % _exerciseProgress.size();
	if (_exerciseProgress.size() > 1)
	{
		while (_exerciseDictionary.at(_exerciseProgress[_wordId]._wordID)->getOriginal() == _pastWord->getOriginal())
		{
			_wordId = std::rand() % _exerciseProgress.size();
		}
	}
	_pastWord = _exerciseDictionary.at(_exerciseProgress[_wordId]._wordID);
}

void ExerciseManager::prepareToNextExecise()
{
	setNextMode();
	if (!_repeatMode)
	{
		for (int i = 0; i < _user->getNumberOfWordsToStudy(); ++i)
			_exerciseProgress.push_back(Answers(i, _user->getRepeatCount()));
		for (int i = _exerciseProgress.size(); i < _user->getNumberOfWordsToStudy() + _firstRepeat->size(); ++i)
			_exerciseProgress.push_back(Answers(i, _user->getFirstRepeatCount()));
		for (int i = _exerciseProgress.size(); i < _exerciseDictionary.size(); ++i)
			_exerciseProgress.push_back(Answers(i, _user->getSecondRepeatCount()));
	}
	else
	{
		for (int i = 0; i < _exerciseDictionary.size(); ++i)
			_exerciseProgress.push_back(Answers(i, _user->getRepeatCount()));
	}
	_wordId = std::rand() % _exerciseProgress.size();
}

void ExerciseManager::setNextMode()
{
	switch (_mode)
	{
	case Mode::EN_RU:
		_mode = Mode::RU_EN;
		break;
	case Mode::RU_EN:
		_mode = Mode::COLLECT;
		break;
	case Mode::COLLECT:
		_mode = Mode::WRITE;
		break;
	case Mode::WRITE:
		break;
	}
}
