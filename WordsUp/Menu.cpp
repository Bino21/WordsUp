#include "Menu.h"
namespace
{
	const int borderSmall = 15;
	const int borderBig = 30;
	const int menuTextSize = 25;
	const int firstElemPosY = 20;
	const int btnWidth = 300;
	const int btnHeight = 50;
	const int nameEditWidth = 350;
	const int maxUsersButtons = 4;
	const int hintTextSize = 18;
    const int textSizeBtnDel = 18;
    const sf::Color btnDelColor = sf::Color(150, 30, 30);
	const sf::Color userBtnColor = sf::Color(185, 122, 87);
	const sf::Color checkedUserBtnColor = sf::Color(30, 222, 87);
	const sf::Color userBtnTextColor = sf::Color(0, 0, 0);
	const sf::Color colorBtn = sf::Color(0, 135, 0);
	const sf::Color textColorBtn = sf::Color(255, 242, 0);
	const sf::Color textColorError = sf::Color(255, 28, 11);
	const sf::Color menuTextColor = sf::Color(0, 0, 0);
	const sf::Color rectColor = sf::Color(153, 217, 234);
	const sf::Color background = sf::Color(112, 146, 190);
    const std::wstring btnDelText = L"Удалить пользователя";
	const std::wstring choiceUserText = L"Выберите пользователя";
	const std::wstring createUserText = L"Создать";
	const std::wstring createUserError = L"Это имя уже используется";
	const std::wstring dictDescription = L"Слов в вашем словаре: ";
	const std::wstring learned = L"Слов выучено: ";
	const std::wstring loadMyDict = L"Вы можете загрузить готовый словарь";
	const std::wstring loadMyDictBtnText = L"Использовать базовый словарь";
	const std::wstring loadYourDict = L"Или любой другой";
	const std::wstring loadYourDictBtnText = L"Загрузить";
	const std::wstring hintText =
		std::wstring(L"Создайте текстовый документ в простом редакторе (например в блокноте). Закидайте слова с переводом")
		+ std::wstring(L" (для примера можете посмотреть файл dictionary.txt в папке с программой). Сохраните в кодировке UTF - 8")
		+ std::wstring(L" и закиньте в папку \"dictionaries\" что в папке с программой. В поле укажите имя файла, например \"dictionary_2.txt\"");
	const std::wstring btnStartText = L"Начать";
    const std::wstring btnRepeatText = L"Повторить";
}

Menu::Menu(sf::RenderWindow* window)
{
    init(window);
}

