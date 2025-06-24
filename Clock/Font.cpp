#include "Font.h"
#include <iostream>	
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <algorithm>

sf::Font FontLoading::font;
bool FontLoading::fontLoaded = false;

const sf::Font& FontLoading::GetFont() {
	if (!fontLoaded) {
		if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
				std::cerr << "Error loading font\n";
		}
		else {
			fontLoaded = true;
		}
	}
	return font;
}

bool FontLoading::IsLoaded() {
	return fontLoaded;
}


