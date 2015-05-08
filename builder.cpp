
#include <algorithm>
#include "build_functions.cpp"

using namespace std;

bool is_label(char c){
    return (!isdigit(c) && c != UNION_SYMBOL && c != STAR_SYMBOL && c != CONCAT_SYMBOL);
}

bool union_(char c){
    return c == UNION_SYMBOL;
}

bool star(char c){
    return c == STAR_SYMBOL;
}

bool concat(char c){
    return c == CONCAT_SYMBOL;
}

bool epsilon(char c){
    return c == EPSILON_SYMBOL;
}


int createNewAutomataInList(vector<char>& label, char number[], int& number_size){
    int newIndex = (destroyed.size() != 0) ? destroyed[destroyed.size()-1] : autCounter;
    lastIndex = newIndex;

	if (statesCounter >= STATES_NUMBER) {
		STATES_NUMBER *= 2;
		follows.resize(STATES_NUMBER);
		labels.resize(STATES_NUMBER);
		numbers.resize(STATES_NUMBER);
	}

    labels[statesCounter] = reverceVector(label);
    numbers[statesCounter] = stringToNumber(reverceString(number, number_size), number_size);
    number_size = 0;

    /*** DEBUG ***/
//    cout << "Label: ";
//    for (int j = 0; j < labels[statesCounter].size(); ++j) {
//        cout << labels[statesCounter][j];
//    }
//    cout << endl;
//    cout << "Number: " << numbers[statesCounter];
//    cout << endl;
    /*************/

    if (destroyed.size() != 0) {
        destroyed.pop_back();
    }
    addAutomata(newIndex);
    maxFreeIndex.push_back(newIndex);
    if (destroyed.size() == 0) ++autCounter;
    label.clear();

    return newIndex;
}

void destroyOldAutomatasFromList(int newIndex, int prevIndex, bool opOverTwoAutomatas = false){

    lastIndex = newIndex;

//    destroyAutomata(prevIndex);
//    if (opOverTwoAutomatas) {
//        destroyAutomata(maxFreeIndex[maxFreeIndex.size()-2]);
//    }

    if (destroyed.size() > 0) {
            destroyed.pop_back();
    }

    destroyed.push_back(prevIndex);
    if (opOverTwoAutomatas) {
        destroyed.push_back(maxFreeIndex[maxFreeIndex.size()-2]);
    }

    maxFreeIndex.pop_back();
    if (opOverTwoAutomatas) {
        maxFreeIndex.pop_back();
    }
    maxFreeIndex.push_back(newIndex);
}

