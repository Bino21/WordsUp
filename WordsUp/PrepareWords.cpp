#include "PrepareWords.h"

namespace
{
	const int firstLinePosY = 50;
	const int verticalSpace = 25;
    const int repVerticalSpace = 15;
	const int horizontalSpace = 20;
	const int textSize = 25;
	const int btnOkPosY = 540;
    const int numShownRepeatWords = 7;
	const sf::Color colorBtn = sf::Color(0, 135, 0);
	const sf::Color textColorBtn = sf::Color(255, 242, 0);
	const sf::Color rectColor = sf::Color(153, 217, 234);
	const sf::Color textColor = sf::Color(0, 0, 0);
	const sf::Color background = sf::Color(112, 146, 190);
	const int textSizeWordsLearnedDigit = 25;
	const int textSizeWordsLearned = 18;
	const int borders = 30;
	const sf::Color colorLearnedLabel = sf::Color(0, 0, 0);
}

void PrepareWords::setLearnedWordsParams()
{
    _wordsLearned.setTextSize(textSizeWordsLearned);
    _wordsLearned.setString(L"слов выучено:");
    _wordsLearned.setTextColor(colorLearnedLabel);
    _wordsLearnedDigit.setTextSize(textSizeWordsLearnedDigit);
    _wordsLearnedDigit.setString(L" " + std::to_wstring(_user->getLearnedWords()->size()));
    _wordsLearnedDigit.setTextColor(colorLearnedLabel);
    int x = _window->getSize().x - (_wordsLearned.getGlobalBounds().width + _wordsLearnedDigit.getGlobalBounds().width + borders);
    int y = _window->getSize().y - (borders + _wordsLearned.getGlobalBounds().height);
    _wordsLearned.setPosition(x, y);
    x = _wordsLearned.getPosition().x + _wordsLearned.getGlobalBounds().width;
    y = _window->getSize().y - (borders + _wordsLearnedDigit.getGlobalBounds().height);
    _wordsLearnedDigit.setPosition(x, y);
}

void PrepareWords::lineInit(lineInPrepareWords* line, Word* word)
{
    line->_rect.setSize(_window->getSize().x - horizontalSpace * 2, textSize * 2);
    line->_rect.setColor(rectColor);
    line->_wordLabel.setTextSize(textSize);
    line->_wordLabel.setString(word->getOriginal() + L" - " + word->getAllTranslation());
    line->_btn1.setTextSize(textSize);
    line->_btn1.setColor(colorBtn);
    line->_btn1.setTextColor(textColorBtn);
    if (!_repeatMode)
    {
        line->_btn1.setString(L"Уже знаю");
        line->_btn1.setSize(line->_btn1.getSize().x, textSize * 1.7);
    }
    else
    {
        line->_btn1.setString(L"Повторить");
        line->_btn1.setSize(line->_btn1.getSize().x, textSize * 1.7);
        line->_btn2.setString(L"Не помню");
        line->_btn2.setTextSize(textSize);
        line->_btn2.setColor(colorBtn);
        line->_btn2.setTextColor(textColorBtn);
        line->_btn2.setSize(line->_btn2.getSize().x, textSize * 1.7);
        if(word->itsInRepeatList())
            line->_rect.setColor(sf::Color(150, 217, 150));
    }
}

PrepareWords::PrepareWords(User* user, sf::RenderWindow* window, bool repeatMode)
{
    _repeatMode = repeatMode;
    init(user, window);
    if (!_repeatMode)
        initLines();
    else
        initRepeatLines();
}

PrepareWords::~PrepareWords()
{
    for (auto i : _wordsToStudy)
        delete i;
}

void PrepareWords::init(User* user, sf::RenderWindow* window)
{
    _user = user;
    _window = window;
    _okButton.setTextSize(35);
    _okButton.setString(L"OK");
    _okButton.setColor(colorBtn);
    _okButton.setTextColor(textColorBtn);
    _okButton.setSize(200, 50);
    _okButton.setPosition(sf::Vector2i(_window->getSize().x / 2 - _okButton.getSize().x / 2, btnOkPosY));
}

