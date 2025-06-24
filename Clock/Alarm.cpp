#include "Font.h"
#include "Alarm.h"
#include <iostream>	
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <algorithm>


	Alarm::Alarm(std::string& label) : alarmText(FontLoading::GetFont(), label, 20), instructionText(FontLoading::GetFont(), label, 20),
		alarmHour(0), alarmMinute(0), isAM(true), alarmSet(false), isRinging(false), waitingForAMPM(false) {

		alarmText.setFillColor(sf::Color::White);
		alarmText.setCharacterSize(200);
		alarmText.setString("00:00 AM");
		sf::FloatRect bounds = alarmText.getLocalBounds();
		alarmText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));

		instructionText.setCharacterSize(40);
		instructionText.setFillColor(sf::Color::Yellow);


		if (!soundBuffer.loadFromFile("alarm.wav")) {
			std::cout << "Error loading alarm sound\n";
		}
		alarmSound = new sf::Sound(soundBuffer);
		alarmSound->setLooping(true);
	}

	std::string Alarm::GetCurrentTime() {
		auto now = std::time(0);
		struct tm ltm;
		localtime_s(&ltm, &now);

		int hour = ltm.tm_hour;
		int minute = ltm.tm_min;

		bool currentIsAM = hour < 12;
		if (hour == 0) {
			hour = 12;
		}
		else if (hour > 12) {
			hour -= 12;
		}

		std::stringstream ss;
		ss << std::setfill('0') << std::setw(2) << hour << ":"
			<< std::setfill('0') << std::setw(2) << minute << " "
			<< (currentIsAM ? "AM" : "PM");

		return ss.str();
	}

	void Alarm::ParseTimeInput(const std::string& input) {
		if (input.length() < 3 || input.length() > 4) return;

		int timeValue = std::stoi(input);
		int hours, minutes;

		if (input.length() == 3) {
			hours = timeValue / 100;
			minutes = timeValue % 100;
		}
		else {
			hours = timeValue / 100;
			minutes = timeValue % 100;
		}

		if (minutes >= 60) {
			return;
		}

		if (hours < 1 || hours > 12) {
			return;
		}


		alarmHour = hours;
		alarmMinute = minutes;
		waitingForAMPM = true;
	}

	bool Alarm::CheckAlarm() {
		auto now = std::time(0);
		struct tm ltm;
		localtime_s(&ltm, &now);

		int currentHour = ltm.tm_hour;
		int currentMinute = ltm.tm_min;
		bool currentIsAM = currentHour < 12;

		int displayHour = currentHour;
		if (displayHour == 0) displayHour = 12;
		else if (displayHour > 12) displayHour -= 12;

		return (displayHour == alarmHour &&
			currentMinute == alarmMinute &&
			currentIsAM == isAM);
	}

	void Alarm::HandleInput(const std::string& input) {
		if (waitingForAMPM) {
			if (input.length() >= 2) {
				std::string lastTwo = input.substr(input.length() - 2);
				std::transform(lastTwo.begin(), lastTwo.end(), lastTwo.begin(), ::tolower);
				if (lastTwo == "am") {
					isAM = true;
					alarmSet = true;
					waitingForAMPM = false;
					inputTime.clear();
				}
				else if (lastTwo == "pm") {
					isAM = false;
					alarmSet = true;
					waitingForAMPM = false;
					inputTime.clear();
				}
			}
		}
		else if (!alarmSet && !waitingForAMPM) {
			std::string upperInput = input;
			std::transform(upperInput.begin(), upperInput.end(), upperInput.begin(), ::toupper);

			if (upperInput.find("AM") != std::string::npos || upperInput.find("PM") != std::string::npos) {
				std::string numericPart;
				for (char c : input) {
					if (std::isdigit(c)) {
						numericPart += c;
					}
				}

				if (numericPart.length() >= 3 && numericPart.length() <= 4) {
					int timeValue = std::stoi(numericPart);
					int hours, minutes;

					if (numericPart.length() == 3) {
						hours = timeValue / 100;
						minutes = timeValue % 100;
					}
					else {
						hours = timeValue / 100;
						minutes = timeValue % 100;
					}

					if (minutes >= 60 || hours < 1 || hours > 12) {
						return;
					}

					alarmHour = hours;
					alarmMinute = minutes;

					if (upperInput.find("AM") != std::string::npos) {
						isAM = true;
					}
					else if (upperInput.find("PM") != std::string::npos) {
						isAM = false;
					}

					alarmSet = true;
					waitingForAMPM = false;
					inputTime.clear();
				}
			}
			else {
				ParseTimeInput(input);
			}
		}
	}

	void Alarm::ProcessInput() {
		if (!inputTime.empty()) {
			HandleInput(inputTime);
		}
	}

	void Alarm::SetInput(const std::string input) {
		inputTime = input;
		if (waitingForAMPM) {
			HandleInput(input);
		}
		else {
			HandleInput(input);
		}
	}

	void Alarm::Update() {
		if (alarmSet && !isRinging && CheckAlarm()) {
			isRinging = true;
			alarmSound->play();
		}
	}

	void Alarm::StopAlarm() {
		if (isRinging) {
			alarmSound->stop();
			isRinging = false;
			alarmSet = false;
		}
	}

	void Alarm::Reset() {
		alarmSound->stop();
		isRinging = false;
		alarmSet = false;
		waitingForAMPM = false;
		inputTime.clear();
		alarmHour = 0;
		alarmMinute = 0;
		isAM = true;
	}

	void Alarm::Draw(sf::RenderWindow& window, const std::string& inputLabel) const {
		sf::Vector2u windowSize = window.getSize();

		std::string currentTimeStr = const_cast<Alarm*>(this)->GetCurrentTime();
		sf::Text currentTimeText = alarmText;
		currentTimeText.setString(currentTimeStr);
		currentTimeText.setPosition(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f));
		window.draw(currentTimeText);

		std::string instructions;
		if (waitingForAMPM) {
			instructions = "Enter 'am' or 'pm' and press Enter: " + inputTime;
		}
		else if (isRinging) {
			instructions = "*** ALARM RINGING! *** Press R to Stop";
		}
		else if (!alarmSet) {
			instructions = "Enter time (730 = 7:30, 1245 = 12:45): " + inputLabel;
		}
		else {
			instructions = "Alarm is set. Press R to Reset";
		}

		sf::Text instructionCenter = instructionText;
		instructionCenter.setString(instructions);
		sf::FloatRect instrBounds = instructionCenter.getLocalBounds();
		instructionCenter.setOrigin(sf::Vector2f(instrBounds.position.x + instrBounds.size.x / 2.f, instrBounds.position.y + instrBounds.size.y / 2.f));
		instructionCenter.setPosition(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f + 250));

		if (isRinging) {
			instructionCenter.setFillColor(sf::Color::Red);
		}
		else {
			instructionCenter.setFillColor(sf::Color::Yellow);
		}

		window.draw(instructionCenter);

		if (alarmSet) {
			std::string alarmTimeStr = std::to_string(alarmHour) + ":" +
				(alarmMinute < 10 ? "0" : "") + std::to_string(alarmMinute) +
				(isAM ? " AM" : " PM");

			sf::Text alarmSetText(FontLoading::GetFont(), alarmTimeStr, 60);
			alarmSetText.setFillColor(sf::Color::Green);
			sf::FloatRect alarmBounds = alarmSetText.getLocalBounds();
			alarmSetText.setOrigin(sf::Vector2f(alarmBounds.position.x + alarmBounds.size.x / 2.f, alarmBounds.position.y + alarmBounds.size.y / 2.f));
			alarmSetText.setPosition(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f + 150));
			window.draw(alarmSetText);
		}
	}
	Alarm::~Alarm() {
		if (alarmSound) {
			alarmSound->stop();
			delete alarmSound;
			alarmSound = nullptr;
		}
	}