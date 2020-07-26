#include "Trail.hpp"

using namespace std;

/**
 * Represents the trail of changes made. This allows backtracking to occur.
 */

// =====================================================================
// Constructor
// =====================================================================

Trail::Trail ( void )
{
}

// =====================================================================
// Accessors
// =====================================================================

int Trail::size ( void )
{
	return trailStack.size();
}

int Trail::getPushCount ( void )
{
	return numPush;
}

int Trail::getUndoCount ( void )
{
	return numUndo;
}

// =====================================================================
// Modifiers
// =====================================================================

// Places a marker in the trail
void Trail::placeTrailMarker ( void )
{
	trailMarker.push( trailStack.size() );
}

/**
 * Before you assign a variable in constraint propagation,
 * use this function to save its initial domain on the
 * backtrack trail. This way if the path you are on fails,
 * you can restore propagated domains correctly.
 */

void Trail::push ( Variable* v )
{
	numPush++;
	trailStack.push( pair<Variable*, Domain>( v, v->getDomain() ) );
}

// Pops and restores variables on the trail until the last trail marker
void Trail::undo ( void )
{
	numUndo++;
	int targetSize = trailMarker.top();
	trailMarker.pop();
	for ( int size = trailStack.size(); size > targetSize; --size )
	{
		pair<Variable*, Domain> vPair = trailStack.top();
		trailStack.pop();
		vPair.first->setDomain(vPair.second);
		vPair.first->setModified( false );
		vPair.first->unassign();
	}
}

// Clears the trail
void Trail::clear ( void )
{
	while ( ! trailStack.empty() )
		trailStack.pop();

	while ( ! trailMarker.empty() )
		trailMarker.pop();
}
