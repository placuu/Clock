#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Rect.hpp>

class Timer {
	sf::Text timerText;
	sf::Text inputText;
	sf::Clock clock;

	float totalTime;
	float remainingTime;

	sf::Vector2f fixedOrigin;
	sf::SoundBuffer soundBuffer;
	sf::Sound* timerSound = nullptr;
	std::string inputDigits;

	bool soundPlayed = false;
	bool IsRunning = false;
	bool isFinished = false;
public:
	Timer(std::string& label);
	~Timer();
	void Start(float input);
	void Update();
	void Draw(sf::RenderWindow& window);
	void SetInput(const std::string& input);
	bool IsTimerFinished() const;
	void StartTimer();
	void Reset();
};

