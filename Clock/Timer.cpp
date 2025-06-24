#include "Font.h"
#include "Timer.h"
#include <iostream>	
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <algorithm>


	Timer::Timer(std::string& label) :timerText(FontLoading::GetFont(), label, 20), inputText(FontLoading::GetFont(), label, 20), IsRunning(false), isFinished(false), totalTime(0), remainingTime(0), soundPlayed(false){
		timerText.setCharacterSize(300);
		timerText.setFillColor(sf::Color::White);
		timerText.setString("88:88:88");
		sf::FloatRect bounds = timerText.getLocalBounds();
		fixedOrigin = sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f);
		timerText.setOrigin(fixedOrigin);

		inputText.setCharacterSize(50);
		inputText.setFillColor(sf::Color::Yellow);
		inputText.setString("Enter time (SS, MMSS, or HHMMSS): ");

		if (!soundBuffer.loadFromFile("alarm.wav")) {
			std::cout << "Error loading alarm sound\n";
		}
		timerSound = new sf::Sound(soundBuffer);
		timerSound->setLooping(true);
	}

	void Timer::Start(float input) {
		totalTime = input;
		remainingTime = totalTime;
		IsRunning = true;
		clock.restart();
	}

	void Timer::Update() {
		if (IsRunning && remainingTime > 0) {
			float elapsed = clock.restart().asSeconds();
			remainingTime -= elapsed;

			if (remainingTime <= 0) {
				remainingTime = 0;
				IsRunning = false;
				isFinished = true;

				if (!soundPlayed) {
					timerSound->play();
					soundPlayed = true;
				}
			}
		}
		int totalSeconds = static_cast<int>(remainingTime);
		int hours = totalSeconds / 3600;
		int minutes = (totalSeconds % 3600) / 60;
		int seconds = totalSeconds % 60;
		std::string timeString =
			(hours < 10 ? "0" : "") + std::to_string(hours) + ":" +
			(minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" +
			(seconds < 10 ? "0" : "") + std::to_string(seconds);

		timerText.setString(timeString);
	}

	void Timer::Draw(sf::RenderWindow& window) {
		sf::Vector2u windowSize = window.getSize();

		/*sf::Text timerCenter = timerText;
		sf::FloatRect timerBounds = timerCenter.getLocalBounds();
		timerCenter.setOrigin(sf::Vector2f(timerBounds.position.x + timerBounds.size.x / 2.f, timerBounds.position.y + timerBounds.size.y / 2.f));*/
		timerText.setPosition(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f));
		window.draw(timerText);

		if (!IsRunning && !isFinished) {
			sf::Text inputCenter = inputText;
			sf::FloatRect inputBounds = inputCenter.getLocalBounds();
			inputCenter.setOrigin(sf::Vector2f(inputBounds.position.x + inputBounds.size.x / 2.f, inputBounds.position.y + inputBounds.size.y / 2.f));
			inputCenter.setPosition(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f + 200));
			window.draw(inputCenter);
		}

		if (isFinished) {
			sf::Text finishedText(FontLoading::GetFont(), "", 20);
			finishedText.setFont(FontLoading::GetFont());
			finishedText.setString("TIME'S UP!");
			finishedText.setCharacterSize(100);
			finishedText.setFillColor(sf::Color::Red);
			sf::FloatRect finishedBounds = finishedText.getLocalBounds();
			finishedText.setOrigin(sf::Vector2f(finishedBounds.position.x + finishedBounds.size.x / 2.f, finishedBounds.position.y + finishedBounds.size.y / 2.f));
			finishedText.setPosition(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f - 200));
			window.draw(finishedText);
		}
	}

	void Timer::SetInput(const std::string& input) {
		std::string instruction = "Enter time (SS, MMSS, or HHMMSS): ";
		inputText.setString(instruction + input);
		inputDigits = input;
	}

	bool Timer::IsTimerFinished() const {
		return isFinished;
	}

void Timer::StartTimer() {
	if (!inputDigits.empty()) {
		try {
			int inputNumber = std::stoi(inputDigits);
			int totalSeconds = 0;
			int hours = 0, minutes = 0, seconds = 0;

			if (inputDigits.length() <= 2) {
				seconds = inputNumber;

			}
			else if (inputDigits.length() <= 4) {
				minutes = inputNumber;

				seconds = inputNumber % 100;
				minutes = inputNumber / 100;
			}
			else {
				seconds = inputNumber % 100;
				minutes = (inputNumber / 100) % 100;
				hours = inputNumber / 10000;
			}

			totalSeconds = hours * 3600 + minutes * 60 + seconds;
			Start(static_cast<float>(totalSeconds));
			inputDigits.clear();
		}
		catch (const std::exception& e) {
			std::cout << "Error parsing time: " << e.what() << std::endl;
		}
	}
}

void Timer::Reset() {
	IsRunning = false;
	isFinished = false;
	remainingTime = 0;
	totalTime = 0;
	soundPlayed = false;
	inputDigits.clear();
	timerSound->stop();
	inputText.setString("Enter time (SS, MMSS, or HHMMSS): ");

}

Timer::~Timer() {
	if (timerSound) {
		delete timerSound;
		timerSound = nullptr;
	}
}
