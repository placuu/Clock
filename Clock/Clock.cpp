#include <iostream>	
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <algorithm>

class FontLoading {
	static sf::Font font;
	static bool fontLoaded;
public:
	static const sf::Font& GetFont() {
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

	static bool IsLoaded() {
		return fontLoaded;
	}
};

sf::Font FontLoading::font;
bool FontLoading::fontLoaded = false;

class Button {
	sf::RectangleShape shape;
	sf::Text text;
	int index;

public:
	Button(const std::string& label,int index, int totalButtons, sf::Vector2u windowSize):text(FontLoading::GetFont(), label, 20), index(index) {


		const float buttonWidth = 300.f;
		const float buttonHeight = 150.f;
		const float spacing = 90.f;
		const float marginBottom = 40.f;

		float totalWidth = totalButtons * buttonWidth + (totalButtons - 1) * spacing;
		float StartX = (windowSize.x - totalWidth) / 2.f;
		float x = StartX + index * (buttonWidth + spacing);
		float y = windowSize.y - buttonHeight - marginBottom;

		shape.setSize(sf::Vector2f(buttonWidth, buttonHeight));
		shape.setPosition(sf::Vector2f(x,y));
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

	void Draw(sf::RenderWindow& window) const {
		window.draw(shape);
		window.draw(text);
	}
	bool IsClicked(const sf::Vector2i& mousePosition) const {
		sf::FloatRect bounds = shape.getGlobalBounds();
		
		if (bounds.contains(static_cast<sf::Vector2f>(mousePosition))) {
			return true;
		}
		return false;
	}

	int GetIndex() const {
		return index;
	}
};

class Clock{
	sf::Text timeText;
public:
	Clock(std::string& label) : timeText(FontLoading::GetFont(), label,400){
		timeText.setFont(FontLoading::GetFont());
		timeText.setCharacterSize(400);
		timeText.setFillColor(sf::Color::White);

		timeText.setString("88:88:88");
		sf::FloatRect bounds = timeText.getLocalBounds();
		timeText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));
	}

	void UpdateTime() {
		std::time_t now = std::time(nullptr);
		std::tm localTime;
		localtime_s(&localTime, &now);  

		std::ostringstream oss;
		oss << std::put_time(&localTime, "%H:%M:%S");
		timeText.setString(oss.str());
	}

	void Draw(sf::RenderWindow& window) const {
		sf::Vector2u windowSize = window.getSize();
		sf::Text centerText = timeText;
		centerText.setPosition(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f));
		window.draw(centerText);
	}
};

class Stopwatch {
	sf::Clock clock;
	bool running = false;
	sf::Text Stopwatchtext;
	sf::Time elapsedTime = sf::Time::Zero;
public:
	Stopwatch(std::string& label) : Stopwatchtext(FontLoading::GetFont(), label, 20) {
		Stopwatchtext.setFont(FontLoading::GetFont());
		Stopwatchtext.setCharacterSize(300);
		Stopwatchtext.setFillColor(sf::Color::White);
	
		Stopwatchtext.setString("88:88:888");
		sf::FloatRect bounds = Stopwatchtext.getLocalBounds();
		Stopwatchtext.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));
	}

	bool IsRunning() const {
		return running;
	}

	void Start(){
		if (!running) {
			clock.restart();
			running = true;
		}
	}
	void Stop() {
		if(running) {
			elapsedTime += clock.getElapsedTime();
			running = false;
		}
	}
	void Reset(){
		elapsedTime = sf::Time::Zero;
		clock.reset();
		running = false;
	}

	sf::Time GetElapsedTime() const {
		if (running) {
			return elapsedTime + clock.getElapsedTime();
		}
		return elapsedTime;
	}
	
	void UpdateDisplay() {
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

	void Draw(sf::RenderWindow& window) const {
		sf::Vector2u windowSize = window.getSize();
		sf::Text centerStopwatch = Stopwatchtext;
		centerStopwatch.setPosition(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f));
		window.draw(centerStopwatch);
	}

};

class Timer {
	sf::Text timerText;
	sf::Text inputText;

	sf::Clock clock;
	float totalTime;
	float remainingTime;

	sf::SoundBuffer soundBuffer;
	sf::Sound timerSound{soundBuffer};
	bool soundPlayed;

