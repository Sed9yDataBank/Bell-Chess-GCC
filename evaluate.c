#include "common.h"

int pieceCountValue(char piece){
	// Returns Piece Value, +ve If White, -ve If Black
	switch(piece){
		case 'P':
			return +100;
		case 'p':
			return -100;
		case 'R':
			return +500;
		case 'r':
			return -500;
		case 'N':
			return +300;
		case 'n':
			return -300;
		case 'B':
			return +350;
		case 'b':
			return -350;
		case 'Q':
			return +900;
		case 'q':
			return -900;
		case 'K':
			return +100000;
		case 'k':
			return -100000;
		case '.':
			return 0;
	}
}

int evaluate(position* pos){
	// Function Always Returns Who Is Winning Regardless Of Whose Move It Is. I.e. The Return
    
	int evaluation = 0;

	if(!isGameRunning(pos)){
		switch(whoWon(pos)){
			case 1:
				return 1000000;
			case -1:
				return -1000000;
			case 0:
				return 0;
		}
	}

	// Count Pieces
	char piece;
	for(int j=0; j<BOARD_SIZE; j++){
		for(int i=0; i<BOARD_SIZE; i++){
			piece = pos->board[j][i];
			evaluation += pieceCountValue(piece);
		}
	}

	return evaluation;
}
