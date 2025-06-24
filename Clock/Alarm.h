#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Rect.hpp>

class Alarm {
	sf::Text alarmText;
	sf::Text instructionText;
	sf::SoundBuffer soundBuffer;
	sf::Sound* alarmSound = nullptr;
	std::string inputTime;

	int alarmHour;
	int alarmMinute;
	bool isAM;
	bool alarmSet;
	bool isRinging;
	bool waitingForAMPM;
public:
	Alarm(std::string& label);
	~Alarm();
	std::string GetCurrentTime();
	void ParseTimeInput(const std::string& input);
	bool CheckAlarm();
	void HandleInput(const std::string& input);
	void ProcessInput();
	void SetInput(const std::string input);
	void Update();
	void StopAlarm();
	void Reset();
	void Draw(sf::RenderWindow& window, const std::string& inputLabel) const;
};