std::vector<std::wstring> Menu::get_directories()
{
    std::vector<std::wstring> rez;
    std::wstring dir;
    try
    {
        for (auto const& p : std::filesystem::directory_iterator(fs::current_path()))
            if (p.is_directory())
            {
                dir = p.path().filename();
                rez.push_back(dir);
            }
        return rez;
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void Menu::createUserButtons()
{
    std::wstring usersDir = User::getUsersPath();
    fs::path appPath = fs::current_path();
    fs::path usersPath = fs::current_path().append(usersDir);
    if (!FileParser::folderIsExist(usersPath))
    {
        fs::create_directory(usersPath);
    }
    fs::current_path(usersPath);
    std::vector<std::wstring> users = get_directories();
    fs::current_path(appPath);
    _allUsersButtons.clear();
    if (!users.empty() && _user == nullptr)
    {
        setUser(new User(users[0]));
    }
    for (auto i : users)
    {
        Button btn;
        btn.setTextSize(menuTextSize);
        btn.setTextColor(userBtnTextColor);
        btn.setPosition(sf::Vector2i(-1000, -1000));
        btn.setString(i, Button::AutoSize::NO);
        btn.setSize(btnWidth, btnHeight);
        btn.setColor(userBtnColor);
        if (_user != nullptr && _user->getName() == i)
        {
            btn.setColor(checkedUserBtnColor);
        }
        _allUsersButtons.push_back(btn);
    }
}

void Menu::setUsersButtonsBlock()
{
    _up.setPosition(-1000, -1000);
    _down.setPosition(-1000, -1000);
    int x, y;
    if (!_allUsersButtons.empty())
    {
        _choiceUserText.setString(choiceUserText);
        _choiceUserText.setTextSize(menuTextSize);
        _choiceUserText.setTextColor(menuTextColor);
        _choiceUserText.setPosition(borderBig + _createUserRect.getSize().x / 2 - _choiceUserText.getGlobalBounds().width / 2, firstElemPosY);
        _usersBtnRect.setPosition(sf::Vector2i(borderBig, firstElemPosY + menuTextSize + borderSmall));
        _usersBtnRect.setColor(rectColor);
        int btnCount = _allUsersButtons.size();
        if (btnCount > maxUsersButtons)
            btnCount = maxUsersButtons;
        y = borderBig * 2 + btnCount * (btnHeight + borderSmall) - borderSmall;
        _usersBtnRect.setSize(_createUserRect.getSize().x, y);
    }
    else
    {
        _choiceUserText.setPosition(-1000, -1000);
        _usersBtnRect.setPosition(sf::Vector2i(-1000, -1000));
    }
    _visibleUsersButtons.clear();
    for (int i = _firstUsersButton; i < _allUsersButtons.size() && i < maxUsersButtons + _firstUsersButton; ++i)
    {
        _visibleUsersButtons.push_back(_allUsersButtons[i]);
    }
    for (int i = 0; i < _visibleUsersButtons.size(); ++i)
    {
        x = _usersBtnRect.getPosition().x + borderBig;
        y = _usersBtnRect.getPosition().y + borderBig + (i * (btnHeight + borderSmall));
        _visibleUsersButtons[i].setPosition(sf::Vector2i(x, y));
    }
    if (_allUsersButtons.size() > maxUsersButtons)
    {
        x = _usersBtnRect.getPosition().x + _usersBtnRect.getSize().x - (_up.getGlobalBounds().width + borderSmall);
        y = _usersBtnRect.getPosition().y + borderSmall;
        _up.setPosition(x, y);
        y = _usersBtnRect.getPosition().y + _usersBtnRect.getSize().y - _down.getLocalBounds().height - borderSmall;
        _down.setPosition(x, y);
    }
}

void Menu::initCreateUser()
{
    createUserButtons();
    setUsersButtonsBlock();
    if (_visibleUsersButtons.empty())
    {
        _createUserText.setString(L"Создайте пользователя");
    }
    else
        _createUserText.setString(L"Или создайте нового");

    int y;
    if (_usersBtnRect.getPosition().y < 0)
        y = 0;
    else
        y = _usersBtnRect.getPosition().y + _usersBtnRect.getSize().y;

    _createUserText.setPosition(borderBig + _createUserRect.getSize().x / 2 - _createUserText.getGlobalBounds().width / 2, y + borderBig);
    _createUserRect.setPosition(sf::Vector2i(borderBig, _createUserText.getPosition().y + menuTextSize + borderSmall));
    _nameEdit.setPosition(sf::Vector2i(_createUserRect.getPosition().x + borderBig, _createUserRect.getPosition().y + borderBig));
    _createUserButton.setPosition(sf::Vector2i(_nameEdit.getTextPosition().x + nameEditWidth + borderBig, _nameEdit.getTextPosition().y));
}

void Menu::onClickBtnCreate()
{
    std::wstring name = _nameEdit.getText();
    _nameEdit.setString(L"");
    if (name.size() > 0)
    {
        if (User::nameIsUsing(name))
        {
            _error.setString(createUserError);
            _error.setPosition(_createUserRect.getPosition().x + borderBig, _createUserRect.getPosition().y + _createUserRect.getSize().y + borderSmall);
            return;
        }
        else
            _error.setString(L"");
        setUser(new User(name));
        createUserButtons();
        for (int i = 0; i < _allUsersButtons.size(); ++i)
        {
            if (_user != nullptr && _user->getName() == _allUsersButtons[i].getString())
            {
                if (i > _firstUsersButton + maxUsersButtons - 1)
                {
                    _firstUsersButton = i - maxUsersButtons + 1;
                }
                if (i < _firstUsersButton)
                    _firstUsersButton = i;
            }
        }
        initCreateUser();
    }
}

void Menu::onClickUser(const std::wstring& name)
{
    setUser(new User(name));
    initCreateUser();
}

void Menu::onClick(float x, float y)
{
    _nameEdit.onClick(x, y);
    _loadDictName.onClick(x, y);
    for (auto i : _visibleUsersButtons)
    {
        if (i.contains(x, y))
        {
            onClickUser(i.getString());
        }
    }
    if (_createUserButton.contains(x, y))
    {
        onClickBtnCreate();
    }
    if (_up.getGlobalBounds().contains(x, y))
        onClickScrollUp();
    if (_down.getGlobalBounds().contains(x, y))
        onClickScrollDown();
    if (_buttonDel.contains(x, y))
    {
        onClickDel();
    }
    if (_loadMyDictBtn.contains(x, y))
    {
        onClickLoadDict();
        setRightBar();
    }
    if (_loadYourDictBtn.contains(x, y))
    {
        onClickLoadUserDict();
        setRightBar();
    }
    if (_help.getGlobalBounds().contains(x, y))
    {
        onClickHelp();
    }
    if (_start.contains(x, y))
        onClickStart();
    if(_repeatBtn.contains(x, y))
        onClickRepeat();
}

void Menu::onClickScrollUp()
{
    if (_firstUsersButton > 0)
        --_firstUsersButton;
    setUsersButtonsBlock();
}

void Menu::onClickScrollDown()
{
    if (_firstUsersButton < _allUsersButtons.size() - maxUsersButtons)
        ++_firstUsersButton;
    setUsersButtonsBlock();
}

void Menu::onClickLoadDict()
{
    _user->addDictionary("dictionaries\\dictionary.txt");
    _user->saveDictionarys();
}

void Menu::onClickLoadUserDict()
{
    std::string path;
    std::wstring str = _loadDictName.getText();
    std::transform(str.begin(), str.end(), std::back_inserter(path), [](wchar_t c) {
        return (char)c;
        });
    _user->addDictionary("dictionaries\\" + path);
    _user->saveDictionarys();
}

void Menu::onClickHelp()
{
    if (_hintVisible)
        _hintVisible = false;
    else
        _hintVisible = true;
}

void Menu::onClickStart()
{
    if (_user->getDictionary()->size() >= _user->getNumberOfWordsToStudy())
    {
        _menuIsOpen = false;
        _prepareWords = new PrepareWords(_user, _window);
        _prepareWords->start();
    }
}

void Menu::onClickRepeat()
{
    _menuIsOpen = false;
    _prepareWords = new PrepareWords(_user, _window, startRepeat());
    _prepareWords->start();
}

void Menu::setRightBar()
{
    int x, y;
    x = _window->getSize().x - _createUserRect.getSize().x - borderBig * 3;
    y = _window->getSize().y - borderBig - borderSmall;
    _rightBarRect.setSize(x, y);
    _rightBarRect.setColor(rectColor);
    _rightBarRect.setPosition(sf::Vector2i(-1000, -1000));
    _hint.setWidth(_rightBarRect.getSize().x - borderBig * 2);
    if (_user != nullptr)
    {
        x = _createUserRect.getSize().x + borderBig * 2;
        _rightBarRect.setPosition(sf::Vector2i(x, firstElemPosY));
        _dictWordsCount = _user->getDictionary()->size();
        _dictionaryDescription.setString(dictDescription + L" " + std::to_wstring(_dictWordsCount) + L".");
        _learnedWordsCount = _user->getLearnedWords()->size();
        _learnedWords.setString(learned + L" " + std::to_wstring(_learnedWordsCount) + L".");
        _buttonDel.setString(btnDelText);
        _loadMyDict.setString(loadMyDict);
        _loadMyDictBtn.setString(loadMyDictBtnText);
        _loadYourDict.setString(loadYourDict);
        _loadYourDictBtn.setString(loadYourDictBtnText);
        _hint.setString(hintText);
        _repeatBtn.setString(btnRepeatText);
        _repeatBtn.setSize(_repeatBtn.getSize().x, btnHeight);
        if (_learnedWordsCount > 0)
        {
            _start.setSize(_rightBarRect.getSize().x - _repeatBtn.getSize().x - borderBig * 2 - borderSmall, btnHeight);
        }
        else
        {
            _start.setSize(btnWidth, btnHeight);
        }
        _start.setString(btnStartText, Button::AutoSize::NO);
        setPositionRightBarElements(_rightBarRect.getPosition().x, _rightBarRect.getPosition().y);
    }
    else
    {
        _rightBarRect.setPosition(sf::Vector2i(-1000, -1000));
        setPositionRightBarElements(_rightBarRect.getPosition().x, _rightBarRect.getPosition().y);
    }
}

void Menu::setUser(User* user)
{
    delete _user;
    _user = user;
    _user->loadDictionarys();
    setRightBar();
}

void Menu::setPositionRightBarElements(int x, int y)
{
    _dictionaryDescription.setPosition(x + borderBig, y + borderBig);
    _learnedWords.setPosition(x + borderBig, y + borderBig + menuTextSize + borderSmall);
    _buttonDel.setPosition(sf::Vector2i(x + _rightBarRect.getSize().x - borderBig - _buttonDel.getSize().x, y + borderBig + menuTextSize + borderSmall));
    _loadMyDict.setPosition(x + borderBig, _learnedWords.getPosition().y + menuTextSize + borderBig);
    _loadMyDictBtn.setPosition(sf::Vector2i(x + (_rightBarRect.getSize().x / 2 - _loadMyDictBtn.getSize().x / 2), _loadMyDict.getPosition().y + menuTextSize + borderSmall));
    _loadYourDict.setPosition(x + borderBig, _loadMyDictBtn.getPosition().y + btnHeight + borderSmall);
    _help.setPosition(_loadYourDict.getPosition().x + _loadYourDict.getGlobalBounds().width + borderBig, _loadYourDict.getPosition().y);
    _loadDictName.setPosition(sf::Vector2i(x + borderBig, _loadYourDict.getPosition().y + menuTextSize + borderSmall));
    _loadYourDictBtn.setPosition(sf::Vector2i(x + (_rightBarRect.getSize().x / 2 - _loadYourDictBtn.getSize().x / 2), _loadDictName.getTextPosition().y + btnHeight + borderSmall));
    _hint.setPosition(x + borderBig, _loadYourDictBtn.getPosition().y + btnHeight);
    if (_learnedWordsCount > 0)
    {
        _repeatBtn.setPosition(sf::Vector2i(x + borderBig, _window->getSize().y - btnHeight - borderBig - borderSmall));
        _start.setPosition(sf::Vector2i(x + borderBig + borderSmall + _repeatBtn.getSize().x, _window->getSize().y - btnHeight - borderBig - borderSmall));
    }
    else
    {
        _repeatBtn.setPosition(sf::Vector2i(-1000, -1000));
        _start.setPosition(sf::Vector2i(x + (_rightBarRect.getSize().x / 2 - _start.getSize().x / 2), _window->getSize().y - btnHeight - borderBig - borderSmall));
    }
}

void Menu::onClickDel()
{
    _confirmWindow = new sf::RenderWindow;
    _confirmWindow->create(sf::VideoMode(700, 200), "WordsUp", sf::Style::None);
    /*int x = 0;
    int y = 0;
    int red, green, blue;*/
    Label question;
    Button yes;
    Button no;
    setConfirmWindowElements(question, yes, no);
    while (_confirmWindow->isOpen())
    {
        sf::Event event;
        while (_confirmWindow->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                _confirmWindow->close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (yes.contains(event.mouseButton.x, event.mouseButton.y))
                        onClickYes();
                    if (no.contains(event.mouseButton.x, event.mouseButton.y))
                        onClickNo();
                }
            }
           /* if (event.type == sf::Event::MouseMoved)
            {
                x = event.mouseMove.x;
                y = event.mouseMove.y;
            }*/
        }
        /*red = 50 + 255.0f / float(_confirmWindow->getSize().x) * float(x) / 2.0f;
        green = 90 + 255.0f / 2.0f - red;
        blue = 255.0f / 3.0f - 255.0f / float(_confirmWindow->getSize().y) * y / 3;
        no.setColor(sf::Color(blue, red, green));
        yes.setColor(sf::Color(blue, green, red));
        _confirmWindow->clear(sf::Color(red, green, blue));*/
        no.setColor(sf::Color(15, 78, 5));
        yes.setColor(sf::Color(134, 22, 13));
        _confirmWindow->clear(sf::Color(78, 83, 105));
        _confirmWindow->draw(question);
        _confirmWindow->draw(yes);
        _confirmWindow->draw(no);
        _confirmWindow->display();
    }
    delete _confirmWindow;
}

