#include "Clock.h"
#include "Button.h"
#include "Font.h"
#include "Timer.h"
#include "Stopwatch.h"
#include "Alarm.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <iostream>	
#include <sstream>
#include <algorithm>

int main() {
	sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Clock Application", sf::Style::Close, sf::State::Windowed);
	window.setFramerateLimit(60);

	FontLoading::GetFont();
	if (!FontLoading::IsLoaded()) {
		std::cerr << "Failed to load font";
		return -1;
	}

	std::vector<Button> buttons;
	buttons.emplace_back("Clock", 0, 4, window.getSize());
	buttons.emplace_back("Stopwatch", 1, 4, window.getSize());
	buttons.emplace_back("Alarm", 2, 4, window.getSize());
	buttons.emplace_back("Timer", 3, 4, window.getSize());

	std::string currentScreen = "menu";

	std::string clockLabel = "";
	Clock clock(clockLabel);

	std::string StopwatchLabel = "";
	Stopwatch Stopwatch(StopwatchLabel);

	std::string timerLabel = "";
	Timer timer(timerLabel);

	std::string alarmLabel = "";
	Alarm alarm(alarmLabel);

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
					if (currentScreen != "menu") {
						currentScreen = "menu";
					}
					else {
						window.close();
					}
				}

				else if (currentScreen == "Stopwatch") {
					if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
						if (Stopwatch.IsRunning()) {
							Stopwatch.Stop();
						}
						else {
							Stopwatch.Start();
						}
					}
					else if (keyPressed->scancode == sf::Keyboard::Scancode::R) {
						Stopwatch.Reset();
					}
				}

				else if (currentScreen == "timer") {
					if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
						timer.StartTimer();
					}
					else if (keyPressed->scancode == sf::Keyboard::Scancode::Backspace) {
						if (!timerLabel.empty()) {
							timerLabel.pop_back();
							timer.SetInput(timerLabel);
						}
					}
					else if (keyPressed->scancode >= sf::Keyboard::Scancode::Num1 && keyPressed->scancode <= sf::Keyboard::Scancode::Num9) {
						int digit = static_cast<int>(keyPressed->scancode) - static_cast<int>(sf::Keyboard::Scancode::Num1) + 1;
						timerLabel += std::to_string(digit);
						timer.SetInput(timerLabel);
					}
					else if (keyPressed->scancode == sf::Keyboard::Scancode::Num0) {
						timerLabel += "0";
						timer.SetInput(timerLabel);
					}
					else if (keyPressed->scancode == sf::Keyboard::Scancode::R) {
						timer.Reset();
						timerLabel.clear();
					}
				}
				else if (currentScreen == "alarm") {
					if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
						if (!alarmLabel.empty()) {
							alarm.SetInput(alarmLabel);
						}
					}
					else if (keyPressed->scancode == sf::Keyboard::Scancode::Backspace) {
						if (!alarmLabel.empty()) {
							alarmLabel.pop_back();
						}
					}
					else if (keyPressed->scancode == sf::Keyboard::Scancode::R) {
						alarm.Reset();
						alarmLabel.clear();
					}
					else if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
						alarm.StopAlarm();
					}
					else if (keyPressed->scancode >= sf::Keyboard::Scancode::Num1 && keyPressed->scancode <= sf::Keyboard::Scancode::Num9) {
						int digit = static_cast<int>(keyPressed->scancode) - static_cast<int>(sf::Keyboard::Scancode::Num1) + 1;
						alarmLabel += std::to_string(digit);
					}
					else if (keyPressed->scancode == sf::Keyboard::Scancode::Num0) {
						alarmLabel += "0";
					}
					else if (keyPressed->scancode == sf::Keyboard::Scancode::A) {
						alarmLabel += "a";
					}
					else if (keyPressed->scancode == sf::Keyboard::Scancode::M) {
						alarmLabel += "m";
					}
					else if (keyPressed->scancode == sf::Keyboard::Scancode::P) {
						alarmLabel += "p";
					}
				}

			}
			else if (const auto* buttonPressed = event->getIf < sf::Event::MouseButtonPressed>()) {
				if (buttonPressed->button == sf::Mouse::Button::Left) {
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					for (const Button& b : buttons) {
						if (b.IsClicked(mousePos)) {
							int clickedButton = b.GetIndex();
							switch (clickedButton) {
							case 0:
								currentScreen = "clock";
								break;
							case 1:
								currentScreen = "Stopwatch";
								break;
							case 2:
								currentScreen = "alarm";
								break;
							case 3:
								currentScreen = "timer";
								break;
							}
						}
					}
				}
			}

		}


		window.clear(sf::Color::Black);


		if (currentScreen == "menu") {
			for (const Button& b : buttons) {
				b.Draw(window);
			}
		}
		else if (currentScreen == "clock") {
			clock.UpdateTime();
			clock.Draw(window);
		}
		else if (currentScreen == "Stopwatch") {
			Stopwatch.UpdateDisplay();
			Stopwatch.Draw(window);
		}
		else if (currentScreen == "timer") {
			timer.Update();
			timer.Draw(window);
		}
		else if (currentScreen == "alarm") {
			alarm.Update();
			alarm.Draw(window, alarmLabel);
		}

		window.display();
	}

	return 0;
}