void PrepareWords::initLines()
{
	int count = _user->getNumberOfWordsToStudy();
	int x, y;
    for (auto i : _wordsToStudy)
        delete i;
    _wordsToStudy.clear();
	if (count <= _user->getDictionary()->size())
	{
		for (int i = 0; i < count; ++i)
		{
			_wordsToStudy.push_back(new lineInPrepareWords);
			lineInit(_wordsToStudy[i], _user->getDictionary()->at(i));
			y = firstLinePosY + (_wordsToStudy[i]->_rect.getSize().y + verticalSpace) * i;
			_wordsToStudy[i]->_rect.setPosition(sf::Vector2i(horizontalSpace, y));
			_wordsToStudy[i]->_wordLabel.setPosition(horizontalSpace * 2, _wordsToStudy[i]->_rect.getPosition().y + textSize / 4);
			_wordsToStudy[i]->_wordLabel.setTextColor(textColor);
			x = _window->getSize().x - (_wordsToStudy[i]->_btn1.getSize().x + horizontalSpace * 2);
			y = _wordsToStudy[i]->_rect.getPosition().y + (_wordsToStudy[i]->_rect.getSize().y - _wordsToStudy[i]->_btn1.getSize().y) / 2;
			_wordsToStudy[i]->_btn1.setPosition(sf::Vector2i(x, y));
            _wordsToStudy[i]->_btn2.setPosition(sf::Vector2i(-1000, -1000));
		}
		setLearnedWordsParams();
	}
	else
		std::cout << "Мало слов в словаре, добавте новый словарь" << std::endl;
}

void PrepareWords::initRepeatLines()
{
    int x, y;
    std::deque<Word*>* learnedDictionary = _user->getLearnedWords();
    for (auto i : _wordsToStudy)
        delete i;
    _wordsToStudy.clear();
    for (int i = numFirstLine; i < numFirstLine + numShownRepeatWords && i < learnedDictionary->size(); ++i)
    {
        _wordsToStudy.push_back(new lineInPrepareWords);
        lineInit(_wordsToStudy[i - numFirstLine], learnedDictionary->at(i));
        y = firstLinePosY + (_wordsToStudy[i - numFirstLine]->_rect.getSize().y + repVerticalSpace) * (i - numFirstLine);
        _wordsToStudy[i - numFirstLine]->_rect.setPosition(sf::Vector2i(horizontalSpace, y));
        _wordsToStudy[i - numFirstLine]->_wordLabel.setPosition(horizontalSpace * 2, _wordsToStudy[i - numFirstLine]->_rect.getPosition().y + textSize / 4);
        _wordsToStudy[i - numFirstLine]->_wordLabel.setTextColor(textColor);
        x = _window->getSize().x - (_wordsToStudy[i - numFirstLine]->_btn2.getSize().x + horizontalSpace * 2);
        y = _wordsToStudy[i - numFirstLine]->_rect.getPosition().y + (_wordsToStudy[i - numFirstLine]->_rect.getSize().y - _wordsToStudy[i - numFirstLine]->_btn2.getSize().y) / 2;
        _wordsToStudy[i - numFirstLine]->_btn2.setPosition(sf::Vector2i(x, y));
        x = x - _wordsToStudy[i - numFirstLine]->_btn1.getSize().x - 5;
        _wordsToStudy[i - numFirstLine]->_btn1.setPosition(sf::Vector2i(x, y));
    }
    setLearnedWordsParams();
}

void PrepareWords::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto i : _wordsToStudy)
    {
        target.draw(i->_rect);
        target.draw(i->_wordLabel);
        target.draw(i->_btn1);
        target.draw(i->_btn2);
    }
    target.draw(_okButton);
    target.draw(_wordsLearned);
    target.draw(_wordsLearnedDigit);
}

void PrepareWords::processingEvents(sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            onClick(event.mouseButton.x, event.mouseButton.y);
        }
    }
    if (event.type == sf::Event::MouseWheelScrolled)
    {
        if (event.mouseButton.button == sf::Mouse::VerticalWheel)
        {
            onMouseWheelScroll(event.mouseWheelScroll.delta);
        }
    }
}

