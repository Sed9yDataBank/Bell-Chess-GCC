// ============================
//
//    PROJECT MEGACHESSATRON
//
// ============================


#include "common.h"
#include "ai.h"

int main(int argc, char const *argv[]){

	loadOpeningBook("opening_book.txt");

	#ifdef TESTING
	runTests();
	return 0;
	#endif

	#ifdef XBOARD
	#ifdef LOGGING
		int rn = (int) time(NULL);
		char filename[100];
		sprintf(filename,"logfile%d.txt",rn);
		FILE* logfile = fopen(filename,"w");
	#endif

	position *current_position;
	char initboard[8][8]={"rnbqkbnr","pppppppp","........",
		"........","........","........","PPPPPPPP","RNBQKBNR"};
	setbuf(stdout, NULL);
	#ifdef LOGGING
		setbuf(logfile, NULL);
	#endif

	int p = 1;
	int depth = 4;
	char inp[100];
	int inputs_to_ignore = 0;
	#ifdef LOGGING
		char *ch;
	#endif

	while(1)
	{
		scanf("%s",inp);
		#ifdef LOGGING
			fprintf(logfile, "<<< %s\n", inp);
		#endif
		if(inputs_to_ignore>0){
			inputs_to_ignore--;
		}
		else if(strcmp(inp,"xboard")==0
			   || strcmp(inp,"accepted")==0
			   || strcmp(inp,"rejected")==0
			   || strcmp(inp,"force")==0
			   || strcmp(inp,"white")==0
			   || strcmp(inp,"black")==0
			   || strcmp(inp,"draw")==0
			   || strcmp(inp,"hint")==0
			   || strcmp(inp,"bk")==0
			   || strcmp(inp,"undo")==0
			   || strcmp(inp,"remove")==0
			   || strcmp(inp,"hard")==0
			   || strcmp(inp,"easy")==0
			   || strcmp(inp,"post")==0
			   || strcmp(inp,"nopost")==0
			   || strcmp(inp,"analyze")==0
			   || strcmp(inp,"computer")==0
			   || strcmp(inp,"pause")==0
			   || strcmp(inp,"resume")==0
			   ){
			// go ahead
			inputs_to_ignore = 0;
		}
		else if(strcmp(inp,"protover")==0
			   || strcmp(inp,"variant")==0
			   || strcmp(inp,"st")==0
			   || strcmp(inp,"sd")==0
			   || strcmp(inp,"nps")==0
			   || strcmp(inp,"time")==0
			   || strcmp(inp,"otim")==0
			   || strcmp(inp,"usermove")==0
			   || strcmp(inp,"ping")==0
			   || strcmp(inp,"setboard")==0
			   || strcmp(inp,"edit")==0
			   || strcmp(inp,"name")==0
			   || strcmp(inp,"ics")==0
			   || strcmp(inp,"memory")==0
			   || strcmp(inp,"cores")==0
			   || strcmp(inp,"lift")==0
			   || strcmp(inp,"put")==0
			   || strcmp(inp,"hover")==0
			   || strcmp(inp,"option")==0
			   || strcmp(inp,"exclude")==0
			   || strcmp(inp,"include")==0
			   ){
			// ignore next input
			inputs_to_ignore = 1;
		}
		else if(strcmp(inp,"rating")==0
			   || strcmp(inp,"egtpath")==0
			   || strcmp(inp,"setscore")==0){
			inputs_to_ignore = 2;
		}
		else if(strcmp(inp,"level")==0
			   || strcmp(inp,"result")==0) {
			inputs_to_ignore = 3;
		}
		else if(strcmp(inp,"new")==0) {
			current_position = createNewPosition(initboard);
		}
		else if(strcmp(inp,"random")==0) {
			// Set Random Mode
			
		}
		else if(strcmp(inp,"quit")==0) {
			return 0;
		}
		else if(strcmp(inp,"go")==0) {
			current_position = getBestMove_threaded(current_position, depth);
			char* c = current_position->currentMove.coordinates;
			printf("move %c%c%c%c\n", c[0],c[1],c[2],c[3]);
			#ifdef LOGGING
				ch = moveplayed(current_position).coordinates;
				fprintf(logfile, ">>> move %c%c%c%c\n", c[0],c[1],c[2],c[3]);
				log_board(logfile, current_position);
				if(p%2 == 1){
					// White's Turn
					fprintf(logfile,"%2d. %s ... \t\t (%d) \n\n",(p+1)/2, ch, current_position->evaluation);
				}
				else {
				// Black's Turn
				fprintf(logfile,"%2d.    ... %s\t\t (%d)\n\n",p/2, ch,  current_position->evaluation);
				}
			#endif
		}
		else {
			if(strcmp(inp,"e1g1")==0)
				strcpy(inp,"OO");
			if(strcmp(inp,"e1c1")==0)
				strcpy(inp,"OOO");
			if(strcmp(inp,"e8g8")==0)
				strcpy(inp,"oo");
			if(strcmp(inp,"e8c8")==0)
				strcpy(inp,"ooo");
			move m;
			m.coordinates[0] = inp[0];
			m.coordinates[1] = inp[1];
			m.coordinates[2] = inp[2];
			m.coordinates[3] = inp[3];
			position* new_pos = getPositionAfterMove(current_position,m);
			deletePosition(current_position);
			current_position = new_pos;
			#ifdef LOGGING
				ch = moveplayed(current_position).coordinates;
				log_board(logfile, current_position);
				if(p%2 == 1){
					// White's Turn
					fprintf(logfile,"%2d. %s ... \t\t (%d) \n\n",(p+1)/2, ch, current_position->evaluation);
				}
				else {
					// Black's Turn
					fprintf(logfile,"%2d.    ... %s\t\t (%d)\n\n",p/2, ch,  current_position->evaluation);
				}
			#endif
			p++;
			// Run
			current_position = getBestMove_threaded(current_position, depth);
			char* c = current_position->currentMove.coordinates;
			char mv[5];
			mv[0]=c[0];mv[1]=c[1];mv[2]=c[2];mv[3]=c[3];mv[4]='\0';
			if(strcmp(mv,"OO")==0)
				strcpy(mv,"e1g1");
			if(strcmp(mv,"OOO")==0)
				strcpy(mv,"e1c1");
			if(strcmp(mv,"oo")==0)
				strcpy(mv,"e8g8");
			if(strcmp(mv,"ooo")==0)
				strcpy(mv,"e8c8");
			printf("move %s\n", mv);
			#ifdef LOGGING
				ch = moveplayed(current_position).coordinates;
				fprintf(logfile, ">>> move %c%c%c%c\n", c[0],c[1],c[2],c[3]);
				log_board(logfile, current_position);
				if(p%2 == 1){
					// White's Turn
					fprintf(logfile,"%2d. %s ... \t\t (%d) \n\n",(p+1)/2, ch, current_position->evaluation);
				}
				else {
					// Black's Turn
					fprintf(logfile,"%2d.    ... %s\t\t (%d)\n\n",p/2, ch,  current_position->evaluation);
				}
			#endif
			p++;
		}
	}

	deletePosition(current_position);
	return 0;
	#endif

	#ifndef XBOARD
		// Text Mode
		if(argv[1] != '\0')
			printf("LEVEL = %s \n",argv[1]);
		else
			puts("LEVEL = 4 (default)\n");
			// Setup intial position
			position *current_position;
			char initboard[8][8]={"rnbqkbnr","pppppppp","........",
			"........","........","........","PPPPPPPP","RNBQKBNR"};
			current_position = createNewPosition(initboard);

			displayBoard(current_position);

			int p = 1;
			while(isGameRunning(current_position) && p<20){

				int depth = 4;
				if (argc == 2)
					depth = atoi(argv[1]);
				current_position = getBestMove_threaded(current_position, depth);
				char* c = moveplayed(current_position).coordinates;

				displayBoard(current_position);

				if(p%2 == 1){
					printf("%2d. %s ... \t\t (%d) \n\n",(p+1)/2, c, current_position->evaluation);
				}
				else {
					printf("%2d.    ... %s\t\t (%d)\n\n",p/2, c,  current_position->evaluation);
				}

				fflush(stdout);
				p++;
			}

			deletePosition(current_position);
			return 0;
	#endif
}
