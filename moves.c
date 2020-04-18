#include "common.h"
#include "ai.h"

position* getPositionAfterMove(position* pos, move m){
	if(m.coordinates[0]=='O'||m.coordinates[0]=='o'){
		// Castling
		position* new_pos = createNewPosition(pos->board);
		int j; char k,r,e='.';
		if (m.coordinates[0]=='O'){
			// White
			j = 7;
			k = 'K';
			r = 'R';
		} else {
			// Black
			j = 0;
			k = 'k';
			r = 'r';
		}
		if(m.coordinates[2]=='O'||m.coordinates[2]=='o'){
			// Queen Side
			new_pos->board[j][4]=e;
			new_pos->board[j][3]=r;
			new_pos->board[j][2]=k;
			new_pos->board[j][1]=e;
			new_pos->board[j][0]=e;
		} else {
			// King Side
			new_pos->board[j][4]=e;
			new_pos->board[j][5]=r;
			new_pos->board[j][6]=k;
			new_pos->board[j][7]=e;
		}
		new_pos->turn = (pos->turn=='w')?'b':'w';
		new_pos->currentMove = m;
		return new_pos;
	}
	int x1=m.coordinates[0]-'a';
	int y1=8-(m.coordinates[1]-'0');
	int x2=m.coordinates[2]-'a';
	int y2=8-(m.coordinates[3]-'0');
	char moving_piece = pos->board[y1][x1];
	position* new_pos = createNewPosition(pos->board);
	new_pos->board[y1][x1] = '.';
	new_pos->board[y2][x2] = moving_piece;
	new_pos->turn = (pos->turn=='w')?'b':'w';
	new_pos->currentMove = m;
	if(moving_piece=='P' && y2 == 0){
		new_pos->board[y2][x2] = 'Q';
	}
	if(moving_piece=='p' && y2 == BOARD_SIZE-1){
		new_pos->board[y2][x2] = 'q';
	}
	new_pos->moveno = pos->moveno+1;
	return new_pos;
}

