#ifndef PLAYER_H
#define PLAYER_H

#include"actor.h"
#include"queue.h"
#include"stack.h"
#include"list.h"

class Point;
class Aquarium;

class Player :public Actor {
public:
	Player(Aquarium* aquarium, Point p, std::string, char sprite);

	virtual void	update(); // The only function of player to implement


	virtual void	say();

	bool			foundExit() const;
	bool			stuck() const;
	Point			getTargetPoint() const;

	void			toggleBackTrack(bool toggle);

#ifndef MAKE_MEMBERS_PUBLIC
private:
#endif

	// Wrapper function to find if a point has been
	// placed inside the m_discovered List
	bool discovered(const Point &p) const;

	// Stack to decide where to look next
	Stack<Point>	m_look;

	// List to save all the visited areas, 
	// Helps us decide where to look next
	List<Point>		m_discovered;

	// ** Variables relating to backtracking. **
	// Backtracking here is defined as "smooth" movement between
	// points.  

	// All back tracking behavior should be toggled using
	// m_toggleBackTracking, if false the player teleports
	// the way seen in the algorithm from the lecture slides.
	bool			m_toggleBackTracking;

	// Used to facilitate backtracking, without backtracking, 
	// the player teleports to  points for looking.  
	//
	// To simulate real movement we need to interpolate between
	// The players current position to the next point to look.
	//
	// The premise is that if there is a point to look around, 
	// The player must have been next to that point earlier (by
	// definition of the algorithm) so if we keep track of where
	// the player has been we can back track our steps to reach
	// the desired point.
	//
	// You may or may not need one or both.  It depends on how you 
	// plan your backtracking algorithm.  
	Stack<Point>	m_btStack;
	Queue<Point>	m_btQueue;

};

#endif//PLAYER_H
