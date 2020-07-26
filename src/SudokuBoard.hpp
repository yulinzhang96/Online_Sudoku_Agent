#ifndef SUDOKUBOARD_HPP
#define SUDOKUBOARD_HPP

#include "Domain.hpp"
#include "Variable.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

/**
 * Represents a Sudoku Board. This is converted to a constraint network,
 * so BTSolver can interface with it as a CSP.
 */

class SudokuBoard
{
public:
	// Typedefs
	typedef typename std::vector< std::vector< int > > Board;
	typedef typename std::vector< int > Row;
	typedef typename std::vector< int > Column;

	// Constructors
	SudokuBoard ( int p, int q, int m ); // Will generate a random Sudoku board
	SudokuBoard ( int p, int q, Board board ); // Will initialize a board
	SudokuBoard ( std::string filename ); // Will load a Sudoku board from file

	// Accessors
	int   get_p     ( void ) const;
	int   get_q     ( void ) const;
	int   get_n     ( void ) const;
	Board get_board ( void ) const;

	// String representation
	std::string toString ( void ) const;

private:
	int p; // number of rows in a block && number of block columns
	int q; // number of columns in a block && number of block rows

	Board board;

    bool check    ( int row, int col, int value ) const;
	bool checkRow ( int row, int value ) const;
	bool checkCol ( int col, int value ) const;
	bool checkBox ( int row, int col, int value ) const;

	std::string intToOdometer ( int n )      const;
	int         odometerToInt ( std::string str ) const;

};

#endif
