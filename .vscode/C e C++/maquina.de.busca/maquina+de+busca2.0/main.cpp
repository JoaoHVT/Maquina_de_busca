#include "search.h"

int main() {
    string folderPath = "./documentos";
    SearchEngine engine(folderPath);
    engine.run();
    return 0;
}

//g++ main.cpp index.cpp search.cpp -o meu_programa && ./meu_programa

