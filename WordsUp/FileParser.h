#pragma once
#include <filesystem>
#include <iostream>
#include <deque>
#include <fstream>
#include "Word.h"

namespace fs = std::filesystem;

/**
* \brief Is engaged in reading from a file and writing in a file of dictionaries
*/
class FileParser
{
public:
	static void readDictionary(std::deque<Word*>* dictionary, const std::string& fileName);
	static void writeDictionary(const std::deque<Word*>* dictionary, const std::string& fileName);
	static bool folderIsExist(const fs::path& folderName);

private:
	static void cleanWord(std::wstring& str);
	static bool isLetter(wchar_t ch);
	static bool isCyrillic(wchar_t ch);
};