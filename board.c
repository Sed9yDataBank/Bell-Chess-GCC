#include "common.h"

/*
 * Representation Of Board In Memory
 * --------------------------------------------
 *
 * The Variable Position->Board Is A 2D Array And Hence Starts Like
 * board [0][0], [0][1], [0][2], ....
 *       [1][0], [1][1], [1][2], ....
 *       ....
 *
 *But The Chess Board Has Coordinates Like
 *
 * 	a8 b8 c8 d8 e8 f8 g8 h8
 * 	a7 b7 c7 d7 e7 f7 g7 h7
 * 	a6 b6 c6 d6 e6 f6 g6 h6
 * 	a5 b5 c5 d5 e5 f5 g5 h5
 * 	a4 b4 c4 d4 e4 f4 g4 h4
 * 	a3 b3 c3 d3 e3 f3 g3 h3
 * 	a2 b2 c2 d2 e2 f2 g2 h2
 * 	a1 b1 c1 d1 e1 f1 g1 h1
 *
 * So board[j][i] Actually In Chess Corresponds To Coordinates
 * ('a'+i,'0'+8-j).
 *
 */

position* createNewPosition(char a[BOARD_SIZE][BOARD_SIZE]){
	position* newPos = malloc(sizeof(position));
	//newPos->lastPos=NULL;
	char *q = (char*)a;
	char *w = (char*)newPos->board;
	newPos->turn = 'w';
	newPos->evaluation = 0;
	newPos->currentMove.move = 0;
	newPos->moveno=1;
	*(q+BOARD_SIZE*BOARD_SIZE) = '\0';
	strcpy(w,q);
	return newPos;
}

void deletePosition(position* pos){
	free(pos);
}

void displayBoard(position* pos){
	char line[]="+---+---+---+---+---+---+---+---+";
	char x = ' ';
	for(int j=0; j<BOARD_SIZE; j++){
		printf("%s\n",line);
		for(int i=0; i<BOARD_SIZE; i++){
			if(pos->board[j][i]!='.')
				printf("|%c%c%c",x,pos->board[j][i],x);
			else
				printf("|%c%c%c",x,x,x);
		}
		printf("|\n");
	}
	printf("%s\n",line);
}

void log_board(FILE* f,position* pos){
	if (!pos)
	{
		fprintf(f, "No Position Initialised.\n");
		return;
	}
	char line[]="+---+---+---+---+---+---+---+---+";
	char x = ' ';
	for(int j=0; j<BOARD_SIZE; j++){
		fprintf(f,"%s\n",line);
		for(int i=0; i<BOARD_SIZE; i++){
			if(pos->board[j][i]!='.')
				fprintf(f,"|%c%c%c",x,pos->board[j][i],x);
			else
				fprintf(f,"|%c%c%c",x,x,x);
		}
		fprintf(f,"|\n");
	}
	fprintf(f,"%s\n",line);
}

bool isGameRunning(position* pos){
	// True If Running
	// False If Ended
	bool isThereAWhiteKing = false;
	bool isThereABlackKing = false;
	bool areThereValidMoves = false;
	for(int j=0; j<BOARD_SIZE; j++){
		for(int i=0; i<BOARD_SIZE; i++){
			switch(pos->board[j][i]){
				case 'k':
					isThereABlackKing = true;
					break;
				case 'K':
					isThereAWhiteKing = true;
					break;
			}
		}
	}
	move* movelist = possibleNextMoves(pos);
	if(movelist[0].move){
		areThereValidMoves = true;
		free(movelist);
	}
	return isThereAWhiteKing && isThereABlackKing && areThereValidMoves;
}

int whoWon(position* pos){
	// Returns -1 For Black Won; +1 For White Won; 0 For Draw;
	// Returns 2 For Game Still Running.
	bool isThereAWhiteKing = false;
	bool isThereABlackKing = false;
	bool areThereValidMoves = false;
	for(int j=0; j<BOARD_SIZE; j++){
		for(int i=0; i<BOARD_SIZE; i++){
			switch(pos->board[j][i]){
				case 'k':
					isThereABlackKing = true;
					break;
				case 'K':
					isThereAWhiteKing = true;
					break;
			}
		}
	}
	move* movelist = possibleNextMoves(pos);
	if(movelist[0].move)
		areThereValidMoves = true;
	free(movelist);
	if(!isThereABlackKing) return 1;
	if(!isThereAWhiteKing) return -1;
	if(!areThereValidMoves) return 0;
	return 2;
}
