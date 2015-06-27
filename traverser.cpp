#include <stack>
#include <deque>

#include "builder.cpp"

#include <unistd.h>
using namespace std;

bool isFinal(int state){
    return finals_check[state];
}

bool compareWord(vector<char> label, vector<char> word, int wordIndex){
    if (label.size() > word.size() - wordIndex) {
        return false;
    }
    for (int i = 0; i < label.size(); ++i) {
        if (label[i] != word[wordIndex + i]) {
            return false;
        }
    }
    return true;
}

bool endOfWordMatch(vector<char> label, vector<char> word, int wordIndex){
    if (label.size() == word.size() - wordIndex) {
        for (int i = 0; i < label.size(); ++i) {
            if (label[i] != word[wordIndex + i]) {
                return false;
            }
        }
        return true;
    }
    return false;
}

vector<int> traverseAndSumWord(vector<char> word){
    vector<int> res;

    int states_subwords_sums[statesCounter*10];
    int top = -3;
    int topTest = -1;

    for (int j = 0; j < Aut[lastIndex].first_size; ++j) {
        if(isFinal(Aut[lastIndex].first_finals[0][j]) && endOfWordMatch(labels[Aut[lastIndex].first_finals[0][j]], word, 0)){
            int sum = numbers[Aut[lastIndex].first_finals[0][j]];
            res.push_back(sum);
        }
        else {
            top += 3;
            states_subwords_sums[top] = Aut[lastIndex].first_finals[0][j];
            states_subwords_sums[top+1] = 0;
            states_subwords_sums[top+2] = numbers[Aut[lastIndex].first_finals[0][j]];
            ++topTest;
        }
    }

    while(topTest >= 0){
        int k = states_subwords_sums[top];
        int subwordIndex = states_subwords_sums[top+1];
        int sum = states_subwords_sums[top+2];
        top -= 3;
        --topTest;

        if(isFinal(k) && endOfWordMatch(labels[k], word, subwordIndex)){ /**TODO: Optimization: also check if the state is not first. **/
            res.push_back(sum);
            continue;
        }

        if (compareWord(labels[k], word, subwordIndex)){
            for (int i = 0; i < follows[k].size(); ++i){
                top += 3;
                states_subwords_sums[top] = follows[k][i];
                states_subwords_sums[top+1] = subwordIndex + labels[k].size();
                states_subwords_sums[top+2] = sum + numbers[follows[k][i]];
                ++topTest;
            }
        }
    }
    return res;
}
