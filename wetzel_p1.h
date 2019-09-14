#ifndef CW_WETZEL_P1
#define CW_WETZEL_P1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct state{
	int val;
	char * type;
};
struct transition{
	int start;
	int after;
	char b;
	char a;
	char dir;
};

int * runAutomata(struct state * states[], struct transition * transitions[], char * inputStr, int max, int start, int startInd);
struct transition * findTransition(struct transition * transitions[], int s, char c);
int findStartState(struct state * states[]);
int findStateIndex(struct state * states[], int cs);

#endif
