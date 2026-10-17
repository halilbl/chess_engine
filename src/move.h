#pragma once

class Move {
public:
	Move() : from(0), to(0){} //without a default constructor and a constructor, default constructor is not defined by the compiler it is deleted.

	Move(int f, int t) : 
	from(f),
	to(t) {}

	int from;
	int to;
};