void Menu::deleteUser(std::wstring name)
{
    std::wstring usersDir = User::getUsersPath();
    fs::path appPath = fs::current_path();
    fs::path usersPath = fs::current_path().append(usersDir);
    fs::current_path(usersPath);
    std::vector<std::wstring> users = get_directories();
    if (std::find(users.begin(), users.end(), name) != users.end())
    {
        std::filesystem::remove_all(name);
    }
    fs::current_path(appPath);
    delete _user;
    _user = nullptr;
    _firstUsersButton = 0;
}

void Menu::onClickNo()
{
    _confirmWindow->close();
}

void Menu::setConfirmWindowElements(Label& question, Button& yes, Button& no)
{
    question.setTextSize(27);
    question.setString(L"Удалить пользователя " + _user->getName() + L"?");
    question.setTextColor(sf::Color(0, 0, 0));
    question.setPosition(_confirmWindow->getSize().x / 2 - question.getGlobalBounds().width / 2, 30);
    yes.setString(L"ДА");
    yes.setTextSize(25);
    yes.setTextColor(sf::Color(0, 0, 0));
    yes.setSize(200, 50);
    yes.setPosition(sf::Vector2i(_confirmWindow->getSize().x - 50 - yes.getSize().x, 100));
    no.setString(L"НЕТ");
    no.setTextSize(25);
    no.setTextColor(sf::Color(0, 0, 0));
    no.setSize(200, 50);
    no.setPosition(sf::Vector2i(50, 100));
}

