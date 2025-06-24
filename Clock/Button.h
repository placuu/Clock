#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Rect.hpp>

class Button {
	sf::RectangleShape shape;
	sf::Text text;
	int index;
public:
	Button(const std::string& label, int index, int totalButtons, sf::Vector2u windowSize);
	void Draw(sf::RenderWindow& window) const;
	bool IsClicked(const sf::Vector2i& mousePosition) const;
	int GetIndex() const;
};