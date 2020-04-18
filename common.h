#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

#define BOARD_SIZE 8
#define MAXMOVES 100

// Prototypes For board.c
#define isWhiteSymbol(x) ('A'<x&&x<'Z')
#define isBlackSymbol(x) ('a'<x&&x<'z')

// Representation Of A Move In Algebriac Notation Like "e2e4"
typedef union move{
	int move;
	char coordinates[4];
} move;

// State Of The Game, Includes Pointer To Last Position
typedef struct position {
	char board[BOARD_SIZE+1][BOARD_SIZE];
	// Board Position Stored Like This:
	// rnbqkbnr pppppppp ........ PPPPPPPP RNBQKBNR
	// Small Letters For Black And Capital For White
	// Dots For Blank Space
	// Board Always Shown With White At Bottom
	char turn; // W For White And B For Black
	int evaluation;
	move currentMove;
	unsigned short int moveno;
} position;

bool isGameRunning(position* pos);
int whoWon(position* pos);

position* createNewPosition(char a[BOARD_SIZE][BOARD_SIZE]);
void deletePosition(position* pos);

void displayBoard(position* pos);
void log_board(FILE*,position*);

// Prototypes
int expandnode(position** nodesList, move* movelist, position* initialPos);
position* findminormax(position* initialPos, position** nodesList);
void deleteAllNodes(position** nodesList);
position* evaluation_at_end(position* initialPos);

move newMove(int x1, int y1, int x2, int y2);

bool isValidCoordinates(int i, int j);

position* getPositionAfterMove(position*, move);

move* possibleNextMoves(position*);

move moveplayed(position* pos);

// Returns Position Evaluation In Centipawns
// +100 Means White Winning And -100 Means Black Winning By 1 Pawn
int evaluate(position*);

extern bool opening;
extern move movehistory[];
extern move openingBookMoves[500][10];
void loadOpeningBook(char* filename);
position* getOpeningFromBook(position* initialPos, int plyDepth);

position* getBestMove(position* pos, int plyDepth);
position* getBestMove_threaded(position* initialPos, int plyDepth);

void runTests();