	std::string inputDigits;
	bool IsRunning;
	bool isFinished;
public:
	Timer(std::string& label) :timerText(FontLoading::GetFont(), label, 20),inputText(FontLoading::GetFont(),label,20), IsRunning(false), isFinished(false),totalTime(0),remainingTime(0),soundPlayed(false) {
		timerText.setCharacterSize(300);
		timerText.setFillColor(sf::Color::White);
		timerText.setString("00:00:00");
		sf::FloatRect bounds = timerText.getLocalBounds();
		timerText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));

		inputText.setCharacterSize(50);
		inputText.setFillColor(sf::Color::Yellow);
		inputText.setString("Enter time (SS, MMSS, or HHMMSS): ");

		if (!soundBuffer.loadFromFile("alarm.wav")) {
			std::cout << "Error loading alarm sound\n";
		}
		timerSound.setBuffer(soundBuffer);
		timerSound.setLooping(true);
	}

	void Start(float input) {
		totalTime = input;
		remainingTime = totalTime;
		IsRunning = true;
		clock.restart();
	}

	void Update() {
		if (IsRunning && remainingTime > 0) {
			float elapsed = clock.restart().asSeconds();
			remainingTime -= elapsed;

			if (remainingTime <= 0) {
				remainingTime = 0;
				IsRunning = false;
				isFinished = true;

				if (!soundPlayed) {
					timerSound.play();
					soundPlayed = true;
				}
			}
		}
		int totalSeconds = static_cast<int>(remainingTime);
		int hours = totalSeconds / 3600;
		int minutes = (totalSeconds%3600) / 60;
		int seconds = totalSeconds % 60;
		std::string timeString =
			(hours < 10 ? "0" : "") + std::to_string(hours) + ":" +
			(minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" +
			(seconds < 10 ? "0" : "") + std::to_string(seconds);

		timerText.setString(timeString);
	}

	void Draw(sf::RenderWindow& window) const{
		sf::Vector2u windowSize = window.getSize();

		sf::Text timerCenter = timerText;
		sf::FloatRect timerBounds = timerCenter.getLocalBounds();
		timerCenter.setOrigin(sf::Vector2f(timerBounds.position.x + timerBounds.size.x / 2.f, timerBounds.position.y + timerBounds.size.y / 2.f));
		timerCenter.setPosition(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f));
		window.draw(timerCenter);

		if (!IsRunning && !isFinished) {
			sf::Text inputCenter = inputText;
			sf::FloatRect inputBounds = inputCenter.getLocalBounds();
			inputCenter.setOrigin(sf::Vector2f(inputBounds.position.x + inputBounds.size.x / 2.f, inputBounds.position.y + inputBounds.size.y / 2.f));
			inputCenter.setPosition(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f + 200));
			window.draw(inputCenter);
		}

		if (isFinished) {
			sf::Text finishedText(FontLoading::GetFont(),"",20);
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

	void SetInput(const std::string& input) {
		std::string instruction = "Enter time (SS, MMSS, or HHMMSS): ";
		inputText.setString(instruction + input);
		inputDigits = input;
	}

	bool IsTimerFinished() const{
		return isFinished;
	}

	void StartTimer() {
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
					minutes = (inputNumber / 100)%100;
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

	void Reset() {
		IsRunning = false;
		isFinished = false;
		remainingTime = 0;
		totalTime = 0;
		soundPlayed = false;
		inputDigits.clear();
		timerSound.stop();
		inputText.setString("Enter time (SS, MMSS, or HHMMSS): ");
	}
};

class Alarm {
	sf::Text alarmText;
	sf::Text instructionText;
	sf::SoundBuffer soundBuffer;
	sf::Sound alarmSound{ soundBuffer };

	std::string inputTime;
	int alarmHour;
	int alarmMinute;
	bool isAM;
	bool alarmSet;
	bool isRinging;
	bool waitingForAMPM;
public:
	Alarm(std::string& label) : alarmText(FontLoading::GetFont(), label, 20), instructionText(FontLoading::GetFont(), label, 20),
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
		alarmSound.setBuffer(soundBuffer);
		alarmSound.setLooping(true);
	}

	std::string getCurrentTime() {
		auto now = std::time(0);
		struct tm ltm;
		localtime_s(&ltm,&now);

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

	void parseTimeInput(const std::string& input) {
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

	bool checkAlarm() {
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
	
	void handleInput(const std::string& input) {
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
				parseTimeInput(input);
			}
		}
	}

	void ProcessInput() {
		if (!inputTime.empty()) {
			handleInput(inputTime);
		}
	}

	void SetInput(const std::string input) {
		inputTime = input;
		if (waitingForAMPM) {
			handleInput(input);
		}
		else {
			handleInput(input);
		}
	}

	void Update() {
		if (alarmSet && !isRinging && checkAlarm()) {
			isRinging = true;
			alarmSound.play();
		}
	}

	void StopAlarm() {
		if (isRinging) {
			alarmSound.stop();
			isRinging = false;
			alarmSet = false;
		}
	}

	void Reset() {
		alarmSound.stop();
		isRinging = false;
		alarmSet = false;
		waitingForAMPM = false;
		inputTime.clear();
		alarmHour = 0;
		alarmMinute = 0;
		isAM = true;
	}

	void Draw(sf::RenderWindow& window, const std::string& inputLabel) const {
		sf::Vector2u windowSize = window.getSize();

		std::string currentTimeStr = const_cast<Alarm*>(this)->getCurrentTime();
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
};

int main() {
	sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Clock Application", sf::Style::Close, sf::State::Windowed);
	window.setFramerateLimit(60);

	FontLoading::GetFont();
	if (!FontLoading::IsLoaded) {
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

				else if(currentScreen == "timer"){
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