void PrepareWords::start()
{
    _prepareWordsIsOpen = true;
    while (_window->isOpen() && _prepareWordsIsOpen)
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

void PrepareWords::onClick(float x, float y)
{
	for (int i = 0; i < _wordsToStudy.size(); ++i)
	{
        if (!_repeatMode)
        {
            if (_wordsToStudy[i]->_btn1.contains(x, y))
            {
                onClickIKnow(i);
            }
        }
        else
        {
            if (_wordsToStudy[i]->_btn1.contains(x, y))
            {
                onClickAddToRepeat(i + numFirstLine);
            }
            if (_wordsToStudy[i]->_btn2.contains(x, y))
            {
                onClickForget(i + numFirstLine);
            }
        }
	}
    if (_okButton.contains(x, y))
    {
        _user->saveDictionarys();
        if (!_repeatMode)
        {
            if (_wordsToStudy.size() == _user->getNumberOfWordsToStudy())
            {
                startExercices();
            }
        }
        else
        {
            if (_exerciseDictionary.size() > 0)
            {
                startExercices();
            }
        } 
    }
}

void PrepareWords::onClickIKnow(int wordIndex)
{
    std::deque<Word*>* userDictionary = _user->getDictionary();
    _user->getLearnedWords()->push_back(userDictionary->at(wordIndex));
    userDictionary->erase(userDictionary->begin() + wordIndex);
    initLines();
}

void PrepareWords::onClickAddToRepeat(int wordIndex)
{
    if (!_user->getLearnedWords()->at(wordIndex)->itsInRepeatList())
    {
        _exerciseDictionary.push_back(_user->getLearnedWords()->at(wordIndex));
        _user->getLearnedWords()->at(wordIndex)->setToRepeatList();
    }
    else
    {
        auto it = std::find(_exerciseDictionary.begin(), _exerciseDictionary.end(), _user->getLearnedWords()->at(wordIndex));
        _exerciseDictionary.erase(it);
        _user->getLearnedWords()->at(wordIndex)->removeFromRepeatList();
    }
    initRepeatLines();
}

void PrepareWords::onClickForget(int wordIndex)
{
    _user->getDictionary()->push_back(_user->getLearnedWords()->at(wordIndex));
    _user->getLearnedWords()->erase(_user->getLearnedWords()->begin() + wordIndex);
    initRepeatLines();
}

void PrepareWords::onMouseWheelScroll(int delta)
{
    if (_user->getLearnedWords()->size() > numShownRepeatWords)
    {
        if (numFirstLine - delta >= 0 && numFirstLine - delta <= _user->getLearnedWords()->size() - numShownRepeatWords)
        {
            numFirstLine -= delta;
            initRepeatLines();
        }
    }
}

void PrepareWords::createExerciceDictionary()
{
    if (!_repeatMode)
    {
        std::deque<Word*>* _userDictionary;
        std::deque<Word*>* _firstRepeat;
        std::deque<Word*>* _secondRepeat;
        int numWordsToStudy = _user->getNumberOfWordsToStudy();
		_userDictionary = _user->getDictionary();
		_firstRepeat = _user->getFirstRepeatDictionary();
		_secondRepeat = _user->getSecondRepeatDictionary();
		for (int i = 0; i < numWordsToStudy; ++i)
		{
			_exerciseDictionary.push_back(_userDictionary->at(i));
		}
		for (auto i : *_firstRepeat)
		{
			_exerciseDictionary.push_back(i);
		}
		for (auto i : *_secondRepeat)
		{
			_exerciseDictionary.push_back(i);
		}
    }
}

void PrepareWords::startExercices()
{
    _prepareWordsIsOpen = false;
    createExerciceDictionary();
    _exManager.setExerciseDictionary(_exerciseDictionary);
    _exManager.setRepeatMode(_repeatMode);
    _exManager.startExercises(_user, _window);
}