void buildMonoidAutomata(char* input, int stringSize){


    /*** DEBUG ***/
//    cout << "Build Monoid Automata: " << endl;
    /*************/

	int i = stringSize - 2;
    bool newLabel = false;
    bool previousWasLabel = false;
    vector<char> label;
//    vector<char> number;
    char number[10] = {0,0,0,0,0,0,0,0,0,0};
    int number_size = 0;

    while (i >= 0) {

        /*** DEBUG ***/
//        cout << "Current symbol: " << input[i] << endl;
        /*************/

        if (isdigit(input[i])){
            if (previousWasLabel) {
                /*** DEBUG ***/
//                cout << "New Label: " << endl;
                /*************/

                createNewAutomataInList(label, number, number_size);
                previousWasLabel = false;
            }

            newLabel = true;
            number[number_size] = input[i];
            ++number_size;

            --i;
            continue;
        }
        if (is_label(input[i])){// cout << "is label" << endl;
            newLabel = true;
            previousWasLabel = true;
            if (!epsilon(input[i])) {
                label.push_back(input[i]);
            }
            --i;
            continue;
        }

        if (newLabel) {
            /*** DEBUG ***/
//            cout << "New Label: " << endl;
            /*************/

            createNewAutomataInList(label, number, number_size);
            newLabel = false;
            previousWasLabel = false;
        }

        int newIndex = (destroyed.size() > 0) ? destroyed[destroyed.size()-1] : autCounter;
        int prevIndex = maxFreeIndex[maxFreeIndex.size()-1];
        int prevPrevIndex = maxFreeIndex[maxFreeIndex.size()-2];

        /*** DEBUG ***/
//        cout << "New index: " << newIndex << " ; Prev index: " << prevIndex << " ; Prev prev index: " << prevPrevIndex << endl;
        /*************/

        if (union_(input[i])){
            /*** DEBUG ***/
//            cout << "UNION" << endl;
            /*************/

            bool opOverTwoAutomatas = true;
            makeUnion(prevIndex, prevPrevIndex, newIndex);
            destroyOldAutomatasFromList(newIndex, prevIndex, opOverTwoAutomatas);
        }

        if (star(input[i])){
            /*** DEBUG ***/
//            cout << "STAR" << endl;
            /*************/

            bool opOverTwoAutomatas = false;
            makeStar(prevIndex, newIndex);
            destroyOldAutomatasFromList(newIndex, prevIndex, opOverTwoAutomatas);
        }

        if (concat(input[i])){
            /*** DEBUG ***/
//            cout << "CONCAT" << endl;
            /*************/

            bool opOverTwoAutomatas = true;
            makeConcat(prevIndex, prevPrevIndex, newIndex);
            destroyOldAutomatasFromList(newIndex, prevIndex, opOverTwoAutomatas);
        }
		--i;

        /*** DEBUG ***/
//        cout << "Automata: " << endl;
//        cout << "first: " << endl;
//        for (int j = 0; j < Aut[lastIndex].first.size(); ++j) {
//            cout << Aut[lastIndex].first[j] << ", " << endl;
//        }
//        cout << endl;
//        cout << "finals: " << endl;
//        for (int j = 0; j < Aut[lastIndex].finals.size(); ++j) {
//            cout << Aut[lastIndex].finals[j] << ", " << endl;
//        }
//        cout << endl;
//        cout << "nullable: " << Aut[lastIndex].nullable << endl;
//        cout << "state // follow // label // number" << endl;
//        for (int j = 0; j < Aut[lastIndex].first.size(); ++j) {
//            cout << Aut[lastIndex].first[j] << " // ";
//            for (int k = 0; k < follows[Aut[lastIndex].first[j]].size(); ++k) {
//                cout << follows[Aut[lastIndex].first[j]][k] << ", ";
//            }
//            cout << " // ";
//            for (int k = 0; k < labels[Aut[lastIndex].first[j]].size(); ++k) {
//                cout << labels[Aut[lastIndex].first[j]][k];
//            }
//            cout << " // ";
//            cout << numbers[Aut[lastIndex].first[j]] << endl;
//        }
//        cout << endl;
        /*************/
    }

    /*** DEBUG ***/
//    cout << "FIRST:" << endl;
//    for (int h = 0; h < Aut[lastIndex].first_size; ++h){
//        cout << Aut[lastIndex].first_finals[0][h] << endl;
//    }
    /*************/

    /*** CREATE AN ARRAY TO CHECK EASY FINAL STATES***/
    finals_check.resize(statesCounter);
    int st = 0;
    int elements = Aut[lastIndex].finals_size / sizeof(Aut[lastIndex].first_finals[1][0]);
    sort(Aut[lastIndex].first_finals[1], Aut[lastIndex].first_finals[1] + elements);
        //cout << "finals size: " << Aut[lastIndex].finals_size << endl;
    for (int j = 0; j < Aut[lastIndex].finals_size; ++j){
       // cout << "final: " << Aut[lastIndex].first_finals[1][j] << endl;
        while (st < Aut[lastIndex].first_finals[1][j]){
            finals_check[st] = false;
           // cout << "check: " << finals_check[st] << " , st = "<< st << endl;
            ++st;
        }
        finals_check[Aut[lastIndex].first_finals[1][j]] = true;
        //cout << "check: " << finals_check[Aut[lastIndex].first_finals[1][j]] << " , st = "<< Aut[lastIndex].first_finals[1][j] << endl;
        ++st;
    }
    while (st < statesCounter){
        //cout << "end check: " << finals_check[st] << " , st = "<< st << endl;
        finals_check[st] = false;
        ++st;
    }

    if (Aut[lastIndex].first_finals[1] != NULL){
        free(Aut[lastIndex].first_finals[1]);
    }
    /***************************************************/

    return;
}
