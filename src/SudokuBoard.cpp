#include "SudokuBoard.hpp"

using namespace std;

/**
 * Represents a Sudoku Board. This is converted to a constraint network,
 * so BTSolver can interface with it as a CSP.
 */

// =====================================================================
// Constructors
// =====================================================================

// Will generate a random Sudoku board
SudokuBoard::SudokuBoard ( int _p, int _q, int _m )
{
	p = _p;
	q = _q;
	int N = p*q;

	for ( int i = 0; i < N; ++i )
	{
		Row row;

		for ( int j = 0; j < N; ++j )
			row.push_back( 0 );

		board.push_back(row);
	}

	if ( _m > N*N )
		_m = N*N;

	srand( time( NULL ) );

	for ( int i = 0; i <= _m; )
	{
		int randomRow = rand() % N;
		int randomCol = rand() % N;
		int randomAssignment = (rand() % N) + 1;

		if ( board[randomRow][randomCol] == 0 )
		{
			if ( check ( randomRow, randomCol, randomAssignment ) )
			{
				board[randomRow][randomCol] = randomAssignment;
				++i;
			}
		}
	}
}

// Will initialize a board
SudokuBoard::SudokuBoard ( int _p, int _q, Board _board )
{
	p = _p;
	q = _q;
	board = _board;
}

// Will load a Sudoku board from file
SudokuBoard::SudokuBoard ( std::string filename )
{
	ifstream file( filename );

	file >> p;
	file >> q;
	int N = p*q;
	string x = "";

	for ( int i = 0; i < N; ++i )
	{
		Row row;

		for ( int j = 0; j < N; ++j )
		{
			file >> x;
			row.push_back( odometerToInt( x ) );
		}

		board.push_back( row );
	}
}

// =====================================================================
// Accessors
// =====================================================================

int SudokuBoard::get_p ( void ) const
{
	return p;
}

int SudokuBoard::get_q ( void ) const
{
	return q;
}

int SudokuBoard::get_n ( void ) const
{
	return p*q;
}

SudokuBoard::Board SudokuBoard::get_board ( void ) const
{
	return board;
}

// =====================================================================
// String representation
// =====================================================================

string SudokuBoard::toString ( void ) const
{
	stringstream ss;
	int N = p*q;
	//ss << "P:  " << p << "\t" << "Q:  " << q << "\n";

	for ( int i = 0; i < N; ++i )
	{
		for ( int j = 0; j < N; ++j )
		{
			ss << intToOdometer( board[i][j] ) ;

			if ( (j+1) % q == 0 && j != 0 && j != N-1 )
			{
				//ss << "| ";
			}
		}

		//ss << "\n";

		if ( (i+1) % p == 0 && i != 0 && i != N-1 )
		{
			for ( int k = 0; k < N+p-1; ++k )
			{
				//ss << "- ";
			}
			//ss << "\n";
		}
	}
	return ss.str();
}

// =====================================================================
// Private Helper Methods
// =====================================================================

bool SudokuBoard::check ( int row, int col, int value ) const
{
	return checkRow( row, value ) && checkCol( col, value ) && checkBox( row, col, value );
}

bool SudokuBoard::checkRow ( int row, int value ) const
{
	for ( int i = 0; i < p*q; ++i )
		if ( board[row][i] == value )
			return false;
	return true;
}

bool SudokuBoard::checkCol ( int col, int value ) const
{
	for ( int i = 0; i < p*q; ++i )
		if ( board[i][col] == value )
			return false;
	return true;
}

bool SudokuBoard::checkBox ( int row, int col, int value ) const
{
	int rDiv = row / p;
	int cDiv = col / q;

	for ( int i = rDiv * p; i < (rDiv + 1) * p; ++i )
	{
		for ( int j = cDiv * q; j < (cDiv + 1) * q; ++j )
		{
			if(board[i][j] == value)
				return false;
		}
	}

	return true;
}

string SudokuBoard::intToOdometer ( int n ) const
{
	string ret = "";

	for ( int i = n; n > 0; n /= 36 )
	{
		int r  = n % 36;
		char x = r < 10 ? '0' + r : 'A' + r - 10;
		ret = x + ret;
	}

	return ret == "" ? "0" : ret;
}

int SudokuBoard::odometerToInt ( string str ) const
{
	int x = 0;

	if ( str[0] >= '0' && str[0] <= '9' )
		x = str[0] - '0';

	if ( str[0] >= 'A' && str[0] <= 'Z' )
		x = str[0] - 'A' + 10;

	for ( int i = 1; i < str.size(); ++i )
		x *= 36;

	if ( str.size() > 1 )
		return x + odometerToInt ( str.substr( 1, str.size() - 1 ) );

	return x;
}
