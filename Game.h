#ifndef GAME_H
#define GAME_H

#include<string>

class Aquarium;

const int MAX_STEPS = 100000;

class Game {
public:
	// Dynamically allocates an aquarium and adds sharks
	Game(std::string settingsFile);
	Game(std::string filename, int numSharks, int delay, bool toggleBackTracking);
	~Game();

	//Game loop
	void play();
private:
	Aquarium * m_aquarium;
	bool		m_automate;
	bool		m_showSteps;
	int			m_maxSteps;
	int			m_delay;

};

#endif//GAME_H
