#include "search.h"
// Main para execução
int main() {
    string folderPath = "./documentos";
    SearchEngine engine(folderPath);
    engine.run();
    return 0;
}
//Comando para compilar e rodar o código
//g++ main.cpp index.cpp search.cpp -o meu_programa && ./meu_programa