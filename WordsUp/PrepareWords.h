#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "User.h"
#include "Button.h"
#include "RoundedRectangle.h"
#include "Label.h"
#include "TextEdit.h"
#include "ExerciseManager.h"

/**
* \brief Represents the window with a choice of words to study.
*
* Contains all its elements, processes events, and can display the items
*/
class PrepareWords : public sf::Drawable
{
public:
	PrepareWords(User* user, sf::RenderWindow* window, bool repeatMode = false);
	~PrepareWords();
	void start();

private:
	struct lineInPrepareWords;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void processingEvents(sf::Event& event);
	void init(User* user, sf::RenderWindow* window);
	void initLines();
	void initRepeatLines();
	void lineInit(lineInPrepareWords* line, Word* word);
	void setLearnedWordsParams();
	void onClick(float x, float y);
	void onClickIKnow(int lineIndex);
	void onClickAddToRepeat(int lineIndex);
	void onClickForget(int lineIndex);
	void onMouseWheelScroll(int delta);
	void createExerciceDictionary();
	void startExercices();

private:
	struct lineInPrepareWords
	{
		RoundedRectangle _rect;
		Label _wordLabel;
		Button _btn1;
		Button _btn2;
	};
	std::vector<lineInPrepareWords*> _wordsToStudy;
	Button _okButton;
	User* _user;
	sf::RenderWindow* _window;
	bool _prepareWordsIsOpen;
	bool _repeatMode;
	Label _wordsLearned;
	Label _wordsLearnedDigit;
	ExerciseManager _exManager;
	std::deque<Word*> _exerciseDictionary;
	int numFirstLine = 0;
};