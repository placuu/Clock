#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Rect.hpp>

class Stopwatch {
	sf::Clock clock;
	sf::Text Stopwatchtext;
	bool running = false;
	sf::Time elapsedTime = sf::Time::Zero;
public:
	Stopwatch(std::string& label);
	bool IsRunning() const;
	void Start();
	void Stop();
	void Reset();
	sf::Time GetElapsedTime() const;
	void UpdateDisplay();
	void Draw(sf::RenderWindow& window) const;
};