#pragma once
#include <string>
#include <vector>
#include <thread>
#include <SFML/Graphics.hpp>
#pragma warning(disable:4996)
#include <sphelper.h>
#pragma warning(default:4996)

/**
* The class contains the original word and variants of its translation
*/
class Word
{
public:
	Word() {}
	Word(std::wstring& original, std::vector<std::wstring*>& translation);
	Word(const Word& rhs);
	~Word();
	Word& operator=(const Word& rhs);
	std::wstring getAllTranslation() const;
	std::wstring getFirstTranslation() const;
	std::wstring getOriginal() const;
	bool operator==(const Word& rhs) const;
	bool operator>(const Word& rhs) const;
	bool operator<(const Word& rhs) const;
	void setToRepeatList();
	void removeFromRepeatList();
	bool itsInRepeatList();
	/// voice acting of the word in English
	void say() const;

private:
	bool isEngVol(const WCHAR* lang) const;
	void speech(int speed = 0) const;

private:
	std::wstring _original;
	std::vector<std::wstring*> _translation;
	bool _inRepeatList = false;
};