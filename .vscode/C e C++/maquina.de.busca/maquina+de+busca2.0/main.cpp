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
//Para rodar o Makefile usar make -f Makefile.mak
/*- meu_projeto
  |- Makefile
  |- src
  |   |- index.cpp
  |   |- search.cpp
  |   |- main.cpp
  |   |- ...
  |
  |- include
  |   |- index.h
  |   |- search.h
  |   |- ...
  |
  |- obj (gerado automaticamente)
  |
  |- bin (gerado automaticamente)
  |
  |- documentos
      |- arquivo1.txt
      |- arquivo2.txt
      |- ...
*/