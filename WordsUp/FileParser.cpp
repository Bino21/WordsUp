#include "FileParser.h"


void FileParser::cleanWord(std::wstring& str)
{
	while (str.length() > 0 && !isLetter(str.at(0)))
	{
		str.erase(0, 1);
	}
	while (str.length() > 0 && !isLetter(str.at(str.length() - 1)))
	{
		str.pop_back();
	}
}

bool FileParser::isLetter(wchar_t ch)
{
	if ((int(ch) >= 65 && int(ch) <= 90) || (int(ch) >= 97 && int(ch) <= 122) || (int(ch) >= 1040 && int(ch) <= 1103)
		|| int(ch) == 39 || int(ch) == 1025 || int(ch) == 1105 || int(ch) == 1110 || int(ch) == 1111 || int(ch) == 1030
		|| int(ch) == 1031 || int(ch) == 1108 || int(ch) == 1028)
		return true;
	else
		return false;
}

bool FileParser::isCyrillic(wchar_t ch)
{
	/// если символ кирилица, пробел, Ё, ё, І, і, Ї, ї, ', є, Є 
	if ((int(ch) >= 1040 && int(ch) <= 1103) || int(ch) == 32 || int(ch) == 1025 || int(ch) == 1105 || int(ch) == 1110
		|| int(ch) == 1111 || int(ch) == 1030 || int(ch) == 1031 || int(ch) == 39 || int(ch) == 1108 || int(ch) == 1028)
		return true;
	return false;
}

void FileParser::readDictionary(std::deque<Word*>* dictionary, const std::string& fileName)
{
	std::fstream file;
	std::string line;
	std::wstring wline;
	std::wstring original;
	std::wstring translation;
	bool isEnglish = true;
	file.open(fileName);
	if (!file.is_open())
	{
		//std::cout << fileName << " open file error!" << std::endl;
	}
	else
	{
		while (!file.eof())
		{
			std::getline(file, line, '\n');
			if (line.compare(0, 3, "\xEF\xBB\xBF") == 0)  // Is the file marked as UTF-8?
			{
				line.erase(0, 3);                  // Now get rid of the BOM.
			}
			wline = sf::String::fromUtf8(line.begin(), line.end()).toWideString();
			for (auto wchar : wline)
			{
				if ((int(wchar) >= 65 && int(wchar) <= 90) || (int(wchar) >= 97 && int(wchar) <= 122))
					isEnglish = true;
				if (int(wchar) >= 1040 && int(wchar) <= 1103)
					isEnglish = false;
				if (isEnglish)
					original.push_back(wchar);
				else
					translation.push_back(wchar);
			}
			cleanWord(original);
			cleanWord(translation);
			std::vector<std::wstring*> translationVariants;
			if (translation.length() > 0)     /// розбиваем строку перевода на отдельные слова (варианты перевода)
			{
				std::wstring str;
				for (auto it = translation.begin(); it != translation.end(); ++it)
				{
					/// если символ кирилица, пробел, Ё, ё, І, і, Ї, ї, ', є, Є  - записываем его в стоку
					if (isCyrillic(*it))
					{
						str.push_back(*it);
					}
					else           /// иначе - значит вариант перевода закончился
					{
						cleanWord(str);
						translationVariants.push_back(new std::wstring(str));
						str.erase();
					}
					if ((it == translation.end() - 1) && isCyrillic(*it))
					{
						cleanWord(str);
						translationVariants.push_back(new std::wstring(str));
						str.erase();
					}
				}
			}
			if (original.length() > 0 && translationVariants.size() > 0)
			{
				dictionary->push_back(new Word(original, translationVariants));
			}
			original.erase();
			translation.erase();
		}
		file.close();
	}
}

void FileParser::writeDictionary(const std::deque<Word*>* dictionary, const std::string& fileName)
{
//#pragma warning(disable : 4996)
	FILE* fout = fopen(fileName.c_str(), "w,ccs=UTF-8");
	if (!fout)
	{
		std::cout << "write dictionary: open file error";
	}
	else
	{
		for (auto i : *dictionary)
		{
			std::wstring str(i->getOriginal() + L" - " + i->getAllTranslation() + L"\n");
			fwrite(str.c_str(), wcslen(str.c_str()) * sizeof(wchar_t), 1, fout);
		}
		fclose(fout);
	}
}

bool FileParser::folderIsExist(const fs::path& folderName)
{
	fs::file_status status = fs::file_status{};
	if (fs::status_known(status) ? fs::exists(status) : fs::exists(folderName))
		return true;
	else
		return false;
}