#ifndef CONSTRAINT_HPP
#define CONSTRAINT_HPP

#include "Domain.hpp"
#include "Variable.hpp"

#include <vector>
#include <sstream>
#include <algorithm>

/**
 * Constraint represents a NotEquals constraint on a set of variables.
 * Used to ensure none of the variables contained in the constraint have the same assignment.
 */

class Constraint
{
public:
	// Typedefs
	typedef typename std::vector< Variable* > VariableSet;

	// Properties
	VariableSet vars;

	// Constructors
	Constraint ( void );
	Constraint ( VariableSet vars );

	// Modifiers
	void addVariable ( Variable* v );

	// Accessors
	int  size         ( void );
	bool contains     ( Variable* v );
	bool isModified   ( void );
	bool isConsistent ( void );

	bool operator== ( const Constraint &other ) const;

	// String representation
	std::string toString ( void );
};

#endif
