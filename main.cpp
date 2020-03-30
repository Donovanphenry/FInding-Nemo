// DEFINE YOUR TEST
#define RUN02

#ifdef RUN01

#include<string>
using std::string;
#include"stack.h"
#include"queue.h"

int main() {

	Stack<std::string> stack;
	Queue<std::string> queue;

	stack.push("Feynman");
	stack.pop();
	stack.push("Turing");
	stack.push("Einstein");
	stack.push("Bohr");
	stack.push("Sanchez");

	stack.print();

	queue.push(stack.peek());
	stack.pop();

	queue.push(stack.peek());
	stack.pop();

	queue.push(stack.peek());
	stack.pop();

	queue.push(stack.peek());
	stack.pop();

	queue.pop();
	queue.pop();

	queue.print();

	system("pause");
	return 0;
}

#elif defined(RUN02) //Test the game


#include"game.h"

int main() {


	//Setup game
	Game g("settings.ini");

	//Exectute game loop
	g.play();

	return 0;
}
#else


#endif
