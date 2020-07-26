#ifndef DOMAIN_HPP
#define DOMAIN_HPP

#include <iterator>
#include <sstream>
#include <vector>
#include <stack>
#include <utility>
#include <iostream>
#include <algorithm>

/**
 * Represents the domain of a variable, i.e. the possible values that each
 * variable may assign.
 */

class Domain
{
public:
	// Typedefs
	typedef std::vector< int > ValueSet;
	typedef ValueSet::iterator iterator;

	// Constructors
	Domain ( int value );
	Domain ( ValueSet values );
	Domain ( const Domain& d );
    Domain ();

	// Accessors
	ValueSet getValues  ( void )      const;
	bool     contains   ( int value ) const;
	int      size       ( void )      const;
	bool     isEmpty    ( void )      const;
	bool     isModified ( void )      const;
	bool     equals     ( Domain d )  const;

	// Modifiers
	void add         ( int value );
	bool remove      ( int value );
	void setModified ( bool modified );

	// Iterator
	ValueSet::iterator begin();
	ValueSet::iterator end();

	// String representation
	std::string toString ( void ) const;

private:
	// Properties
	ValueSet values;
	bool modified;
};

#endif