bool Menu::startRepeat()
{
    return true;
}

void Menu::onClickYes()
{
    _confirmWindow->close();
    deleteUser(_user->getName());
    initCreateUser();
    setRightBar();
}

void Menu::init(sf::RenderWindow* window)
{
    _window = window;
    _usersBtnRect.setPosition(sf::Vector2i(-100, 0));
    _usersBtnRect.setSize(0, 0);
    _createUserText.setTextSize(menuTextSize);
    _createUserText.setTextColor(menuTextColor);
    _nameEdit.setFocus(true);
    _nameEdit.setSize(nameEditWidth, btnHeight);
    _createUserButton.setColor(colorBtn);
    _createUserButton.setTextColor(textColorBtn);
    _createUserButton.setTextSize(menuTextSize);
    _createUserButton.setString(createUserText);
    _createUserButton.setSize(_createUserButton.getSize().x, btnHeight);
    _createUserRect.setColor(rectColor);
    _error.setTextColor(textColorError);
    _error.setTextSize(menuTextSize);
    _createUserRect.setSize(nameEditWidth + borderBig * 3 + _createUserButton.getSize().x, borderBig * 2 + menuTextSize + btnHeight);
    if (!_arrowTexture.loadFromFile("arrow.png"))
    {
        std::cout << "error load file: arrow.png" << std::endl;
    }
    if (!_helpTexture.loadFromFile("help.png"))
    {
        std::cout << "error load file: help.png" << std::endl;
    }
    _helpTexture.setSmooth(true);
    _up.setTexture(_arrowTexture);
    _down.setTexture(_arrowTexture);
    _down.setOrigin(_down.getLocalBounds().width / 2, _down.getLocalBounds().height / 2);
    _down.setRotation(180);
    _down.setOrigin(_down.getLocalBounds().width, _down.getLocalBounds().height);
    _dictionaryDescription.setTextSize(menuTextSize);
    _dictionaryDescription.setTextColor(menuTextColor);
    _learnedWords.setTextSize(menuTextSize);
    _learnedWords.setTextColor(menuTextColor);
    _buttonDel.setColor(btnDelColor);
    _buttonDel.setTextColor(menuTextColor);
    _buttonDel.setTextSize(textSizeBtnDel);
    _loadMyDict.setTextSize(menuTextSize);
    _loadMyDict.setTextColor(menuTextColor);
    _loadMyDictBtn.setColor(colorBtn);
    _loadMyDictBtn.setTextColor(textColorBtn);
    _loadMyDictBtn.setTextSize(menuTextSize);
    _help.setTexture(&_helpTexture);
    _help.setRadius(menuTextSize / 1.5);
    _loadYourDict.setTextSize(menuTextSize);
    _loadYourDict.setTextColor(menuTextColor);
    _loadDictName.setSize(nameEditWidth, btnHeight);
    _loadYourDictBtn.setColor(colorBtn);
    _loadYourDictBtn.setTextColor(textColorBtn);
    _loadYourDictBtn.setTextSize(menuTextSize);
    _hint.setTextSize(hintTextSize);
    _hint.setTextColor(menuTextColor);
    _start.setColor(colorBtn);
    _start.setTextColor(textColorBtn);
    _start.setTextSize(menuTextSize);
    _repeatBtn.setColor(colorBtn);
    _repeatBtn.setTextColor(textColorBtn);
    _repeatBtn.setTextSize(menuTextSize);
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_usersBtnRect);
    target.draw(_createUserRect);
    target.draw(_choiceUserText);
    for (auto i : _visibleUsersButtons)
        target.draw(i);
    target.draw(_createUserText);
    target.draw(_nameEdit);
    target.draw(_createUserButton);
    target.draw(_error);
    target.draw(_up);
    target.draw(_down);
    target.draw(_rightBarRect);
    target.draw(_dictionaryDescription);
    target.draw(_learnedWords);
    target.draw(_buttonDel);
    target.draw(_loadMyDict);
    target.draw(_loadMyDictBtn);
    target.draw(_loadYourDict);
    target.draw(_loadDictName);
    target.draw(_loadYourDictBtn);
    target.draw(_help);
    if (_hintVisible)
    {
        target.draw(_hint);
    }
    target.draw(_repeatBtn);
    target.draw(_start);
}

void Menu::processingEvents(sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            onClick(event.mouseButton.x, event.mouseButton.y);
        }
    }
    if (event.type == sf::Event::TextEntered)
    {
        _nameEdit.onWrite(event);
        _loadDictName.onWrite(event);
    }
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Enter)
        {
            if (_nameEdit.onFocus())
                onClickBtnCreate();
            if (_loadDictName.onFocus())
                onClickLoadUserDict();
        }
    }
}

void Menu::start()
{
    _menuIsOpen = true;
    initCreateUser();
    setRightBar();
    while (_window->isOpen() && _menuIsOpen)
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
