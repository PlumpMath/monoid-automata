
#include "additional_helper_functions.cpp"

using namespace std;


void destroyAutomata(int autInd){

    if (Aut[autInd].first_finals[0] != NULL){
        free(Aut[autInd].first_finals[0]);
        Aut[autInd].first_finals[0] = NULL;
    }

    if (Aut[autInd].first_finals[1] != NULL){
        free(Aut[autInd].first_finals[1]);
        Aut[autInd].first_finals[1] = NULL;
    }

    Aut[autInd].first_size = 0;
    Aut[autInd].finals_size = 0;

    Aut[autInd].nullable = false;

    return;
}

void addAutomata(int newIndex){
    if (newIndex >= AUT_MEMOERY_SIZE) {
        AUT_MEMOERY_SIZE *= 2;
        Aut.resize(AUT_MEMOERY_SIZE);
    }

    Aut[newIndex].first_finals[0] = (int*)malloc(sizeof(int)*1);
    Aut[newIndex].first_finals[1] = (int*)malloc(sizeof(int)*1);

    Aut[newIndex].first_finals[0][0] = statesCounter;
    Aut[newIndex].first_finals[1][0] = statesCounter;

    Aut[newIndex].first_size = 1;
    Aut[newIndex].finals_size = 1;

    Aut[newIndex].nullable = false;

    ++statesCounter;

    return;
}

void makeUnion(int indexA1, int indexA2, int newIndex){
    if (newIndex >= AUT_MEMOERY_SIZE) {
        AUT_MEMOERY_SIZE *= 2;
        Aut.resize(AUT_MEMOERY_SIZE);
    }

    /*** FIRST ***/
    int* tmpFirst = (int*)realloc(Aut[newIndex].first_finals[0], sizeof(int)*(Aut[indexA1].first_size + Aut[indexA2].first_size));
    if (tmpFirst) {
        Aut[newIndex].first_finals[0] = tmpFirst;
        for (int i = 0; i < Aut[indexA1].first_size; ++i){
            Aut[newIndex].first_finals[0][i] = Aut[indexA1].first_finals[0][i];
        }
        Aut[newIndex].first_size = Aut[indexA1].first_size;
        for (int i = 0; i < Aut[indexA2].first_size; ++i){
            Aut[newIndex].first_finals[0][i + Aut[newIndex].first_size] = Aut[indexA2].first_finals[0][i];
        }
        Aut[newIndex].first_size += Aut[indexA2].first_size;
    }

    /*** FINALS ***/
    int* tmpFinals = (int*)realloc(Aut[newIndex].first_finals[1], sizeof(int)*(Aut[indexA1].first_size + Aut[indexA2].first_size));
    if (tmpFinals) {
    Aut[newIndex].first_finals[1] = tmpFinals;
        for (int i = 0; i < Aut[indexA1].finals_size; ++i){
            Aut[newIndex].first_finals[1][i] = Aut[indexA1].first_finals[1][i];
        }
        Aut[newIndex].finals_size = Aut[indexA1].finals_size;
        for (int i = 0; i < Aut[indexA2].finals_size; ++i){
            Aut[newIndex].first_finals[1][i+Aut[newIndex].finals_size] = Aut[indexA2].first_finals[1][i];
        }
        Aut[newIndex].finals_size += Aut[indexA2].finals_size;
    }

    Aut[newIndex].nullable = (Aut[indexA1].nullable || Aut[indexA2].nullable);

    destroyAutomata(indexA1); destroyAutomata(indexA2);

    return;
}

