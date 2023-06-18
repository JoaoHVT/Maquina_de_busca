#include "search.h"
// Main para execução
int main() {
    string folderPath = "./documentos";
    SearchEngine engine(folderPath);
    engine.run();
    return 0;
}
/*Comando para compilar e rodar o código:
g++ main.cpp index.cpp search.cpp -o meu_programa && ./meu_programa
Para rodar o Makefile, usar "make" (compilar) e "make run" (rodar)
Estrutura do código:
- meu_projeto
  |- Makefile
  |- src
  |   |- index.cpp
  |   |- search.cpp
  |   |- main.cpp
  |
  |- include
  |   |- index.h
  |   |- search.h
  |
  |- obj (gerado automaticamente)
  |
  |- bin (gerado automaticamente)
  |
  |- documentos
      |- ......txt
*/
