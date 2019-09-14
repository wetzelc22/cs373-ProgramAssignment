#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "wetzel_p1.h"
#include <stdbool.h>

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



int * runAutomata(struct state * states[], struct transition * transitions[], char * inputStr, int max, int start, int startInd){
		char accept[] = "accept";
		char reject[] = "reject";
		char * temp = inputStr;
		int tranTrack = 1;
		int ptr = 0;
		int cs = start;
		int csIndex = startInd;
		int * stateTrack = (int *)malloc(2 * sizeof(int));
		stateTrack[0] = 0;
		stateTrack[1] = start;
		while(tranTrack < (max+1)){
			tranTrack++;
			char cur = inputStr[ptr];
			struct transition * t = findTransition(transitions, cs, cur);
			temp[ptr] = t->a;
			cs = t->after;
			stateTrack = realloc(stateTrack, (tranTrack+1)*sizeof(int));
			stateTrack[tranTrack] = cs;
			csIndex = findStateIndex(states, cs);
			int check = strcmp(accept, states[csIndex]->type);
			if(check == 0){
				stateTrack[0] = 1;
				break;
			}
			check = strcmp(reject, states[csIndex]->type);
			if(check == 0){
				stateTrack[0] = 2;
				break;
			}
			if(t->dir == 'R'){
				ptr++;
			}else{
				ptr--;
			}
		}
		free(temp);
		return stateTrack;

}

struct transition *findTransition(struct transition * transitions[], int s, char c){
	printf("in findTran");
	struct transition * ret;
	int run = 0;
	int i = 0;
	while(run == 0){
		if(transitions[i]->start == s && transitions[i]->b == c){
			ret = transitions[i];
			run = 1;
		}
	}
	return ret;	
}

int findStartState(struct state * states[]){
	printf("In findStartState");	
	bool b = true;
	int i = 0;
	char start[] = "start";
	while(b){
		int check = strcmp(start, states[i]->type);
		if(check == 0){
			b = false;
		}else{
			i++;
		}
	}
	return i;
}
int findStateIndex(struct state * states[], int cs){
	printf("in findStateIndex");
	bool b = true;
	int i = 0;
	while(b){
		if(states[i]->val == cs){
			b = false;
		}else{
			i++;
		}
	}
	return i;

}
int main(int argc, char **argv){
	FILE *fptr;
	char line[128];
	int length = sizeof(argv[2]);
	char inputStr[length];
	strcpy(inputStr, argv[2]);
	int max = atoi(argv[3]);
	int stateInd = 0;
	int tranInd = 0;
	fptr = fopen(argv[1], "r");
	struct state * states[1000];
	struct transition *transitions[100000];
	while(fgets(line, sizeof(line), fptr) != NULL){
		int count = 0;
		char *s = strtok(line, "\t");
		char *state = "state";
		int check = strcmp(s, state);
		if(check == 0){
			struct state * st = malloc(sizeof(struct state));
			while(s){
				if(count == 1){
					int num = atoi(s);
					st->val = num;
					count++;
					s = strtok(NULL, "\t");
				}else if(count == 2){
					st->type = strdup(s);
					s = strtok(NULL, "\t");
				}else{
					s = strtok(NULL, "\t");
					count++;
				}		
			}
			states[stateInd] = st;
			stateInd++;
		}else{
			struct transition *t = malloc(sizeof(struct transition));
			while(s){
				if(count ==1){
					//printf("1: %s\n", s);
					count++;
					int i = atoi(s);
					t->start = i;
					s = strtok(NULL, "\t");
				}else if(count == 2){
					//printf("2: %s\n", s);
					count++;
					char c = s[0];
					t->b = c;
					s = strtok(NULL, "\t");
				}else if(count == 3){
					//printf("3: %s\n", s);
					count++;
					int n = atoi(s);
					t->after = n;
					s = strtok(NULL, "\t");
				}else if(count == 4){
					//printf("4: %s\n", s);
					count++;
					char c = s[0];
					t->a = c;
					s = strtok(NULL, "\t");
				}else if(count == 5){
					//printf("5: %s\n", s);
					char dir = s[0];
					t->dir = dir;
					s = strtok(NULL, "\t");
				}else{
					count++;
					s = strtok(NULL, "\t");
				}
		
			}
			printf("tranInd: %d", tranInd);
			transitions[tranInd] = t;
			tranInd++;
			int checkB = 0;
			int checkA = 0;
			for(int i = 0; i < (stateInd); i++){
				if(states[i]->val == t->start){checkB = 1;}
				if(states[i]->val == t->after){checkA = 1;}
			}
			if(checkB == 0){
				struct state * st = malloc(sizeof(struct state));
				st->val = t->start;
				st->type = NULL;
				states[stateInd] = st;
				stateInd++;
				printf("check here\n");
			}
			if(checkA == 0){
				struct state * st = malloc(sizeof(struct state));
				st->val = t->after;
				st->type = NULL;
				states[stateInd] = st;
				stateInd++;
			}
		}
	}
	fclose(fptr);
	int startIndex = findStartState(states);
	int startStateVal = states[startIndex]->val;
	int * arr = runAutomata(states, transitions, inputStr, max, startStateVal, startIndex);
	int siz = sizeof(arr)/4;
	for(int i = 1; i < siz; i++){
		if(i < (siz-1)){
			printf("%c->", arr[i]);
		}else{
			if(arr[0] == 0){
				printf("%d quit",arr[i]);
			}else if(arr[0] == 1){
				printf("%d accept",arr[i]);
			}else{
				printf("%d reject", arr[i]);
			}
		}
	}
	return 0;
}
