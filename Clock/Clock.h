#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Rect.hpp>

class Clock {
	sf::Text timeText;
public:
	Clock(std::string& label);
	void UpdateTime();
	void Draw(sf::RenderWindow& window) const;
};



