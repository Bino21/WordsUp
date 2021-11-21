#include "Application.h"

namespace
{
    const std::wstring labelBtnNext = L"Дальше >";
    const int textSizeBtnNext = 30;
    const sf::Vector2i sizeBtnNext = sf::Vector2i(270, 55);
    sf::Vector2i positionBtnNext = sf::Vector2i(800, 525);
    const sf::Color colorBtnNext = sf::Color(0, 135, 0);
    const sf::Color textColorBtnNext = sf::Color(255, 242, 0);
    const sf::Color background = sf::Color(112, 146, 190);
    const int windowHeight = 620;
    const int windowMaxWidth = 1200;
}

Application::Application()
{
    _resolution.y = windowHeight;
    _resolution.x = sf::VideoMode::getDesktopMode().width;
    if (_resolution.x > windowMaxWidth)
        _resolution.x = windowMaxWidth;
    _window = nullptr;
}

void Application::start()
{
    createWindow();
    while (_window->isOpen())
    {
        sf::Event event;
        while (_window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                _window->close();
            }
        }
        _menu = new Menu(_window);
        _menu->start();
        delete _menu;
    }
}

void Application::createWindow()
{
    _window = new sf::RenderWindow();
    _window->create(sf::VideoMode(_resolution.x, _resolution.y), "WordsUp", sf::Style::Close | sf::Style::Titlebar);
    _window->setFramerateLimit(60);
}