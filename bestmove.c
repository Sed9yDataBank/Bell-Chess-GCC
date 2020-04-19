#include "common.h"

typedef struct thd_args
{
	int threadno;
	position* initialPos;
	int plyDepth;
} thd_args;

bool opening = true;

// Are We Still In The Opening?
// Can We Still Use The Opening Book?
move movehistory[10] = {};
move openingBookMoves[500][10] = {};

void loadOpeningBook(char* filename){
	srand(time(NULL));
	FILE* openingBook = fopen(filename,"r");
	int i = 0, j = 0;
	char str[10];
	while(fscanf(openingBook,"%s",str)!=EOF){
		move m;
		m.coordinates[0]=str[0];
		m.coordinates[1]=str[1];
		m.coordinates[2]=str[2];
		m.coordinates[3]=str[3];
		openingBookMoves[j][i] = m;
		i++;
		if(strcmp(str,"END")==0){
			i = 0; j++;
		}
	}
}

position* getOpeningFromBook(position* initialPos, int plyDepth){
	move ret; ret.move = 0;
	move possibleMoves[100]; int l_posmoves = 0;
	for (int i = 0; i < 50; ++i){
		move* openingLine = openingBookMoves[i];
		bool discard = false;
		for (int j = 0; j<initialPos->moveno-1; ++j){
			if (movehistory[j].move != openingLine[j].move){
				discard = true;
				break;
			}
		}
		if(!discard){
			if(strcmp(openingLine[initialPos->moveno-1].coordinates,"END")==0){
				opening = false;
				return getBestMove_threaded(initialPos, plyDepth);
			} else if(openingLine[initialPos->moveno-1].move == 0) {
				break;
			} else {
				possibleMoves[l_posmoves] = openingLine[initialPos->moveno-1];
				++ l_posmoves;
			}
		}
	}
	float r = (int) ( ((float) rand()/RAND_MAX) * l_posmoves );
	printf("DEBUG:%f\n",r);
	ret = possibleMoves[(int)r];
	if(ret.move != 0){
		movehistory[initialPos->moveno-1] = ret;
	}
	else{
		opening = false;
		return getBestMove_threaded(initialPos, plyDepth);
	}
	return getPositionAfterMove(initialPos,ret);
}

void* singleThread(void* args){
	thd_args* a = (thd_args*) args;
	position* p = a->initialPos;
	int d = a->plyDepth;
	position* z = getBestMove(p,d);
	#ifdef DEBUG
	printf("called getBestMove(%p,%d)\n",p,d);
	#endif
	pthread_exit(z);
}

position* getBestMove_threaded(position* initialPos, int plyDepth){
	if (opening){
		return getOpeningFromBook(initialPos, plyDepth);
	}
	// this fxn Must Be Run At Top Level
	// i.e. initialPos Is Not Ended And plydepth>1
	move* movelist = possibleNextMoves(initialPos);
	int i=0;
	move x=movelist[i];
	position** nodesList = malloc(MAXMOVES*sizeof(position));
	int noMoves = expandnode(nodesList,movelist,initialPos);

	// Create Threads And Recurse
	// MAIN CODE BEGINS HERE
	pthread_t *threads = malloc(noMoves*sizeof(pthread_t));
	thd_args* args = malloc(noMoves*sizeof(thd_args));
	position** returnList = malloc(noMoves*sizeof(position*));
	for (int i = 0; i < noMoves; ++i)
	{
		args[i].initialPos = nodesList[i];
		args[i].plyDepth = plyDepth;
		args[i].threadno = i;
		#ifdef DEBUG
		printf("Calling Singlethread On Args[%d] With %p \n", i, initialPos);
		#endif
		if(pthread_create(threads+i,NULL,singleThread,&args[i])){
			printf("Error In Creating Thread\n");
		}
	}
	for (int i = 0; i < noMoves; ++i)
	{
		pthread_join(threads[i],(void**)&returnList[i]);
		position* best_eval = returnList[i];
		nodesList[i]->evaluation = best_eval->evaluation;
		deletePosition(best_eval);
	}

	// Main Code Ends Here
	// Choose Best Move And Return
	position* best_position = findminormax(initialPos, nodesList);
	position* ret = createNewPosition(best_position->board);
	*ret = *best_position;
	deleteAllNodes(nodesList);
	free(nodesList);
	free(movelist);
	return ret;
}

position* getBestMove(position* initialPos, int plyDepth){
	if(!isGameRunning(initialPos)){
		return evaluation_at_end(initialPos);
	}

	// First Of All Find Out All Possible Moves
	move* movelist = possibleNextMoves(initialPos);

	// Then Find What Will Be The Position After These Moves
	position** nodesList = malloc(MAXMOVES*sizeof(position));

	// Todo Later Replace 50 By The Actual Number
	// Write A Function To Simply Count No. Of Legal Moves
	expandnode(nodesList,movelist,initialPos);

	// Now Evaluate All The Positions ...
	int i;
	position* pos;
	if(plyDepth==1){
		// Direct Evaluation
		for(i=0;nodesList[i]!=NULL;i++){
			pos = nodesList[i];
			pos->evaluation = evaluate(pos);
		}
	} else {
		for(i=0;nodesList[i]!=NULL;i++){
			// Recurse
			position* best_eval = getBestMove(nodesList[i],
					plyDepth-1);
			nodesList[i]->evaluation = best_eval->evaluation;
			deletePosition(best_eval);
		}
	}

	// ... And Now Choose The Best One According To The Turn
	position* best_position = findminormax(initialPos, nodesList);

	// ... Free Nodeslist ... But Copy Bestmove First
	position* ret = createNewPosition(best_position->board);
	*ret = *best_position;

	deleteAllNodes(nodesList);
	free(nodesList);
	free(movelist);

	// Return Best Position

	#ifdef DEBUG
		if(plyDepth==4)
			;printf("Returning Position %p With Eval = %d \n", ret, ret->evaluation);
	#endif

	return ret;
}
