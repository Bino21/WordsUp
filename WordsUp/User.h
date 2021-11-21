#pragma once
#include <deque>
#include <vector>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cwctype>
#include <filesystem>
#include "CollectWords.h"
#include "FindTranslation.h"
#include "WriteAnswer.h"
#include "Word.h"
#include "FileParser.h"

/**
* \brief Represents the user. Contains his name and dictionaries
*/
class User
{
public:
	User(const std::wstring& name);
	~User();
	/// add new dictionary to users dictionary. Adds only words that are not yet in users dictionaryies
	void addDictionary(const std::string& fileName); 
	std::deque<Word*>* getDictionary();
	const std::deque<Word*>* getDictionary() const;
	std::deque<Word*>* getFirstRepeatDictionary();
	std::deque<Word*>* getSecondRepeatDictionary();
	std::deque<Word*>* getLearnedWords();
	const std::deque<Word*>* getLearnedWords() const;
	int getNumberOfWordsToStudy() const;
	int getRepeatCount() const;
	int getFirstRepeatCount() const;
	int getSecondRepeatCount() const;
	std::wstring getName() const;
	static bool nameIsUsing(const std::wstring& userName);
	static std::wstring getUsersPath();
	void saveDictionarys();
	/// loads previously saved dictionaries
	void loadDictionarys();
	void changeDictionarysAfterExercises(const std::deque<Word*>* exercisesDictionary);

private:
	/// determines whether such a word is in the dictionary. Words that differ in case or a / an are considered the same 
	bool itWordIsCopyFromDictionary(const Word* word, const std::deque<Word*>* dict);
	std::wstring toLower(const std::wstring& str) const;

private:
	std::deque<Word*> _dictionary;
	std::deque<Word*> _firstRepeatDictionary;
	std::deque<Word*> _secondRepeatDictionary;
	std::deque<Word*> _learnedWords;
	std::wstring _name;	
};

