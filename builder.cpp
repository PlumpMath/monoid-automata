
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

    int i = stringSize - 2;
    bool newLabel = false;
    bool previousWasLabel = false;
    vector<char> label;

    char number[10] = {0,0,0,0,0,0,0,0,0,0};
    int number_size = 0;

    while (i >= 0) {
        if (isdigit(input[i])){
            if (previousWasLabel) {
                createNewAutomataInList(label, number, number_size);
                previousWasLabel = false;
            }

            newLabel = true;
            number[number_size] = input[i];
            ++number_size;

            --i;
            continue;
        }
        if (is_label(input[i])){
            newLabel = true;
            previousWasLabel = true;
            if (!epsilon(input[i])) {
                label.push_back(input[i]);
            }
            --i;
            continue;
        }

        if (newLabel) {
            createNewAutomataInList(label, number, number_size);
            newLabel = false;
            previousWasLabel = false;
        }

        int newIndex = (destroyed.size() > 0) ? destroyed[destroyed.size()-1] : autCounter;
        int prevIndex = maxFreeIndex[maxFreeIndex.size()-1];
        int prevPrevIndex = maxFreeIndex[maxFreeIndex.size()-2];

        if (union_(input[i])){
            bool opOverTwoAutomatas = true;
            makeUnion(prevIndex, prevPrevIndex, newIndex);
            destroyOldAutomatasFromList(newIndex, prevIndex, opOverTwoAutomatas);
        }

        if (star(input[i])){
            bool opOverTwoAutomatas = false;
            makeStar(prevIndex, newIndex);
            destroyOldAutomatasFromList(newIndex, prevIndex, opOverTwoAutomatas);
        }

        if (concat(input[i])){
            bool opOverTwoAutomatas = true;
            makeConcat(prevIndex, prevPrevIndex, newIndex);
            destroyOldAutomatasFromList(newIndex, prevIndex, opOverTwoAutomatas);
        }
        --i;
    }

    /*** CREATE AN ARRAY TO CHECK EASY FINAL STATES***/
    finals_check.resize(statesCounter);
    int st = 0;
    int elements = Aut[lastIndex].finals_size / sizeof(Aut[lastIndex].first_finals[1][0]);
    sort(Aut[lastIndex].first_finals[1], Aut[lastIndex].first_finals[1] + elements);

    for (int j = 0; j < Aut[lastIndex].finals_size; ++j){
        while (st < Aut[lastIndex].first_finals[1][j]){
            finals_check[st] = false;
            ++st;
        }
        finals_check[Aut[lastIndex].first_finals[1][j]] = true;
        ++st;
    }
    while (st < statesCounter){
        finals_check[st] = false;
        ++st;
    }

    if (Aut[lastIndex].first_finals[1] != NULL){
        free(Aut[lastIndex].first_finals[1]);
    }
    /***************************************************/

    return;
}
