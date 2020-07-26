#include"Variable.hpp"

/**
 * Represents a variable in a CSP
 */

int Variable::namingCounter = 1;

// =====================================================================
// Constructors
// =====================================================================

Variable::Variable ( Domain::ValueSet possible_Values, int row, int col, int block ) : domain(possible_Values)
{
	r = row;
	c = col;
	b = block;
	name = "v" + std::to_string(namingCounter++);
	oldSize = size();
	modified = false;
	changeable = true;
	assigned = false;

	if ( domain.size() == 1 )
	{
		modified = true;
		changeable = false;
		assigned = true;
	}
}

Variable::Variable ( const Variable& v ) : domain(v.domain)
{
	r = v.row();
	c = v.col();
	b = v.block();
	modified = v.isModified();
	name = v.getName();
	changeable = v.isChangeable();
}

// =====================================================================
// Accessors
// =====================================================================

bool Variable::isChangeable( void ) const
{
	return changeable;
}

bool Variable::isAssigned( void ) const
{
	return assigned;
}

bool Variable::isModified( void ) const
{
	return modified;
}

int Variable::row ( void ) const
{
	return r;
}

int Variable::col ( void ) const
{
	return c;
}

int Variable::block ( void ) const
{
	return b;
}

int Variable::size ( void ) const
{
	return domain.size();
}

// Returns the assigned value or 0 if unassigned
int Variable::getAssignment ( void ) const
{
	if ( isAssigned() )
		return domain.getValues()[0];

	return 0;
}

Domain Variable::getDomain ( void ) const
{
	return domain;
}

std::string Variable::getName ( void ) const
{
	return name;
}

Domain::ValueSet Variable::getValues ( void ) const
{
	return domain.getValues();
}

bool Variable::operator== ( const Variable &other ) const
{
	return (row() == other.row()) && (col() == other.col()) && (block() == other.block());
}

bool Variable::operator!= ( const Variable &other ) const
{
	return !(*this == other);
}

// =====================================================================
// Modifiers
// =====================================================================

void Variable::unassign()
{
	assigned = false;
}

void Variable::setModified( bool modified )
{
	this->modified = modified;
	this->domain.setModified(modified);
}

// Assign a value to the variable
void Variable::assignValue ( int val )
{
	if ( ! changeable )
		return;

	assigned = true;
	setDomain( Domain( val ) );
}

// Sets the domain of the variable
void Variable::setDomain ( Domain d )
{
	if ( ! changeable )
		return;

	domain = d;
	modified = true;
}

// Removes a value from the domain
void Variable::removeValueFromDomain ( int val )
{
	if ( ! changeable )
		return;

	domain.remove(val);
	modified = domain.isModified();
}

// =====================================================================
// Iterator
// =====================================================================

Domain::iterator Variable::begin()
{
	return domain.begin();
}

Domain::iterator Variable::end()
{
	return domain.end();
}

// =====================================================================
// String representation
// =====================================================================

std::string Variable::toString()
{
	std::stringstream ss;
	std::string sep = "";
	ss << " Name: " << name;
	ss << "\tdomain: {";

	for ( int i = 0; i < domain.getValues().size(); ++i )
	{
		ss << sep << domain.getValues()[i];
		sep = ",";
	}

	ss << "}";
	return ss.str();
}
