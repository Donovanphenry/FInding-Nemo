#include"player.h"
#include"actor.h"
#include"stack.h"
#include"list.h"
#include"point.h"
#include"aquarium.h"
#include<iostream>

//  Player( ... )
//		Constructs and initializes the Player/Actor and its member variables
//		Remembers and discovers the starting point.
Player::Player(Aquarium* aquarium, Point p, std::string name, char sprite)
	:Actor(aquarium, p, name, sprite),
	m_look(),
	m_discovered(),
	m_btQueue(),
	m_btStack(),
	m_toggleBackTracking(false) {

	// Discover the starting point
	m_discovered.push_front(p);
	m_look.push(p);
}

//  stuck() 
//		See if the player is stuck in the maze (no solution)
bool Player::stuck() const {
	return Actor::getState() == State::STUCK;
}

//  foundExit()
//		See if the player has found the exit
bool Player::foundExit() const {
	return Actor::getState() == State::FREEDOM;
}
//  toggleBackTrack( ... )
//		Turn on/off backtracking
void Player::toggleBackTrack(bool toggle) {
	m_toggleBackTracking = toggle;
}

//  getTargetPoint()
//		Get the point the player wants to look around next.
//      If m_look is empty then return an invalid point.
Point Player::getTargetPoint() const {
	if (m_look.empty()) return Point(-1, -1);
	return m_look.peek();
}

// discovered
//		returns true if the item is in the list
bool Player::discovered(const Point &p) const {
	return (m_discovered.find(p) != -1);
}



// say()
//		What does the player say?
//		Already Implemented, nothing to do here
void Player::say() {

	// Freedom supercedes being eaten
	if (getState() == State::FREEDOM) {
		std::cout << getName() << ": WEEEEEEEEE!";
		return;
	}

	// Being eaten supercedes being lost
	switch (getInteract()) {
	case Interact::ATTACK:
		std::cout << getName() << ": OUCH!";
		break;
	case Interact::GREET:
		break;
	case Interact::ALONE:
	default:
		switch (getState()) {
		case State::LOOKING:
			std::cout << getName() << ": Where is the exit?";
			break;
		case State::STUCK:
			std::cout << getName() << ": Oh no! I am Trapped!";
			break;
		case State::BACKTRACK:
			std::cout << getName() << ": Got to backtrack...";
			break;
		default:
			break;
		}

		break;
	}
}


//  update() 
//		This function implements an algorithm to look through the maze
//      for places to move (STATE::LOOKING). update also handles moving the 
//		player as well, if there is an open undiscovered cell to move to the  
//		player just moves there.  However, there will be cases in the next 
//		desired point to look around is not adjacent (more than one cell away), 
//		at this point the player must backtrack (State::BACKTRACK) to a point 
//		that is adjacent to the next point the player was planning to visit
//
//		The player can only do one thing each call, they can either LOOK or 
//		BACKTRACK not both.  Nor should the player move more than one cell per
//		call to update.  If you examine the function calls for the entire game
//		you should observe that this is already be called within a loop. Your
//		Implementation should NOT have any loops that pertain to actual movement 
//		of the player.  
//
//		Backtracking is challenging, save it for the very very very last thing.
//		Make sure the STATE::LOOKING aspect compiles and works first.
void Player::update() 
{
	int index = 0;
	Point currentPoint, south, east, north, west, update;

	if (getState() == State::LOOKING)
	{
		m_btStack.push(getPosition());
		update = m_look.peek();
		setPosition(update);
		m_look.pop();
		currentPoint = getPosition();

		if (this->getPosition() == this->getAquarium()->getEndPoint())
		{
			this->setState(State::FREEDOM);
			return;
		}

		south.set(currentPoint.getX(), currentPoint.getY() + 1);
		east.set(currentPoint.getX() + 1, currentPoint.getY());
		north.set(currentPoint.getX(), currentPoint.getY() - 1);
		west.set(currentPoint.getX() - 1, currentPoint.getY());
	
		if (!discovered(west) && this->getAquarium()->isCellOpen(west))
		{
			update = west;
			m_look.push(west);
			m_discovered.push_front(west);
		}

		if (!discovered(east) && this->getAquarium()->isCellOpen(east))
		{
			update = east;
			m_look.push(east);
			m_discovered.push_front(east);
		}

		if (!discovered(north) && this->getAquarium()->isCellOpen(north))
		{
			update = north;
			m_look.push(north);
			m_discovered.push_front(north);
		}

		if (!discovered(south) && this->getAquarium()->isCellOpen(south))
		{
			update = south;
			m_look.push(south);
			m_discovered.push_front(south);
		}

		
		if (m_look.peek() != north && m_look.peek() != south && m_look.peek() != west && east != m_look.peek())
		{
			setState(State::BACKTRACK);
			return;
		}

		/*update = m_look.peek();
		index = m_discovered.find(update);
		m_discovered.pop_at(index);
		m_discovered.push_front(update);*/
	}

	
	else 
	{
		currentPoint = getPosition();
		south.set(currentPoint.getX(), currentPoint.getY() + 1);
		east.set(currentPoint.getX() + 1, currentPoint.getY());
		north.set(currentPoint.getX(), currentPoint.getY() - 1);
		west.set(currentPoint.getX() - 1, currentPoint.getY());

		if (m_btStack.empty())
		{
			if (north != m_look.peek() && m_look.peek() != west && m_look.peek() != east && m_look.peek() != south)
			{
				setState(State::STUCK);
				return;
			}
			else
			{
				if (south == m_look.peek())
					update = south;
				if (north == m_look.peek())
					update = north;
				if (east == m_look.peek())
					update = east;
				if (west == m_look.peek())
					update = west;

				m_btStack.push(update);
				m_discovered.push_front(update);
				m_look.push(update);
				setState(State::LOOKING);
				return;
			}
		}

		if (south == m_look.peek() || north == m_look.peek() || east == m_look.peek() || west == m_look.peek())
		{
			if (south == m_look.peek())
				update = south;
			if (north == m_look.peek())
				update = north;
			if (east == m_look.peek())
				update = east;
			if (west == m_look.peek())
				update = west;

			m_discovered.push_front(update);
			//m_look.push(update);
			setState(State::LOOKING);
			setPosition(update);

			//while (!m_btStack.empty())
				//m_btStack.pop();
		
			//m_btStack.print();
			return;
		}

		update = m_btStack.peek();
		m_btStack.pop();
		setPosition(update);
		
		/*cout << (m_discovered.front() == west);
		if (south == m_look.peek() || north == m_look.peek() || east == m_look.peek() || west == m_look.peek())
		{
			if (south == m_look.peek())
				update = south;
			if (north == m_look.peek())
				update = north;
			if (east == m_look.peek())
				update = east;
			if (west == m_look.peek())
				update = west;

			m_discovered.push_front(update);
			setPosition(update);
			m_look.pop();
			setState(State::LOOKING);
			return;
		}

		if (m_discovered.front() == south)
			update = south;

		if (m_discovered.front() == north)
			update = north;

		if (m_discovered.front() == east)
			update = east;

		if (m_discovered.front() == west)
			update = west;

		m_discovered.push_back(update);
		m_discovered.pop_front();
		this->setPosition(update);*/
	}


	// Set by the settings file and checked here
	// if(m_toggleBackTracking) { ... code relating to backtracking 
}