move* possibleNextMoves(position* pos){
	//50 Moves Max
	move* movelist = malloc(MAXMOVES*sizeof(move));

	char piece;
	move m;
	int l = 0,x,y;
	for(int j=0; j<BOARD_SIZE; j++){
		for(int i=0; i<BOARD_SIZE; i++){
			piece = pos->board[j][i];
			switch(piece){
				case '.':
					// No Move Possible
					break;
				case 'P':
					if(pos->turn=='b') break;
					if(pos->board[j-1][i]=='.'){
						m = newMove(i,j,i,j-1);
						movelist[l] = m; l++;
					}
					if(j==BOARD_SIZE-2 && pos->board[j-2][i]=='.'&& pos->board[j-1][i]=='.'){
						m = newMove(i,j,i,j-2);
						movelist[l] = m; l++;
					}
					if(isValidCoordinates(i+1,j-1) && isBlackSymbol(pos->board[j-1][i+1])){
						m = newMove(i,j,i+1,j-1);
						movelist[l] = m; l++;
					}
					if(isValidCoordinates(i-1,j-1) && isBlackSymbol(pos->board[j-1][i-1])){
						m = newMove(i,j,i-1,j-1);
						movelist[l] = m; l++;
					}
					break;
				case 'p':
					if(pos->turn=='w') break;
					if(pos->board[j+1][i]=='.'){
						m = newMove(i,j,i,j+1);
						movelist[l] = m; l++;
					}
					if(j==1 && pos->board[j+2][i]=='.'&& pos->board[j+1][i]=='.'){
						m = newMove(i,j,i,j+2);
						movelist[l] = m; l++;
					}
					if(isValidCoordinates(i+1,j+1) && isWhiteSymbol(pos->board[j+1][i+1])){
						m = newMove(i,j,i+1,j+1);
						movelist[l] = m; l++;
					}
					if(isValidCoordinates(i-1,j+1) && isWhiteSymbol(pos->board[j+1][i-1])){
						m = newMove(i,j,i-1,j+1);
						movelist[l] = m; l++;
					}
					break;
				case 'K':
					if(pos->turn=='b') break;
					for (int x = -1; x <= 1; ++x)
					{
						for (int y = -1; y <= 1; ++y)
						{
							if(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
								||isBlackSymbol(pos->board[j+y][i+x]))){
								m = newMove(i,j,i+x,j+y);
								movelist[l] = m; l++;
							}
						}
					}
					if(j==7&&i==4&&pos->board[7][4]=='K'&&
								   pos->board[7][5]=='.'&&
								   pos->board[7][6]=='.'&&
								   pos->board[7][7]=='R'){
						// White King Side Castling 'OO\0\0'
						move m;
						m.coordinates[0] = 'O';
						m.coordinates[1] = 'O';
						m.coordinates[2] = '\0';
						m.coordinates[3] = '\0';
						movelist[l] = m; l++;
					}
					if(j==7&&i==4&&pos->board[7][4]=='K'&&
								   pos->board[7][3]=='.'&&
								   pos->board[7][2]=='.'&&
								   pos->board[7][1]=='.'&&
								   pos->board[7][0]=='R'){
						// White Queen Side Castling 'OOO\0'
						move m;
						m.coordinates[0] = 'O';
						m.coordinates[1] = 'O';
						m.coordinates[2] = 'O';
						m.coordinates[3] = '\0';
						movelist[l] = m; l++;
					}
					break;
				case 'k':
					if(pos->turn=='w') break;
					for (int x = -1; x <= 1; ++x)
					{
						for (int y = -1; y <= 1; ++y)
						{
							if(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
								||isWhiteSymbol(pos->board[j+y][i+x]))){
								m = newMove(i,j,i+x,j+y);
								movelist[l] = m; l++;
							}
						}
					}
					if(j==0&&i==4&&pos->board[0][4]=='k'&&
								   pos->board[0][5]=='.'&&
								   pos->board[0][6]=='.'&&
								   pos->board[0][7]=='r'){
						// Black King Side Castling 'oo\0\0'
						move m;
						m.coordinates[0] = 'o';
						m.coordinates[1] = 'o';
						m.coordinates[2] = '\0';
						m.coordinates[3] = '\0';
						movelist[l] = m; l++;
					}
					if(j==0&&i==4&&pos->board[0][4]=='k'&&
								   pos->board[0][3]=='.'&&
								   pos->board[0][2]=='.'&&
								   pos->board[0][1]=='.'&&
								   pos->board[0][0]=='r'){
						// Black Queen Side Castling 'ooo\0'
						move m;
						m.coordinates[0] = 'o';
						m.coordinates[1] = 'o';
						m.coordinates[2] = 'o';
						m.coordinates[3] = '\0';
						movelist[l] = m; l++;
					}
					break;
					break;
				case 'N':
					if(pos->turn=='b') break;
					x = 1; y = 2;
					if(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
					}
					x = 2; y = 1;
					if(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
					}
					x = 1; y = -2;
					if(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
					}
					x = 2; y = -1;
					if(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
					}
					x = -1; y = 2;
					if(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
					}
					x = -2; y = 1;
					if(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
					}
					x = -1; y = -2;
					if(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
					}
					x = -2; y = -1;
					if(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
					}
					break;
				case 'n':
					if(pos->turn=='w') break;
					x = 1; y = 2;
					if(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
					}
					x = 2; y = 1;
					if(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
					}
					x = 1; y = -2;
					if(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
					}
					x = 2; y = -1;
					if(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
					}
					x = -1; y = 2;
					if(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
					}
					x = -2; y = 1;
					if(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
					}
					x = -1; y = -2;
					if(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
					}
					x = -2; y = -1;
					if(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
					}
					break;
				case 'R':
					if(pos->turn=='b') break;
					x=1; y=0;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isBlackSymbol(pos->board[j+y][i+x])){
							break;
						}
						x++;
					}
					x=-1; y=0;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isBlackSymbol(pos->board[j+y][i+x])){
							break;
						}
						x--;
					}
					x=0; y=1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isBlackSymbol(pos->board[j+y][i+x])){
							break;
						}
						y++;
					}
					x=0; y=-1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isBlackSymbol(pos->board[j+y][i+x])){
							break;
						}
						y--;
					}
					break;
				case 'r':
					if(pos->turn=='w') break;
					x=1; y=0;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isWhiteSymbol(pos->board[j+y][i+x])){
							break;
						}
						x++;
					}
					x=-1; y=0;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isWhiteSymbol(pos->board[j+y][i+x])){
							break;
						}
						x--;
					}
					x=0; y=1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isWhiteSymbol(pos->board[j+y][i+x])){
							break;
						}
						y++;
					}
					x=0; y=-1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isWhiteSymbol(pos->board[j+y][i+x])){
							break;
						}
						y--;
					}
					break;
				case 'B':
					if(pos->turn=='b') break;
					x=1; y=1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isBlackSymbol(pos->board[j+y][i+x])){
							break;
						}
						x++; y++;
					}
					x=1; y=-1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isBlackSymbol(pos->board[j+y][i+x])){
							break;
						}
						x++; y--;
					}
					x=-1; y=1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isBlackSymbol(pos->board[j+y][i+x])){
							break;
						}
						x--; y++;
					}
					x=-1; y=-1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isBlackSymbol(pos->board[j+y][i+x])){
							break;
						}
						x--; y--;
					}
					break;
				case 'b':
					if(pos->turn=='w') break;
					x=1; y=1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isWhiteSymbol(pos->board[j+y][i+x])){
							break;
						}
						x++; y++;
					}
					x=1; y=-1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isWhiteSymbol(pos->board[j+y][i+x])){
							break;
						}
						x++; y--;
					}
					x=-1; y=1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isWhiteSymbol(pos->board[j+y][i+x])){
							break;
						}
						x--; y++;
					}
					x=-1; y=-1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isWhiteSymbol(pos->board[j+y][i+x])){
							break;
						}
						x--; y--;
					}
					break;
				case 'Q':
					if(pos->turn=='b') break;
					x=1; y=0;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isBlackSymbol(pos->board[j+y][i+x])){
							break;
						}
						x++;
					}
					x=-1; y=0;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isBlackSymbol(pos->board[j+y][i+x])){
							break;
						}
						x--;
					}
					x=0; y=1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isBlackSymbol(pos->board[j+y][i+x])){
							break;
						}
						y++;
					}
					x=0; y=-1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isBlackSymbol(pos->board[j+y][i+x])){
							break;
						}
						y--;
					}
					if(pos->turn=='b') break;
					x=1; y=1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isBlackSymbol(pos->board[j+y][i+x])){
							break;
						}
						x++; y++;
					}
					x=1; y=-1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isBlackSymbol(pos->board[j+y][i+x])){
							break;
						}
						x++; y--;
					}
					x=-1; y=1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isBlackSymbol(pos->board[j+y][i+x])){
							break;
						}
						x--; y++;
					}
					x=-1; y=-1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isBlackSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isBlackSymbol(pos->board[j+y][i+x])){
							break;
						}
						x--; y--;
					}
					break;
				case 'q':
					if(pos->turn=='w') break;
					x=1; y=0;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isWhiteSymbol(pos->board[j+y][i+x])){
							break;
						}
						x++;
					}
					x=-1; y=0;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isWhiteSymbol(pos->board[j+y][i+x])){
							break;
						}
						x--;
					}
					x=0; y=1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isWhiteSymbol(pos->board[j+y][i+x])){
							break;
						}
						y++;
					}
					x=0; y=-1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isWhiteSymbol(pos->board[j+y][i+x])){
							break;
						}
						y--;
					}
					if(pos->turn=='b') break;
					x=1; y=1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isWhiteSymbol(pos->board[j+y][i+x])){
							break;
						}
						x++; y++;
					}
					x=1; y=-1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isWhiteSymbol(pos->board[j+y][i+x])){
							break;
						}
						x++; y--;
					}
					x=-1; y=1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isWhiteSymbol(pos->board[j+y][i+x])){
							break;
						}
						x--; y++;
					}
					x=-1; y=-1;
					while(isValidCoordinates(i+x,j+y) && (pos->board[j+y][i+x]=='.'
						||isWhiteSymbol(pos->board[j+y][i+x]))){
						m = newMove(i,j,i+x,j+y);
						movelist[l] = m; l++;
						if(isWhiteSymbol(pos->board[j+y][i+x])){
							break;
						}
						x--; y--;
					}
					break;
			}
		}
	}
	m.move = 0;
	movelist[l] = m;

	return movelist;
}

