#ifndef SEARCH_H
#define SEARCH_H

#include <iostream>
#include <string>
#include "index.h"

using namespace std;

class SearchEngine {// Classe Search responsável pela interação com o usuário
public:
    InvertedIndex index;

    SearchEngine(const string& folderPath) : index(folderPath) {}

    void run();
};

#endif  // SEARCH_H
