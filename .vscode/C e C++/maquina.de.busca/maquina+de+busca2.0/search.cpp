#include "search.h"

void SearchEngine::run() {
    string query;
    cout << "Enter a search query (type 'quit' to exit): ";
    getline(cin, query);
    while (query != "quit") {
        vector<pair<int, int>> results = index.search(query);
        index.print_titles(results, query);
        cout << "Enter a search query (type 'quit' to exit): ";
        getline(cin, query);
    }
}
