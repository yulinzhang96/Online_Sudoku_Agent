#include "Domain.hpp"

/**
 * Represents the domain of a variable, i.e. the possible values that each
 * variable may assign.
 */

// =====================================================================
// Constructors
// =====================================================================

// Constructs a domain with a single value
Domain::Domain ( int value )
{
	values.push_back( value );
}

// Constructs a domain from a list of values
Domain::Domain ( Domain::ValueSet vals )
{
	values = vals;
}

// Copy Constructor
Domain::Domain ( const Domain& d )
{
	values = d.values;
}

// Default Constructor
Domain::Domain (){};

// =====================================================================
// Accessors
// =====================================================================

Domain::ValueSet Domain::getValues ( void ) const
{
	return values;
}

// Checks if value exists within the domain
bool Domain::contains ( int value ) const
{
	if ( std::find( values.begin(), values.end(), value ) != values.end() )
		return true;
	return false;
}

// Returns number of values in the domain
int Domain::size ( void ) const
{
	return values.size();
}

// Returns true if no values are contained in the domain
bool Domain::isEmpty ( void ) const
{
	return values.empty();
}

// Returns whether or not the domain has been modified
bool Domain::isModified ( void ) const
{
	return modified;
}

bool Domain::equals ( Domain d ) const
{
	return false;
}

// =====================================================================
// Modifiers
// =====================================================================

// Adds a value to the domain
void Domain::add ( int value )
{
	if ( ! contains( value ) )
		values.push_back( value );
}

// Remove a value from the domain
bool Domain::remove ( int value )
{
	int v = value;

	if ( std::find( values.begin(), values.end(), value ) == values.end() )
		return false;

	setModified( true );
	values.erase( std::remove( values.begin(), values.end(), value ), values.end() );
	return true;
}

// Sets the modified flag
void Domain::setModified ( bool modified )
{
	this->modified = modified;
}

// =====================================================================
// Iterator
// =====================================================================

Domain::ValueSet::iterator Domain::begin()
{
	return values.begin();
}

Domain::ValueSet::iterator Domain::end()
{
	return values.end();
}

// =====================================================================
// String representation
// =====================================================================

std::string Domain::toString( void ) const
{
	std::stringstream ss;
	ss << "{";
	std::string delim = "";

	for ( int i = 0; i < values.size(); ++i )
	{
		ss << delim << values[i];
		delim = ",";
	}

	ss << "}";
	return ss.str();
}
