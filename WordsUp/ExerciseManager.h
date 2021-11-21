#pragma once
#include <SFML/Graphics.hpp>
#include "User.h"
#include "CollectWords.h"
#include "FindTranslation.h"
#include "WriteAnswer.h"


/**
* \brief Decides which word with which exercise to display on the screen
*/
class ExerciseManager : public sf::Drawable
{
public:
	ExerciseManager();
	void startExercises(User* user, sf::RenderWindow* window);
	void setExerciseDictionary(std::deque<Word*>& exDict);
	void setRepeatMode(bool repeatMode);

private:
	enum class Mode;
	void setExercise();
	void setUser(User* user);
	void setWindow(sf::RenderWindow* window);
	void init(User* user, sf::RenderWindow* window);
	void createExerciseProgressLine();
	void onClickNext(float x, float y);
	void nextRandomWord();
	void prepareToNextExecise();
	void setNextMode();
	void processingEvents(sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool exercisesOver() const;
	bool exercisesStarted() const;

private:
	struct Answers
	{
		int _wordID;   // номер слова в списке
		int _answers;  // количество верных ответов
		Answers(int word, int answers);
	};
	enum class Mode { EN_RU, RU_EN, COLLECT, WRITE };
	Mode _mode;
	std::vector<Answers> _exerciseProgress;
	CollectWords _collect;
	FindTranslation _findTrans;
	WriteAnswer _writeAns;
	Button _buttonNext;
	User* _user;
	sf::RenderWindow* _window;
	std::deque<Word*> _exerciseDictionary;
	int _wordId;
	Word* _pastWord = nullptr;
	bool _isStarted = false;
	bool _repeatMode;
	/// temporary dictionary - the sum of user and dictionary exercises(for correct display of translation options)
	std::deque<Word*> _tempDictionary;
	std::deque<Word*>* _userDictionary = nullptr;
	std::deque<Word*>* _firstRepeat = nullptr;
	std::deque<Word*>* _secondRepeat = nullptr;
};

