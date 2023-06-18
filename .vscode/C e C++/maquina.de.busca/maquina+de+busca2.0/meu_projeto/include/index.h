#ifndef INDEX_H
#define INDEX_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cassert>
#include <dirent.h>
#include <cctype>
#include <locale>
#include <codecvt>

using namespace std;

class Document {// Classe documento
public:
    int id;
    string title;
    string content;

    Document() = default;
    Document(int id, const string& title, const string& content) : id(id), title(title), content(content) {}
};

class InvertedIndex {// Classe índice invertido
public:
    InvertedIndex() : next_id(1) {}
    InvertedIndex(const string& folderPath) {
        load_documents(folderPath);
    }
    vector<pair<int, int>> search(const string& query) const;
    void print_titles(const vector<pair<int, int>>& results, const string& query) const;

private:
    unordered_map<string, unordered_map<int, int>> index;
    unordered_map<int, Document> documents;
    int next_id;

    string normalize(const string& term) const;
    void load_documents(const string& folderPath);
    vector<string> get_file_names(const string& folderPath);
    void add_document(const Document& doc);
    vector<string> tokenize(const string& str) const;
};

#endif  // INDEX_H
