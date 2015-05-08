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
//cout << "compareWord: ";
//cout << "wordIndex: " << wordIndex << "; label / word: ";
    for (int i = 0; i < label.size(); ++i) {
        //cout << label[i] << " / " << word[wordIndex + i];
        if (label[i] != word[wordIndex + i]) {
        //    cout << "----------------" << endl;
            return false;
        }
    }
   // cout << "----------------" << endl;
    return true;
}

bool endOfWordMatch(vector<char> label, vector<char> word, int wordIndex){
//cout << "endOfWordMatch: ";
//cout << "wordIndex: " << wordIndex << " , label size: " << label.size() << " , word size: " << word.size() << "; label / word: ";
    if (label.size() == word.size() - wordIndex) {
        for (int i = 0; i < label.size(); ++i) {
       // cout << " , " << label[i] << " / " << word[wordIndex + i];
            if (label[i] != word[wordIndex + i]) {
            //    cout << "-------- false --------" << endl;
                return false;
            }
        }
        //cout << "-------- true --------" << endl;
        return true;
    }
    //cout << "-------- false --------" << endl;
    return false;
}

vector<int> traverseAndSumWord(vector<char> word){

    vector<int> res;
    vector<int> states;
    vector<int> subwords;
    vector<int> sums;

    int states_subwords_sums[statesCounter*3];
    int top = 0;

    for (int j = 0; j < Aut[lastIndex].first_size; ++j) {
//        cout << Aut[lastIndex].first[j] << ";  ";
//        for (int k = 0; k < labels[Aut[lastIndex].first[j]].size(); ++k) {
//            cout << labels[Aut[lastIndex].first[j]][k];
//        }
//        cout << endl;
        if(isFinal(Aut[lastIndex].first_finals[0][j]) && endOfWordMatch(labels[Aut[lastIndex].first_finals[0][j]], word, 0)){
            int sum = numbers[Aut[lastIndex].first_finals[0][j]];
            res.push_back(sum);
        }
        else {
            states_subwords_sums[top] = Aut[lastIndex].first_finals[0][j];
            states_subwords_sums[top+1] = 0;
            states_subwords_sums[top+2] = numbers[Aut[lastIndex].first_finals[0][j]];
            ++top;
        }
    }

    while(!states.empty()){
        int k = states_subwords_sums[top];
        int sum = states_subwords_sums[top+1];
        int subwordIndex = states_subwords_sums[top+2];
        --top;

//        cout << "k = " << k << ", sum = " << sum << ", subwordIndex = " << subwordIndex << endl;

//        cout << "final(k) : " << final(k) << endl;
        if(isFinal(k) && endOfWordMatch(labels[k], word, subwordIndex)){ /**TODO: Optimization: also check if the state is not first. **/
            res.push_back(sum);
            continue;
        }

        if (compareWord(labels[k], word, subwordIndex)){
            for (int i = 0; i < follows[k].size(); ++i){
                states_subwords_sums[top] = follows[k][i];
                states_subwords_sums[top+1] = subwordIndex + labels[k].size();
                states_subwords_sums[top+2] = sum + numbers[follows[k][i]];
                ++top;
            }
        }
    }
    return res;
}
