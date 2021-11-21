#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include "PrepareWords.h"
#include "MultiLineText.h"
#include "Button.h"
#include "RoundedRectangle.h"
#include "Label.h"
#include "TextEdit.h"
#include "User.h"

namespace fs = std::filesystem;

/**
* \brief Represents the menu.
* 
* Contains all its elements, processes events, and can display the items
*/
class Menu : public sf::Drawable
{
public:
	Menu() {}
	Menu(sf::RenderWindow* window);
	void start();

private:
	std::vector<std::wstring> get_directories();
	void setUsersButtonsBlock();
	void createUserButtons();
	void initCreateUser();
	void onClickBtnCreate();
	void onClickUser(const std::wstring& name);
	void onClick(float x, float y);
	void onClickScrollUp();
	void onClickScrollDown();
	void onClickLoadDict();
	void onClickLoadUserDict();
	void onClickHelp();
	void onClickStart();
	void onClickRepeat();
	void setRightBar();
	void setUser(User* user);
	void init(sf::RenderWindow* window);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void processingEvents(sf::Event& event);
	/// setting the elements relative to the position x, y
	void setPositionRightBarElements(int x, int y); 
	void onClickDel();
	void deleteUser(std::wstring name);
	void onClickYes();
	void onClickNo();
	void setConfirmWindowElements(Label& question, Button& yes, Button& no);
	bool startRepeat();

private:
	sf::RenderWindow* _window = nullptr;
	User* _user = nullptr;
	bool _menuIsOpen = false;
	int _firstUsersButton = 0;
	bool _hintVisible = false;
	PrepareWords* _prepareWords = nullptr;
	sf::RenderWindow* _confirmWindow;
	////////// left bar //////////////
	std::vector<Button> _allUsersButtons;
	std::vector<Button> _visibleUsersButtons;
	RoundedRectangle _usersBtnRect;
	Label _choiceUserText;
	Label _createUserText;
	TextEdit _nameEdit;
	Button _createUserButton;
	RoundedRectangle _createUserRect;
	Label _error;
	sf::Texture _arrowTexture;
	sf::Sprite _up;
	sf::Sprite _down;
	///////////// righr bar ////////////////////
	RoundedRectangle _rightBarRect;
	Label _dictionaryDescription;
	int _dictWordsCount;
	Label _learnedWords;
	int _learnedWordsCount;
	Button _buttonDel;
	Label _loadMyDict;
	Button _loadMyDictBtn;
	Label _loadYourDict;
	TextEdit _loadDictName;
	Button _loadYourDictBtn;
	sf::Texture _helpTexture;
	sf::CircleShape _help;
	MultiLineText _hint;
	Button _start;
	Button _repeatBtn;
	////////////////////////////////////////////
};