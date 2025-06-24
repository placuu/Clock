#include "Button.h"
#include "Font.h"
#include <iostream>	
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <algorithm>


Button::Button(const std::string& label, int index, int totalButtons, sf::Vector2u windowSize) :text(FontLoading::GetFont(), label, 20), index(index) {


	const float buttonWidth = 300.f;
	const float buttonHeight = 150.f;
	const float spacing = 90.f;
	const float marginBottom = 40.f;

	float totalWidth = totalButtons * buttonWidth + (totalButtons - 1) * spacing;
	float StartX = (windowSize.x - totalWidth) / 2.f;
	float x = StartX + index * (buttonWidth + spacing);
	float y = windowSize.y - buttonHeight - marginBottom;

	shape.setSize(sf::Vector2f(buttonWidth, buttonHeight));
	shape.setPosition(sf::Vector2f(x, y));
	shape.setFillColor(sf::Color(70, 70, 70));
	shape.setOutlineThickness(2);
	shape.setOutlineColor(sf::Color(100, 100, 100));

	text.setFont(FontLoading::GetFont());
	text.setString(label);
	text.setCharacterSize(40);
	text.setFillColor(sf::Color::White);

	sf::FloatRect bounds = text.getLocalBounds();
	sf::Vector2f center = bounds.getCenter();

	text.setOrigin(center);

	sf::Vector2f shapeCenter = shape.getPosition() + shape.getSize() / 2.f;
	text.setPosition(shapeCenter);
}

void Button::Draw(sf::RenderWindow& window) const {
	window.draw(shape);
	window.draw(text);
}
bool Button::IsClicked(const sf::Vector2i& mousePosition) const {
	sf::FloatRect bounds = shape.getGlobalBounds();

	if (bounds.contains(static_cast<sf::Vector2f>(mousePosition))) {
		return true;
	}
	return false;
}

int Button::GetIndex() const {
	return index;
}
