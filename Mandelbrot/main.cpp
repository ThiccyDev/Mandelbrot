#include <iostream>
#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"
using namespace std;
using namespace sf;

int main()
{
	VideoMode vm(500, 500);
	RenderWindow window(vm, "Mandelbrot", Style::Default);

	ComplexPlane complexPlane(vm.width, vm.height);

	Font font;
	font.loadFromFile("alagard.ttf");

	Text text;
	text.setCharacterSize(15);
	text.setFont(font);

	while (window.isOpen())
	{
		/*
		****************************************
		Handle the players input
		****************************************
		*/
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				// Quit the game when the window is closed
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					complexPlane.zoomIn();
					complexPlane.setCenter({ event.mouseButton.x, event.mouseButton.y });
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					complexPlane.zoomOut();
					complexPlane.setCenter({ event.mouseButton.x, event.mouseButton.y });
				}
			}
			if (event.type == sf::Event::MouseMoved)
			{
				complexPlane.setMouseLocation(Mouse::getPosition());
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		/*
		****************************************
		Update
		****************************************
		*/
		complexPlane.updateRenderer();
		complexPlane.loadText(text);
		/*
		****************************************
		Draw
		****************************************
		*/
		window.clear();
		window.draw(complexPlane);
		window.draw(text);
		window.display();
	}

	return 0;
}