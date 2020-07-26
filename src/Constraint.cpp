#include"Constraint.hpp"

/**
 * Constraint represents a NotEquals constraint on a set of variables.
 * Used to ensure none of the variables contained in the constraint have the same assignment.
 */

Constraint::Constraint ( void )
{

}

Constraint::Constraint ( VariableSet vars )
{
	this->vars = vars;
}

void Constraint::addVariable ( Variable* v )
{
	vars.push_back( v );
}

int Constraint::size ( void )
{
	return vars.size();
}

// Returns true if v is in the constraint, false otherwise
bool Constraint::contains ( Variable* v )
{
	if ( std::find( vars.begin(), vars.end(), v ) != vars.end() )
		return true;

	return false;
}

// Returns whether or not the a variable in the constraint has been modified
bool Constraint::isModified ( void )
{
	for ( Variable* var:vars )
		if ( var->isModified() )
			return true;

	return false;
}

// Returns true if constraint is consistent, false otherwise
bool Constraint::isConsistent ( void )
{
	for ( Variable* var : vars )
	{
		if ( !var->isAssigned() )
			continue;

		for ( Variable* otherVar : vars )
		{
			if ( *var == *otherVar )
				continue;

			if ( otherVar->isAssigned()
				 && otherVar->getAssignment() == var->getAssignment() )
				return false;
		}
	}

	return true;
}

bool Constraint::operator== ( const Constraint &other ) const
{
	if ( vars.size() == other.vars.size() )
	{
		for ( Variable* v : vars )
		{
			if ( std::find( other.vars.begin(), other.vars.end(),v ) == other.vars.end() )
				return false;
		}
		return true;
	}
	return false;
}

std::string Constraint::toString ( void )
{
	std::stringstream ss;
	ss << "{";
	std::string delim = "";

	for ( Variable* v : vars )
	{
		ss << delim << v->getName();
		delim = ",";
	}

	ss << "}";
	return ss.str();
}
