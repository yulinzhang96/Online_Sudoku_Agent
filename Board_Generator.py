import sys
import random
import os
import shutil

def isValidValue(row, col, value, p, q, N, board ):
    # check whether current value can be assigned to current variable
    return isValidColValue(col, value, p, q, N, board) and isValidRowValue(row, value, p, q, N, board) and isValidBlock(row, col, value, p, q, N, board)


def isValidColValue(col, value, p, q, N, board):
    return value not in [board[v][col] for v in range(N)]


def isValidRowValue(row, value, p, q, N, board):
    return value not in [board[row][v] for v in range(N)]


def isValidBlock(row, col, value, p, q, N, board):
    rDiv = row // p;
    cDiv = col // q;
    for i in range(rDiv * p, (rDiv + 1) * p):
        for j in range(cDiv * q, (cDiv + 1) * q):
            if board[i][j] == value:
                return False
    return True

def intToOdometer ( n ):
    alphabet='0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    toReturn = ''

    while n != 0:
        n, i = divmod(n, len(alphabet))
        toReturn = alphabet[i] + toReturn

    if toReturn == '':
        toReturn = '0'

    return toReturn

def genBoard ( p, q, m, filename ):
    N = p*q
    board = [[0 for j in range(N)] for i in range(N)]

    while True:
        if m <= 0:
            break

        randomRow = random.randint(0, N-1)
        randomCol = random.randint(0, N-1)
        randomAssignment = random.randint(1, N)
        if board[randomRow][randomCol] == 0 and isValidValue( randomRow, randomCol, randomAssignment, p, q, N, board ):
            board[randomRow][randomCol] = randomAssignment
            m -= 1

    file = open(filename, "w")
    file.write( str(p) + " " + str(q) + "\n" )
    for i in range(N):
        for j in range(N):
            file.write( intToOdometer( board[i][j] ) + " " )
        file.write("\n")

    file.close();

if os.path.exists("/board_0.txt"):
    os.remove(file)

if len(sys.argv) != 6:
    print ( "Usage: Board_Generator Base_File_Name #ofBoards p q m" )
    exit(0)

baseFileName = sys.argv[1]
numOfFiles = int(sys.argv[2])
p = int(sys.argv[3])
q = int(sys.argv[4])
m = int(sys.argv[5])

for i in range(numOfFiles):
    print ( "Creating board number: " + str(i) + "." )
    genBoard( p, q, m, baseFileName + "_" + str(i) + ".txt" )