void makeConcat(int indexA1, int indexA2, int newIndex){
    if (newIndex >= AUT_MEMOERY_SIZE) {
        AUT_MEMOERY_SIZE *= 2;
        Aut.resize(AUT_MEMOERY_SIZE);
    }

    /*** FIRST ***/
    if (Aut[indexA1].nullable) {
        int* tmpFirst = (int*)realloc(Aut[newIndex].first_finals[0], sizeof(int)*(Aut[indexA1].first_size + Aut[indexA2].first_size));
        if (tmpFirst) {
            Aut[newIndex].first_finals[0] = tmpFirst;
            for (int i = 0; i < Aut[indexA1].first_size; ++i){
                Aut[newIndex].first_finals[0][i] = Aut[indexA1].first_finals[0][i];
            }
            Aut[newIndex].first_size = Aut[indexA1].first_size;
            for (int i = 0; i < Aut[indexA2].first_size; ++i){
                Aut[newIndex].first_finals[0][i + Aut[newIndex].first_size] = Aut[indexA2].first_finals[0][i];
            }
            Aut[newIndex].first_size += Aut[indexA2].first_size;
        }
    }
    else {
        int* tmpFirst = (int*)realloc(Aut[newIndex].first_finals[0], sizeof(int)*Aut[indexA1].first_size);
        if (tmpFirst) {
            Aut[newIndex].first_finals[0] = tmpFirst;
            for (int i = 0; i < Aut[indexA1].first_size; ++i){
                Aut[newIndex].first_finals[0][i] = Aut[indexA1].first_finals[0][i];
            }
        }
        Aut[newIndex].first_size = Aut[indexA1].first_size;
    }

    /*** FOLLOWS ***/
    for (int i = 0; i < Aut[indexA1].finals_size; ++i){
        follows[Aut[indexA1].first_finals[1][i]].resize(follows[Aut[indexA1].first_finals[1][i]].size() + Aut[indexA2].first_size);
        for (int j = 0; j < Aut[indexA2].first_size; ++j){
            follows[Aut[indexA1].first_finals[1][i]][j] = Aut[indexA2].first_finals[0][j];
        }
    }

    /*** FINALS ***/
    if (Aut[indexA2].nullable) {
        int* tmpFinals = (int*)realloc(Aut[newIndex].first_finals[1], sizeof(int)*(Aut[indexA1].first_size + Aut[indexA2].first_size));
        if (tmpFinals) {
            Aut[newIndex].first_finals[1] = tmpFinals;
            for (int i = 0; i < Aut[indexA1].finals_size; ++i){
                Aut[newIndex].first_finals[1][i] = Aut[indexA1].first_finals[1][i];
            }
            Aut[newIndex].finals_size = Aut[indexA1].finals_size;
            for (int i = 0; i < Aut[indexA2].finals_size; ++i){
                Aut[newIndex].first_finals[1][i+Aut[newIndex].finals_size] = Aut[indexA2].first_finals[1][i];
            }
            Aut[newIndex].finals_size += Aut[indexA2].finals_size;
        }
    }
    else {
        int* tmpFinals = (int*)realloc(Aut[newIndex].first_finals[1], sizeof(int)*Aut[indexA2].finals_size);
        if (tmpFinals) {
            Aut[newIndex].first_finals[1] = tmpFinals;
            for (int i = 0; i < Aut[indexA2].finals_size; ++i){
                Aut[newIndex].first_finals[1][i] = Aut[indexA2].first_finals[1][i];
            }
            Aut[newIndex].finals_size = Aut[indexA2].finals_size;
        }
    }

    Aut[newIndex].nullable = (Aut[indexA1].nullable && Aut[indexA2].nullable);

    destroyAutomata(indexA1); destroyAutomata(indexA2);

    return;
}

void makeStar(int indexA1, int newIndex){
    if (newIndex >= AUT_MEMOERY_SIZE) {
        AUT_MEMOERY_SIZE *= 2;
        Aut.resize(AUT_MEMOERY_SIZE);
    }

    /*** FOLLOWS ***/
    for (int i = 0; i < Aut[indexA1].finals_size; ++i){
        follows[Aut[indexA1].first_finals[1][i]].resize(follows[Aut[indexA1].first_finals[1][i]].size() + Aut[indexA1].first_size);
        for (int j = 0; j < Aut[indexA1].first_size; ++j){
            follows[Aut[indexA1].first_finals[1][i]][j] = Aut[indexA1].first_finals[0][j];
        }
    }

    /*** FIRST ***/
    int* tmpFirst = (int*)realloc(Aut[newIndex].first_finals[0], sizeof(int)*Aut[indexA1].first_size);
    if (tmpFirst) {
        Aut[newIndex].first_finals[0] = tmpFirst;
        for (int i = 0; i < Aut[indexA1].first_size; ++i){
            Aut[newIndex].first_finals[0][i] = Aut[indexA1].first_finals[0][i];
        }
    }
    Aut[newIndex].first_size = Aut[indexA1].first_size;

    /*** FINALS **/
    int* tmpFinals = (int*)realloc(Aut[newIndex].first_finals[1], sizeof(int)*Aut[indexA1].first_size);
    if (tmpFinals) {
        Aut[newIndex].first_finals[1] = tmpFinals;
        for (int i = 0; i < Aut[indexA1].finals_size; ++i){
            Aut[newIndex].first_finals[1][i] = Aut[indexA1].first_finals[1][i];
        }
        Aut[newIndex].finals_size = Aut[indexA1].finals_size;
    }

    Aut[newIndex].nullable = true;

    destroyAutomata(indexA1);

    return;
}
