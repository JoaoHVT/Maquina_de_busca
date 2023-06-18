#include "search.h"
// Função que realiza a busca e retorna os docs da pesquisa
void SearchEngine::run() {
    string query;
    cout << "Enter a search query (type 'quit' to exit): ";
    getline(cin, query);
    while (query != "quit") {
        // Realiza a busca no índice invertido com base na consulta
        vector<pair<int, int>> results = index.search(query);
        // Imprime os títulos dos documentos encontrados na busca
        index.print_titles(results, query);
        // Solicita uma nova consulta ao usuário
        cout << "Enter a search query (type 'quit' to exit): ";
        getline(cin, query);
    }
}
