#pragma once
#include <deque>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "User.h"

/**
* \brief Creates a window. Launches the menu
*/
class Application
{
public:
	Application();
	void start();
	
private:
	void createWindow();
	sf::Vector2i _resolution;
	sf::RenderWindow* _window;
	Menu* _menu;
	bool _open = true;
};


