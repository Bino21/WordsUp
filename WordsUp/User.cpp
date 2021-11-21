#include "User.h"

namespace
{
	const int repeat = 3;
	const int firstRepeat = 2;
	const int secondRepeat = 1;
	const std::wstring usersPath = L"Users";
	int numberOfWordsToStudy = 5;
	int wordNumber = 0;
}

namespace fs = std::filesystem;

bool User::itWordIsCopyFromDictionary(const Word* word, const std::deque<Word*>* dict)
{
	std::wstring lhs, rhs;
	for (auto i : *dict)
	{
		lhs = toLower(word->getOriginal());
		rhs = toLower(i->getOriginal());
		if (lhs.size() > 2 && rhs.size() > 2)
		{
			if (int(lhs[0]) == 97 && int(lhs[1]) == 32)
				lhs.erase(0, 2);
			else if (int(lhs[0]) == 97 && int(lhs[1]) == 110 && int(lhs[2]) == 32)
				lhs.erase(0, 3);

			if (int(rhs[0]) == 97 && int(rhs[1]) == 32)
				rhs.erase(0, 2);
			else if (int(rhs[0]) == 97 && int(rhs[1]) == 110 && int(rhs[2]) == 32)
				rhs.erase(0, 3);

			if (lhs == rhs)
				return true;
		}
	}
	return false;
}

std::wstring User::toLower(const std::wstring& str) const
{
	std::wstring rezult;
	for (auto ch : str)
	{
		if (int(ch) >= 97 && int(ch) <= 122 || (int(ch) == 39) || (int(ch) == 32))
			rezult.push_back(ch);
		else if ((int(ch) >= 65 && int(ch) <= 90))
			rezult.push_back(ch + 32);
	}
	return rezult;
}

User::User(const std::wstring& name)
{
    _name = name;
	std::wstring usersDir = User::getUsersPath();
	fs::path appPath = fs::current_path();
	fs::path usersPath = fs::current_path().append(usersDir);
	fs::path userName = fs::current_path().append(usersDir).append(name);
	try
	{
		if (!FileParser::folderIsExist(usersPath))
			fs::create_directory(usersPath);
		fs::current_path(usersPath);
		if (!FileParser::folderIsExist(userName))
			fs::create_directory(userName);
		fs::current_path(appPath);
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}

User::~User()
{
    while (!_dictionary.empty())
    {
        delete _dictionary.back();
        _dictionary.pop_back();
    }
	while (!_firstRepeatDictionary.empty())
	{
		delete _firstRepeatDictionary.back();
		_firstRepeatDictionary.pop_back();
	}
	while (!_secondRepeatDictionary.empty())
	{
		delete _secondRepeatDictionary.back();
		_secondRepeatDictionary.pop_back();
	}
	while (!_learnedWords.empty())
	{
		delete _learnedWords.back();
		_learnedWords.pop_back();
	}
}

void User::addDictionary(const std::string& fileName)
{
	std::deque<Word*> allDictionarys;
	allDictionarys.insert(allDictionarys.end(), _dictionary.begin(), _dictionary.end());
	allDictionarys.insert(allDictionarys.end(), _firstRepeatDictionary.begin(), _firstRepeatDictionary.end());
	allDictionarys.insert(allDictionarys.end(), _secondRepeatDictionary.begin(), _secondRepeatDictionary.end());
	allDictionarys.insert(allDictionarys.end(), _learnedWords.begin(), _learnedWords.end());
	if (allDictionarys.empty())
	{
		FileParser::readDictionary(&_dictionary, fileName);
	}
	else
	{
		std::deque<Word*> loadedDictionary;
		FileParser::readDictionary(&loadedDictionary, fileName);
		for (auto it = loadedDictionary.begin(); it != loadedDictionary.end(); ++it)
		{
			if (itWordIsCopyFromDictionary(*it, &allDictionarys))
			{
				delete* it;
			}
			else
				_dictionary.push_back(*it);
		}
		loadedDictionary.clear();
	}
	int seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(_dictionary.begin(), _dictionary.end(), std::default_random_engine(seed));
}

void User::loadDictionarys()
{
	fs::path appPath = fs::current_path();
	fs::path usersPath = fs::current_path().append(getUsersPath());
	fs::path userNamePath = fs::current_path().append(getUsersPath()).append(_name);

	if (!FileParser::folderIsExist(userNamePath))
	{
		fs::create_directory(userNamePath);
	}
	fs::current_path(userNamePath);
	FileParser::readDictionary(&_dictionary, "dictionary.txt");
	FileParser::readDictionary(&_firstRepeatDictionary, "firstRepeatDictionary.txt");
	FileParser::readDictionary(&_secondRepeatDictionary, "secondRepeatDictionary.txt");
	FileParser::readDictionary(&_learnedWords, "learnedWords.txt");
	fs::current_path(appPath);
}


std::deque<Word*>* User::getDictionary()
{
    return &_dictionary;
}

const std::deque<Word*>* User::getDictionary() const
{
	return &_dictionary;
}

std::deque<Word*>* User::getFirstRepeatDictionary()
{
	return &_firstRepeatDictionary;
}

std::deque<Word*>* User::getSecondRepeatDictionary()
{
	return &_secondRepeatDictionary;
}

std::deque<Word*>* User::getLearnedWords()
{
	return &_learnedWords;
}

const std::deque<Word*>* User::getLearnedWords() const
{
	return &_learnedWords;
}

int User::getNumberOfWordsToStudy() const
{
    return numberOfWordsToStudy;
}


int User::getRepeatCount() const
{
    return repeat;
}

int User::getFirstRepeatCount() const
{
	return firstRepeat;
}

int User::getSecondRepeatCount() const
{
	return secondRepeat;
}

std::wstring User::getName() const
{
    return _name;
}

bool User::nameIsUsing(const std::wstring& userName)
{
	std::wstring path = usersPath + L"\\" + userName;
    if (FileParser::folderIsExist(path))
        return true;
    else
        return false;
}

std::wstring User::getUsersPath()
{
	return usersPath;
}

void User::saveDictionarys()
{
	fs::path appPath = fs::current_path();
	fs::path usersPath = fs::current_path().append(getUsersPath());
	fs::path userNamePath = fs::current_path().append(getUsersPath()).append(_name);

	if (!FileParser::folderIsExist(userNamePath))
	{
		fs::create_directory(userNamePath);
	}
	fs::current_path(userNamePath);
	FileParser::writeDictionary(&_dictionary, "dictionary.txt");
	FileParser::writeDictionary(&_firstRepeatDictionary, "firstRepeatDictionary.txt");
	FileParser::writeDictionary(&_secondRepeatDictionary, "secondRepeatDictionary.txt");
	FileParser::writeDictionary(&_learnedWords, "learnedWords.txt");
	fs::current_path(appPath);
	
}

void User::changeDictionarysAfterExercises(const std::deque<Word*>* exercisesDictionary)
{
	for (auto i : _secondRepeatDictionary)
	{
		_learnedWords.push_back(i);
	}
	_secondRepeatDictionary.erase(_secondRepeatDictionary.begin(), _secondRepeatDictionary.end());
	for (auto i : _firstRepeatDictionary)
	{
		_secondRepeatDictionary.push_back(i);
	}
	_firstRepeatDictionary.erase(_firstRepeatDictionary.begin(), _firstRepeatDictionary.end());
	for (auto i : *exercisesDictionary)
	{
		auto rez = std::find(_dictionary.begin(), _dictionary.end(), i);
		if (rez != _dictionary.end())
		{
			_firstRepeatDictionary.push_back(*rez);
			_dictionary.erase(rez);
		}
	}
}
