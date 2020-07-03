
#include"aquarium.h"
#include"player.h"
#include"shark.h"
#include"utils.h"
#include<iostream>
#include <fstream>
#include <iterator>
#include<string>
#include <chrono>
// Create the aquarium from file
Aquarium::Aquarium(std::string filename) :
	m_openCells(), m_nActors(0), m_duration(0) {

	std::fstream file(filename);

	//assert(file.is_open());
	if (!file.is_open()) {
		std::cerr << "ERROR Aquarium: could not open file " << filename << ". Exiting." << std::endl;
		exit(1);
	}
	int col = 0, row = 0;
	// Count rows and columns in file
	for (std::istreambuf_iterator<char> iter(file), e; iter != e; ++iter) {
		char c = *iter;
		if (!(c == '\n' || c == '\r'))
			col++;
		if (c == '\n') {
			row++;
		}

	}
	// columns count occured row-times, divide out
	m_cols = (int)(col / row);
	m_rows = row;

	// Allocate the aquarium 
	m_aquarium = new char*[m_cols];
	m_renderedAquarium = new char*[m_cols];
	for (int i = 0; i < m_cols; i++) {
		m_aquarium[i] = new char[m_rows];
		m_renderedAquarium[i] = new char[m_rows];
	}
	int x = 0, y = 0;
	// Reset file 
	file.clear();
	file.seekg(0, std::ios::beg);
	// Fill in the aquarium
	for (std::istreambuf_iterator<char> iter(file), e; iter != e; ++iter) {
		char c = *iter;
		if (!(c == '\n' || c == '\r')) {
			m_aquarium[(x++) % m_cols][y] = c;
		}
		if (c == '\n') {
			y++;
		}
	}
	file.close();
	// Store number of free cells
	for (int y = 0; y < m_rows; y++) {
		for (int x = 0; x < m_cols; x++) {
			if (m_aquarium[x][y] != 'X' && m_aquarium[x][y] != 'S')
				m_openCells.push_front(Point(x, y));
			if (m_aquarium[x][y] == 'S') {
				m_aquarium[x][y] = ' ';
				m_start.set(x, y);
			}
			if (m_aquarium[x][y] == 'E') {
				m_aquarium[x][y] = ' ';
				m_end.set(x, y);
			}
		}
	}

	m_player = new Player(this, m_start, "Nemo", '@');
	// 0th actor Polymorphic handle to player
	m_actors[m_nActors++] = m_player;
}

Aquarium::~Aquarium() {
	for (int i = 0; i < m_cols; i++) {
		delete[] m_renderedAquarium[i];
		delete[] m_aquarium[i];

	}
	delete[] m_aquarium;

	delete[] m_renderedAquarium;

	for (int i = 0; i < m_nActors; i++) {
		delete m_actors[i];
	}
}

void Aquarium::setPlayerBackTracking(bool toggle) {
	if (m_player != nullptr) m_player->toggleBackTrack(toggle);
}

void Aquarium::update() {

	auto t1 = std::chrono::high_resolution_clock::now();
	m_actors[0]->update();
	auto t2 = std::chrono::high_resolution_clock::now();
	m_duration += std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

	for (int i = 1; i < m_nActors; i++) {
		m_actors[i]->update();
	}

}

double Aquarium::getPlayerUpdateRunTime() {
	return m_duration;
}
void Aquarium::interact() {

	// Reset Interactions
	for (int i = 0; i < m_nActors; i++)
		m_actors[i]->setInteract(Interact::ALONE);

	// Permute over Actors
	// If actors are on top of eachother, have them interact
	for (int i = 0; i < m_nActors - 1; i++) {
		Point pos = m_actors[i]->getPosition();
		for (int j = i + 1; j < m_nActors; j++) { // j never 0
			if (pos == m_actors[j]->getPosition()) {
				// Actor 0 is nemo, any interaction between nemo and anything
				// else is an ATTACK
				if (i == 0) {
					m_actors[i]->setInteract(Interact::ATTACK);
					m_actors[j]->setInteract(Interact::ATTACK);
				}
				else {
					// If the sharks are attacking that means nemo is in the same cell
					// as multiple sharks. Attacking takes precedence over greeting
					if (m_actors[i]->getInteract() != Interact::ATTACK) {
						m_actors[i]->setInteract(Interact::GREET);
						m_actors[j]->setInteract(Interact::GREET);
					}
				}
			}
		}
	}
}
void Aquarium::draw() const {



	// Copy the clean version of the aquarium into the rendered version
	for (int y = 0; y < m_cols; y++) {
		memcpy(m_renderedAquarium[y], m_aquarium[y], m_rows * sizeof(char));
	}

	// Encode the output, go through all actors and place them in the Aquarium
	// Nemo: '@', Sharks: 'S', Multiple Shars: '2'-'9', An Attack '!'
	for (int i = 0; i < m_nActors; i++) {
		Point pos = m_actors[i]->getPosition();
		// Reference to the the character at this position, for convenience
		char& c = m_renderedAquarium[pos.getX()][pos.getY()];
		switch (c) {
		case ' ': c = m_actors[i]->draw(); break; // Empty cell, just draw
		case '@': c = '!'; break;	// ATTACK
		case 'S': c = '2'; break;	// Two sharks
		case '!':  break;			// MORE ATTACK. do nothing, already ded..
		case '9':  break;			// do nothing
		default: c++; break;		// '2' through '8', a party
		}
	}
	// Render the next point to visit for looking

	if (!(m_player->stuck() || m_player->foundExit())) {
		Point target = m_player->getTargetPoint();
		if (target.getX() >= 0 && target.getY() >= 0)
			m_renderedAquarium[target.getX()][target.getY()] = 'T';
	}


	// Draw aquarium with actors
	for (int y = 0; y < m_rows; y++) {
		for (int x = 0; x < m_cols; x++) {
			std::cout << m_renderedAquarium[x][y];
		}
		std::cout << std::endl;
	}
	std::cout << "===" << std::endl;

	// Display actors Interactions
	for (int i = 0; i < m_nActors; i++) {
		m_actors[i]->say();
		std::cout << std::endl;
	}
}

int	Aquarium::rows() const {
	return m_rows;
}
int	Aquarium::cols() const {
	return m_cols;
}
int	Aquarium::numOpenCells() const {
	return m_openCells.size();
}
Player*	Aquarium::player() const {
	return m_player;
}
Point Aquarium::getEndPoint() const {
	return m_end;
}
bool Aquarium::isCellOpen(Point p) const {
	return (m_openCells.find(p) != -1);
}
int Aquarium::addSharks() {

	// Randomly place sharks
	Point p;
	do {
		int index = randInt(0, m_openCells.size() - 1);
		p = m_openCells.get_at(index);

	} while ((p == m_player->getPosition())); // Don't start where the player starts

	std::string name = "Shark " + std::to_string((m_nActors - 1));
	m_actors[m_nActors++] = new Shark(this, p, name, 'S');
	return m_nActors;
}
