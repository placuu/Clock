#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Rect.hpp>

class FontLoading {
	static sf::Font font;
	static bool fontLoaded;
public:
	static const sf::Font& GetFont();
	static bool IsLoaded();
};