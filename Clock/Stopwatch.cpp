#include "Font.h"
#include "Stopwatch.h"
#include <iostream>	
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <algorithm>

	Stopwatch::Stopwatch(std::string& label) : Stopwatchtext(FontLoading::GetFont(), label, 20) {
		Stopwatchtext.setFont(FontLoading::GetFont());
		Stopwatchtext.setCharacterSize(300);
		Stopwatchtext.setFillColor(sf::Color::White);

		Stopwatchtext.setString("88:88:888");
		sf::FloatRect bounds = Stopwatchtext.getLocalBounds();
		Stopwatchtext.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));
	}

	bool Stopwatch::IsRunning() const {
		return running;
	}

	void Stopwatch::Start() {
		if (!running) {
			clock.restart();
			running = true;
		}
	}
	void Stopwatch::Stop() {
		if (running) {
			elapsedTime += clock.getElapsedTime();
			running = false;
		}
	}
	void Stopwatch::Reset() {
		elapsedTime = sf::Time::Zero;
		clock.reset();
		running = false;
	}

	sf::Time Stopwatch::GetElapsedTime() const {
		if (running) {
			return elapsedTime + clock.getElapsedTime();
		}
		return elapsedTime;
	}

	void Stopwatch::UpdateDisplay() {
		sf::Time currentTime = GetElapsedTime();
		int totalMilliSeconds = static_cast<int>(currentTime.asMilliseconds());
		int minutes = totalMilliSeconds / 60000;
		int seconds = (totalMilliSeconds % 60000) / 1000;
		int milliseconds = totalMilliSeconds % 1000;

		std::ostringstream oss;
		if (minutes < 10) oss << "0";
		oss << minutes << ":";

		if (seconds < 10) oss << "0";
		oss << seconds << ".";

		if (milliseconds < 100) oss << "0";
		if (milliseconds < 10) oss << "0";
		oss << milliseconds;

		Stopwatchtext.setString(oss.str());
	}

	void Stopwatch::Draw(sf::RenderWindow& window) const {
		sf::Vector2u windowSize = window.getSize();
		sf::Text centerStopwatch = Stopwatchtext;
		centerStopwatch.setPosition(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f));
		window.draw(centerStopwatch);
	}
