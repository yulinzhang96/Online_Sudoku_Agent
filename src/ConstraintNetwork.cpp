#include "ConstraintNetwork.hpp"

/**
 * CSP representation of the problem. Contains the variables, constraints, and
 * many helpful accessors.
 */

// =====================================================================
// Constructors
// =====================================================================

ConstraintNetwork::ConstraintNetwork ( void )
{

}

ConstraintNetwork::ConstraintNetwork ( SudokuBoard sboard )
{
	SudokuBoard::Board board = sboard.get_board();

	VariableSet variables;
	int value = 0;
	int N = sboard.get_p() * sboard.get_q();

	for ( int i = 0; i < N; ++i )
	{
		for ( int j = 0; j < N; ++j )
		{
			value = board[i][j];
			std::vector<int> domain;
			if ( value == 0 )
			{
				for ( int d = 1; d <= N; ++d )
				{
					domain.push_back( d );
				}
			}
			else
			{
				domain.push_back( value );
			}

			int block = ( i / sboard.get_p() * sboard.get_p() + j / sboard.get_q() );

			Variable* toPushBack = new Variable(domain,i,j,block);
			variables.push_back(toPushBack);
		}
	}

	std::map<int, VariableSet > rows;
	std::map<int, VariableSet > cols;
	std::map<int, VariableSet > blocks;

	for ( Variable* v:variables )
	{
		int row = v->row();
		int col = v->col();
		int block = v->block();

		if ( !rows.count( row ) )
		{
			rows[row] = VariableSet();
		}
		if ( !cols.count( col ) )
		{
			cols[col] = VariableSet();
		}
		if ( !blocks.count( block ) )
		{
			blocks[block] = VariableSet();
		}
		rows[row].push_back(v);
		cols[col].push_back(v);
		blocks[block].push_back(v);
	}

	for ( Variable* v : variables )
	{
		add( v );
	}

	for ( auto iterator = rows.begin(); iterator != rows.end(); ++iterator )
	{
		Constraint c = Constraint();
		for ( Variable* vv:iterator->second )
		{
			c.addVariable(vv);
		}

		add( c );
	}

	for ( auto iterator = cols.begin(); iterator != cols.end(); ++iterator)
	{
		 Constraint c = Constraint();
		 for ( Variable* vv:iterator->second )
		 {
			c.addVariable(vv);
		 }
		 add( c );
	 }

	 for ( auto iterator = blocks.begin(); iterator != blocks.end(); ++iterator)
	 {
		 Constraint c = Constraint();
		 for ( Variable* vv:iterator->second )
		 {
			 c.addVariable(vv);
		 }
		 add( c );
	  }
}

// =====================================================================
// Modifiers
// =====================================================================

void ConstraintNetwork::add ( Constraint c )
{
	if ( std::find(constraints.begin(),constraints.end(),c) == constraints.end()){
		constraints.push_back( c );
	}
}

void ConstraintNetwork::add ( Variable* v )
{
	if ( std::find( variables.begin(), variables.end(), v ) == variables.end() )
	{
		variables.push_back( v );
	}
}

// =====================================================================
// Accessors
// =====================================================================

ConstraintNetwork::ConstraintSet ConstraintNetwork::getConstraints ( void ) const
{
	return constraints;
}

ConstraintNetwork::VariableSet ConstraintNetwork::getVariables ( void ) const
{
	return variables;
}

// Returns all variables that share a constraint with v
ConstraintNetwork::VariableSet ConstraintNetwork::getNeighborsOfVariable ( Variable* v )
{
	ConstraintNetwork::VariableSet neighbors;

	for ( Constraint& c : constraints )
	{
		if ( c.contains( v ) )
		{
			for ( Variable* va : c.vars )
				if ( va != v && std::find(neighbors.begin(),neighbors.end(),va) == neighbors.end() )
					neighbors.push_back( va );
		}
	}

	return neighbors;
}

// Returns true is every constraint is consistent
bool ConstraintNetwork::isConsistent ( void ) const
{
	for ( Constraint c : constraints )
		if ( ! c.isConsistent() )
			return false;

	return true;
}

// Returns a list of constraints that contains v
ConstraintNetwork::ConstraintRefSet ConstraintNetwork::getConstraintsContainingVariable ( Variable* v )
{
	ConstraintNetwork::ConstraintRefSet outList;

	for ( Constraint& c : constraints )
		if ( c.contains( v ) )
			outList.push_back( &c );

	return outList;
}

/**
 * Returns the constraints that contain variables whose domains were
 * modified since the last call to this method.
 *
 * After getting the constraints, it will reset each variable to
 * unmodified.
 *
 * Note* The first call to this method returns the constraints containing
 * the initialized variables.
 */
ConstraintNetwork::ConstraintRefSet ConstraintNetwork::getModifiedConstraints ( void )
{
	ConstraintNetwork::ConstraintRefSet mConstraints;

	for ( Constraint& c : constraints )
		if ( c.isModified() )
			mConstraints.push_back( &c );

	for ( Variable* v : variables )
		v->setModified( false );

	return mConstraints;
}

// =====================================================================
// String Representation
// =====================================================================

std::string ConstraintNetwork::toString ( void ) const
{
	std::stringstream ss;
	ss << variables.size() << " Variables: {";
	std::string delim = "";

	for ( Variable* v : variables )
	{
		ss << delim << v->getName();
		delim = ",";
	}
	ss<<"}";

	ss << "\n" << constraints.size() << " Constraints:";
	delim = "\n";
	for ( Constraint c: constraints )
	{
		ss << delim << c.toString();
	}
	return ss.str();
}

// =====================================================================
// Sudoku Board Representation
// =====================================================================

SudokuBoard ConstraintNetwork::toSudokuBoard ( int _p, int _q ) const
{
	SudokuBoard::Board board;
	int n = _p*_q;

	for ( int i = 0; i < n; ++i )
	{
		SudokuBoard::Row row;

		for ( int j = 0; j < n; ++j )
			row.push_back( 0 );

		board.push_back( row );
	}

	int row = 0, col = 0;
	for ( Variable* v: getVariables() )
	{
		board[row][col++] = v->getAssignment();

		if ( col >= n )
		{
			col = 0;
			row++;
		}
	}

	return SudokuBoard( _p, _q, board );
}
