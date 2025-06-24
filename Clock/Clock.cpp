#include "Clock.h"
#include "Font.h"
#include <iostream>	
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <algorithm>


	Clock::Clock(std::string& label) : timeText(FontLoading::GetFont(), label,400){
		timeText.setFont(FontLoading::GetFont());
		timeText.setCharacterSize(400);
		timeText.setFillColor(sf::Color::White);

		timeText.setString("88:88:88");
		sf::FloatRect bounds = timeText.getLocalBounds();
		timeText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));
	}

	void Clock::UpdateTime() {
		std::time_t now = std::time(nullptr);
		std::tm localTime;
		localtime_s(&localTime, &now);  

		std::ostringstream oss;
		oss << std::put_time(&localTime, "%H:%M:%S");
		timeText.setString(oss.str());
	}

	void Clock::Draw(sf::RenderWindow& window) const {
		sf::Vector2u windowSize = window.getSize();
		sf::Text centerText = timeText;
		centerText.setPosition(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f));
		window.draw(centerText);
	}







