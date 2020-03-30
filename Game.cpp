#include"game.h"
#include"aquarium.h"
#include"player.h"
#include"utils.h"
#include <cstdlib>
#include<iostream>
#include <fstream>
#include <sstream>
#include<algorithm>


Game::Game(std::string settingsFile) :m_maxSteps(0), m_automate(false), m_showSteps(false) {
	std::string fsettings = "settings.ini";
	std::ifstream file(fsettings);

	if (!file.is_open()) {
		std::cerr << "ERROR Main: could not open file " << fsettings << ". Exiting." << std::endl;
		std::cin.ignore(); exit(0);
	}
	//Grab settings from file
	std::string line, setting, value;

	std::string mazeFile = "maze_lecture.txt";

	bool havePlayerBackTack = true; //Toggle player backtracking

	int frameTimeDelay = 500;		//Effects game speed, shorter delay
									//less time between frames, faster 
									//simulation when automating.
	int numSharks = 0;

	while (std::getline(file, line)) {
		line.erase(std::remove_if(line.begin(), line.end(),
			[](char c) { return c == ' ' || c == '\n' || c == '\r' || c == '\t'; }), line.end());
		if (line.size() == 0) continue;
		int delpos = line.find('=');
		setting = line.substr(0, delpos);
		value = line.substr(delpos + 1);

		if (setting == "mazeFile") {
			std::ifstream tf(value);

			if (!tf.is_open()) {
				std::cerr << "ERROR Main: could not open file " << value << ". Exiting." << std::endl;
				std::cin.ignore(); exit(0);
			}
			tf.close();

			mazeFile = value;
		}
		else if (setting == "havePlayerBackTack") {
			if (value != "true" && value != "false") {
				std::cerr << "ERROR Main: havePlayerBackTack setting incorrect format\n\tExpected: true/false, given: ." << value << ".\n\tExiting." << std::endl;;
				std::cin.ignore(); exit(1);
			}

			std::istringstream istr(value);
			istr >> std::boolalpha >> havePlayerBackTack;
		}
		else if (setting == "frameTimeDelay") {
			frameTimeDelay = std::stoi(value);
		}
		else if (setting == "numSharks") {
			numSharks = std::stoi(value);
		}
		else {
			std::cerr << "ERROR Main: unknown setting " << setting << " " << value << ". Exiting." << std::endl;
			std::cin.ignore(); exit(1);
		}
	}
	file.close();

	m_delay = frameTimeDelay;

	m_aquarium = new Aquarium(mazeFile);

	m_aquarium->setPlayerBackTracking(havePlayerBackTack);

	// Arbitrarily set maximum number of sharks the half the number of open cells
	int maxSharks = m_aquarium->numOpenCells() / 2;
	if (numSharks > maxSharks) {
		std::cerr << "ERROR GAME: too many sharks: " << numSharks
			<< " for game size: " << maxSharks << ". Exiting." << std::endl;
		exit(1);
	}

	for (int i = 0; i < numSharks; i++) {
		m_aquarium->addSharks();
	}

}
Game::Game(std::string filename, int numSharks, int delay, bool toggleBackTracking)
	:m_maxSteps(0), m_automate(false), m_delay(delay) {

	m_aquarium = new Aquarium(filename);

	m_aquarium->setPlayerBackTracking(toggleBackTracking);
	// Arbitrarily set maximum number of sharks the half the number of open cells
	int maxSharks = m_aquarium->numOpenCells() / 2;
	if (numSharks > maxSharks) {
		std::cerr << "ERROR GAME: too many sharks: " << numSharks
			<< " for game size: " << maxSharks << ". Exiting." << std::endl;
		exit(1);
	}

	for (int i = 0; i < numSharks; i++) {
		m_aquarium->addSharks();
	}

}

Game::~Game() {
	delete m_aquarium;
	//std::cerr << "~Game" << std::endl;
}

void Game::play() {
	// Game loop
	while (true) {

		// Clear the canvas to draw new scene
		clearScreen();

		m_maxSteps++;
		if (m_showSteps)
			std::cout << "Step: " << m_maxSteps
			<< ", Total Player::Update Runtime: " << m_aquarium->getPlayerUpdateRunTime()
			<< ", Runtime/Step: " << m_aquarium->getPlayerUpdateRunTime() / m_maxSteps << std::endl;

		m_aquarium->draw();


		if (m_aquarium->player()->stuck()) {
			std::cout << "Got stuck with no way out :( " << std::endl;
			std::cout << "Press enter to continue.";
			std::cin.ignore(10000, '\n');
			return;
		}

		if (m_aquarium->player()->foundExit()) {
			std::cout << "You've reached the end! Congratulations! " << std::endl;
			std::cout << "Press enter to continue.";
			std::cin.ignore(10000, '\n');
			return;
		}



		if (!m_automate) {
			std::cout << "Command (<space> to step, <a> to automate, <q> to quit): ";
			std::string action;
			std::getline(std::cin, action);
			switch (action[0]) {
			default:
				std::cout << '\a' << std::endl;  // beep
				continue;
			case 'q':
				std::cout << "Quitting Game." << std::endl;
				return;
			case ' ':
				break;
			case 'a':
				m_automate = true;
				break;
			case 's':
				m_showSteps = !m_showSteps;
				break;
			}
		}
		else {
			if (m_maxSteps >= MAX_STEPS) {
				std::cout << "Reached max steps, quitting." << std::endl;
				return;
			}
			delay(m_delay);
		}

		m_aquarium->update();
		m_aquarium->interact();

	}
}
