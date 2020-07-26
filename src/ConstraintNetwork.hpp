#ifndef CONSTRAINTNETWORK_HPP
#define CONSTRAINTNETWORK_HPP

#include "Domain.hpp"
#include "Variable.hpp"
#include "Constraint.hpp"
#include "SudokuBoard.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <map>
#include <utility>

class ConstraintNetwork
{
public:
	// Typedefs
	typedef typename std::vector< Constraint* > ConstraintRefSet;
	typedef typename std::vector< Constraint > ConstraintSet;
	typedef typename std::vector< Variable* > VariableSet;


	// Constructors
	ConstraintNetwork ( void );
	ConstraintNetwork ( SudokuBoard sboard );

	// Modifiers
	void add ( Constraint c );
	void add ( Variable* v );

	// Accessors
	ConstraintSet getConstraints ( void ) const;
	VariableSet   getVariables   ( void ) const;

	VariableSet getNeighborsOfVariable ( Variable* v );

	bool isConsistent ( void ) const;

	ConstraintRefSet getConstraintsContainingVariable ( Variable* v );
	ConstraintRefSet getModifiedConstraints ( void );

	SudokuBoard toSudokuBoard ( int _p, int _q ) const;
	std::string toString ( void ) const;

private:
	ConstraintSet constraints;
	VariableSet variables;
};

#endif
