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


int * runAutomata(struct state * states[], struct transition * transitions[], char * inputStr, int max, int start, int startInd, int stateLen, int tranLen);
struct transition * findTransition(struct transition * transitions[], int s, char c, int len);
int findStartState(struct state * states[], int len);
int findStateIndex(struct state * states[], int cs, int stateLen);



int * runAutomata(struct state * states[], struct transition * transitions[], char * inputStr, int max, int start, int startInd, int stateLen, int tranLen){
		char accept[] = "accept\n";
		char reject[] = "reject\n";
		char * temp = inputStr;
		int tranTrack = 2;
		int ptr = 0;
		int cs = states[startInd]->val;
		int csIndex = startInd;
		int * stateTrack = (int *)malloc(3 * sizeof(int));
		stateTrack[0] = tranTrack;
		stateTrack[1] = 0;
		stateTrack[2] = start;
		while(tranTrack < (max+2)){
			tranTrack++;
			char cur = inputStr[ptr];
			struct transition * t = findTransition(transitions, cs, cur, tranLen);
			temp[ptr] = t->a;
			cs = t->after;
			stateTrack = realloc(stateTrack, (tranTrack+1)*sizeof(int));
			stateTrack[tranTrack] = cs;
			csIndex = findStateIndex(states, cs, stateLen);
			int check = strcmp(accept, states[csIndex]->type);
			if(check == 0){
				stateTrack[1] = 1;
				break;
			}
			check = strcmp(reject, states[csIndex]->type);
			if(check == 0){
				stateTrack[1] = 2;
				break;
			}
			if(t->dir == 'R'){
				ptr++;
			}else{
				ptr--;
			}
		}
		stateTrack[0] = tranTrack;
		return stateTrack;

}

struct transition *findTransition(struct transition * transitions[], int s, char c, int len){
	struct transition * ret = malloc(sizeof(struct transition));
	int i = 0;
	for(i = 0; i < len; i++){
		if(transitions[i]->start == s && transitions[i]->b == c){
			ret = transitions[i];
			break;
		}
	}
	return ret;	
}

int findStartState(struct state * states[], int len){	
	int i = 0;
	char start[] = "start\n";
	for(i = 0; i < len; i++){
		int check = strcmp(start, states[i]->type);
		if(check == 0){
			break;
		}
	}
	return i;
}
int findStateIndex(struct state * states[], int cs, int stateLen){
	int i = 0;
	for(i=0;i < stateLen; i++){
		if(states[i]->val == cs){
			break;
		}
	}
	return i;

}
int main(int argc, char **argv){
	FILE *fptr;
	char line[128];
	int length = 1000;
	char inputStr[length];
	strcpy(inputStr, argv[2]);
	int max = atoi(argv[3]);
	int stateInd = 0;
	int tranInd = 0;
	fptr = fopen(argv[1], "r");
	struct state * states[1000];
	struct transition *transitions[100000];
	while(fgets(line, sizeof(line), fptr)){
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
					count++;
					int i = atoi(s);
					t->start = i;
					s = strtok(NULL, "\t");
				}else if(count == 2){
					count++;
					char c = s[0];
					t->b = c;
					s = strtok(NULL, "\t");
				}else if(count == 3){
					count++;
					int n = atoi(s);
					t->after = n;
					s = strtok(NULL, "\t");
				}else if(count == 4){
					count++;
					char c = s[0];
					t->a = c;
					s = strtok(NULL, "\t");
				}else if(count == 5){
					char dir = s[0];
					t->dir = dir;
					s = strtok(NULL, "\t");
				}else{
					count++;
					s = strtok(NULL, "\t");
				}
		
			}
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
				st->type = "none";
				states[stateInd] = st;
				stateInd++;
			}
			if(checkA == 0){
				struct state * st = malloc(sizeof(struct state));
				st->val = t->after;
				st->type = "none";
				states[stateInd] = st;
				stateInd++;
			}
		}
	}
	fclose(fptr);
	int startIndex = findStartState(states, stateInd-1);
	int startStateVal = states[startIndex]->val;
	int * arr = runAutomata(states, transitions, inputStr, max, startStateVal, startIndex, stateInd-1, tranInd);
	int len = arr[0];
	for(int i = 2; i < len+1; i++){
		if(i < (len)){
			printf("%d->", arr[i]);
		}else{
			if(arr[1] == 0){
				printf("%d quit\n",arr[i]);
			}else if(arr[1] == 1){
				printf("%d accept\n",arr[i]);
			}else{
				printf("%d reject\n", arr[i]);
			}
		}
	}
	free(arr);
	for(int i = 0; i<stateInd; i++){
		//free(states[i]->type);
		free(&states[i]);
	}
	for(int i = 0; i<tranInd; i++){
		free(&transitions[i]);
	}
	return 0;
}
