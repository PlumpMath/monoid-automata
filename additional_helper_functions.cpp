
#include <math.h>

#include "structures.hpp"

using namespace std;

vector<char> numberToString(int number) {
    string resultString;
    ostringstream convert;

    resultString = convert.str();

    vector<char> result;
    for (int i = 0; i < resultString.size(); ++i){
        result.push_back(resultString[i]);
    }

    return result;
}

vector<char> reverceVector(vector<char> vec){
    vector<char> res;
    for(int i = vec.size()-1; i >= 0; --i){
        res.push_back(vec[i]);
    }
    return res;
}

int stringToNumber(char str[], int str_size) {
    int resultNumber = 0;
    for (int i=0; i < str_size; ++i) {
        resultNumber += ( (int) (str[str_size - 1 - i] - '0')) * pow(10, i);
        str[i];
    }
	return resultNumber;
}

char* reverceString(char vec[], int vec_size){
    char* res = new char(vec_size);
    for(int i = vec_size-1; i >= 0; --i){
        res[vec_size-1 - i] = vec[i];
    }
    return res;
}
