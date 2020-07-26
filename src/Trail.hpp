#ifndef TRAIL_HPP
#define TRAIL_HPP

#include "Domain.hpp"
#include "Variable.hpp"

#include <stack>
#include <utility>
#include <iostream>

/**
 * Represents the trail of changes made. This allows backtracking to occur.
 */

class Trail
{
public:

	// Constructor
	Trail ( void );

	// Accessors
	int size ( void );
	int getPushCount ( void );
	int getUndoCount ( void );

	// Modifiers

	// Places a marker in the trail
	void placeTrailMarker ( void );

	// Pops and restores variables on the trail until the last trail marker
	void undo ( void );

	/**
	 * Before you assign a variable in constraint propagation,
	 * use this function to save its initial domain on the
	 * backtrack trail. This way if the path you are on fails,
	 * you can restore propagated domains correctly.
	 */

	void push ( Variable* v );

	// Clears the trail
	void clear ( void );

private:
	// Properties
	std::stack<std::pair<Variable*, Domain> > trailStack;
	std::stack<int> trailMarker;

	int numPush = 0;
	int numUndo = 0;
};

#endif