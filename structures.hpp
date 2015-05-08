#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <queue>
#include <sstream>
#include <iomanip>
#include <locale>
#include <iostream>

using namespace std;

char UNION_SYMBOL = '|';
char STAR_SYMBOL = '*';
char CONCAT_SYMBOL = '.';
char EPSILON_SYMBOL = '#';

/* keeps the max index in the automatas vector, that is free at the moment */
vector<int> maxFreeIndex;

/* keeps a list of the indexes with destroyed automatas */
vector<int> destroyed;


/* increments the indexes for new automatas in the Automata vector */
int autCounter = 0;

/* counts the number of states */
int statesCounter = 0;


/* keeps the last index that the final automata is saved in */
int lastIndex = 0;

int STATES_NUMBER = 200000;

vector<vector<int> > follows(STATES_NUMBER);
vector<vector<char> > labels(STATES_NUMBER);
vector<int> numbers(STATES_NUMBER);
vector<bool> finals_check;


struct Automata{
	int* first_finals[2];
	//int* finals;
	bool nullable;

	int first_size;
	int finals_size;
};


int AUT_MEMOERY_SIZE = 200000;
vector<Automata> Aut(AUT_MEMOERY_SIZE);
