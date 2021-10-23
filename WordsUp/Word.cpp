#include <iostream>
#include "Word.h"

bool Word::isEngVol(const WCHAR* lang) const
{
	const wchar_t* ch = lang;
	const wchar_t* engLang = L"409";
	while (*ch != '\0')
	{
		if (*ch != *engLang)
			return false;
		++ch;
		++engLang; 
	}
	return true;
}

Word::Word(std::wstring& original, std::vector<std::wstring*>& translation)
{
	_original = original;
	_translation = translation;
}

Word::Word(const Word& rhs)
{
	_original = rhs._original;
	_translation.clear();
	for (auto i : rhs._translation)
		_translation.push_back(new std::wstring(*i));
}

Word::~Word()
{
	for (auto i : _translation)
	{
		delete i;
		i = nullptr;
	}
}

Word& Word::operator=(const Word& rhs)
{
	if (this == &rhs)
		return *this;
	_original = rhs._original;
	_translation.clear();
	for (auto i : rhs._translation)
		_translation.push_back(new std::wstring(*i));
}

std::wstring Word::getAllTranslation() const
{
	std::wstring rezult;
	for (auto it = _translation.begin(); it != _translation.end(); ++it)
	{
		rezult += *(*it);
		rezult += L", ";
	}
	if (rezult.size() > 1)
	{
		rezult.erase(rezult.end() - 2, rezult.end());
	}
	return rezult;
}

std::wstring Word::getFirstTranslation() const
{
	return *_translation[0];
}

std::wstring Word::getOriginal() const
{
	return _original;
}

bool Word::operator==(const Word& rhs) const
{
	if(_original == rhs._original)
		return true;
	return false;
}

bool Word::operator>(const Word& rhs) const
{
	if (_original > rhs._original)
		return true;
	return false;
}

bool Word::operator<(const Word& rhs) const
{
	if(*this > rhs)
		return false;
	return true;
}

void Word::setToRepeatList()
{
	_inRepeatList = true;
}

void Word::removeFromRepeatList()
{
	_inRepeatList = false;
}

bool Word::itsInRepeatList()
{
	return _inRepeatList;
}

void Word::speech(int speed) const
{
	if (FAILED(::CoInitialize(NULL)))
	{
		std::cout << "Word::speech(int speed): FAILED(::CoInitialize(NULL))" << std::endl;
		return;
	}
    HRESULT hr = S_OK;

    CComPtr<ISpVoice> cpVoice; //Will send data to ISpStream

    hr = cpVoice.CoCreateInstance(CLSID_SpVoice);
    CComPtr<ISpObjectTokenCategory> cpSpCategory = NULL;
    if (SUCCEEDED(hr = SpGetCategoryFromId(SPCAT_VOICES, &cpSpCategory)))
    {
        CComPtr<IEnumSpObjectTokens> cpSpEnumTokens;
        if (SUCCEEDED(hr = cpSpCategory->EnumTokens(NULL, NULL, &cpSpEnumTokens)))
        {
            CComPtr<ISpObjectToken> pSpTok;
            while (cpSpEnumTokens->Next(1, &pSpTok, NULL) == S_OK)
            {
                CComPtr<ISpDataKey> cpSpAttributesKey;
                if (SUCCEEDED(hr = pSpTok->OpenKey(L"Attributes", &cpSpAttributesKey)))
                {
                    CSpDynamicString dstrLang;
                    cpSpAttributesKey->GetStringValue(L"language", &dstrLang);
                    if (isEngVol(dstrLang.Copy()))
                        break;
                }
                pSpTok.Release();
            }
            hr = cpVoice->SetVoice(pSpTok);
            hr = cpVoice->SetRate(speed);
            hr = cpVoice->Speak(_original.c_str(), SPF_DEFAULT, NULL);
            pSpTok.Release();
        }
    }
}

void Word::say() const
{
	std::thread th([&]()
		{
			speech();
		});
	th.detach();
}