move newMove(int x1, int y1, int x2, int y2){
	move m;
	m.coordinates[0] = 'a'+x1;
	m.coordinates[1] = (8-y1)+'0';
	m.coordinates[2] = 'a'+x2;
	m.coordinates[3] = (8-y2)+'0';
	return m;
}

bool isValidCoordinates(int i, int j){
	if(i<0)
		return false;
	if(j<0)
		return false;
	if(i>BOARD_SIZE-1)
		return false;
	if(j>BOARD_SIZE-1)
		return false;
	return true;
}

move moveplayed(position* pos){
	move m = pos->currentMove;
	if(m.coordinates[0]=='O'||m.coordinates[0]=='o'){
		return m;
	}
	int x1=m.coordinates[0]-'a';
	int y1=8-(m.coordinates[1]-'0');
	int x2=m.coordinates[2]-'a';
	int y2=8-(m.coordinates[3]-'0');
	char moving_piece = pos->board[y2][x2];
	move newMove;
	if(moving_piece!='p' && moving_piece !='P')
		newMove.coordinates[0] = moving_piece;
	else
		newMove.coordinates[0] = ' ';
	newMove.coordinates[1] = m.coordinates[2];
	newMove.coordinates[2] = m.coordinates[3];
	newMove.coordinates[3] = '\0';
	return newMove;
}
