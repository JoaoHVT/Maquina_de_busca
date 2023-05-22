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

class Document {
public:
    int id;
    string title;
    string content;

    Document() = default;
    Document(int id, const string& title, const string& content) : id(id), title(title), content(content) {}
};

class InvertedIndex {
public:
    unordered_map<string, unordered_map<int, int>> index;
    unordered_map<int, Document> documents;
    int next_id;

    InvertedIndex() : next_id(1) {}
    InvertedIndex(const string& folderPath) {
        load_documents(folderPath);
    }

string normalize(const string& term) const {
    static const vector<pair<string, char>> accent_pairs = {
    {u8"\u00E1", 'a'}, {u8"\u00E0", 'a'}, {u8"\u00E2", 'a'}, {u8"\u00E3", 'a'}, {u8"\u00E4", 'a'},
    {u8"\u00C1", 'A'}, {u8"\u00C0", 'A'}, {u8"\u00C2", 'A'}, {u8"\u00C3", 'A'}, {u8"\u00C4", 'A'},
    {u8"\u00E9", 'e'}, {u8"\u00E8", 'e'}, {u8"\u00EA", 'e'}, {u8"\u00EB", 'e'},
    {u8"\u00C9", 'E'}, {u8"\u00C8", 'E'}, {u8"\u00CA", 'E'}, {u8"\u00CB", 'E'},
    {u8"\u00ED", 'i'}, {u8"\u00EC", 'i'}, {u8"\u00EE", 'i'}, {u8"\u00EF", 'i'},
    {u8"\u00CD", 'I'}, {u8"\u00CC", 'I'}, {u8"\u00CE", 'I'}, {u8"\u00CF", 'I'},
    {u8"\u00F3", 'o'}, {u8"\u00F2", 'o'}, {u8"\u00F4", 'o'}, {u8"\u00F5", 'o'}, {u8"\u00F6", 'o'},
    {u8"\u00D3", 'O'}, {u8"\u00D2", 'O'}, {u8"\u00D4", 'O'}, {u8"\u00D5", 'O'}, {u8"\u00D6", 'O'},
    {u8"\u00FA", 'u'}, {u8"\u00F9", 'u'}, {u8"\u00FB", 'u'}, {u8"\u00FC", 'u'},
    {u8"\u00DA", 'U'}, {u8"\u00D9", 'U'}, {u8"\u00DB", 'U'}, {u8"\u00DC", 'U'},
    {u8"\u00E7", 'c'}, {u8"\u00C7", 'C'}
};

    string normalized_term = term;

    // Convert accents
    for (auto& pair : accent_pairs) {
        size_t pos = normalized_term.find(pair.first);
        while (pos != string::npos) {
            normalized_term.replace(pos, pair.first.length(), 1, pair.second);
            pos = normalized_term.find(pair.first, pos + 1);
        }
    }

    // Remove numbers
    normalized_term.erase(remove_if(normalized_term.begin(), normalized_term.end(),
                                    [](unsigned char c) { return isdigit(c); }),
                          normalized_term.end());

    // Remover non-alphanumeric characters
    normalized_term.erase(remove_if(normalized_term.begin(), normalized_term.end(),
                                    [](unsigned char c) { return !isalnum(c); }),
                          normalized_term.end());
    // Minuscula
    transform(normalized_term.begin(), normalized_term.end(), normalized_term.begin(),
              [](unsigned char c) { return tolower(c); });

    return normalized_term;
}

    void load_documents(const string& folderPath) {
        vector<string> fileNames = get_file_names(folderPath);
        for (const auto& fileName : fileNames) {
            string filePath = folderPath + "/" + fileName;
            ifstream file(filePath);
            if (file.is_open()) {
                string title = fileName;
                string content;
                string line;
                while (getline(file, line)) {
                    content += line + "\n";
                }
                file.close();
                add_document(Document(next_id, title, content));
                next_id++;
            } else {
                cout << "Failed to open the file: " << fileName << endl;
            }
        }
    }

    vector<string> get_file_names(const string& folderPath) {
        vector<string> fileNames;
        DIR* directory;
        dirent* entry;
        if ((directory = opendir(folderPath.c_str())) != nullptr) {
            while ((entry = readdir(directory)) != nullptr) {
                if (entry->d_type == DT_REG) {  // Regular file
                    fileNames.push_back(entry->d_name);
                }
            }
            closedir(directory);
        } else {
            cout << "Failed to open the directory: " << folderPath << endl;
        }
        return fileNames;
    }

    void add_document(const Document& doc) {
        documents[doc.id] = doc;
        vector<string> terms = tokenize(doc.title + " " + doc.content);
        unordered_set<string> unique_terms;
        for (const auto& term : terms) {
            string normalized_term = normalize(term);
            if (!unique_terms.count(normalized_term)) {
                index[normalized_term][doc.id] = 0;
                unique_terms.insert(normalized_term);
            }
            index[normalized_term][doc.id]++;
        }
    }

    vector<string> tokenize(const string& str) const {
        vector<string> tokens;
        string token;
        istringstream iss(str);
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

    vector<pair<int, int>> search(const string& query) const {
        vector<string> terms = tokenize(query);
        unordered_map<int, int> document_scores;
        for (const auto& term : terms) {
            string normalized_term = normalize(term);
            if (index.count(normalized_term)) {
                const unordered_map<int, int>& term_postings = index.at(normalized_term);
                for (const auto& posting : term_postings) {
                    document_scores[posting.first] += posting.second;
                }
            }
        }
        vector<pair<int, int>> sorted_results(document_scores.begin(), document_scores.end());
        sort(sorted_results.begin(), sorted_results.end(), [this](const auto& a, const auto& b) {
            if (a.second == b.second) {
                const Document& docA = documents.at(a.first);
                const Document& docB = documents.at(b.first);
                return docA.title < docB.title;
            }
            return a.second > b.second;
        });
        return sorted_results;
    }

    void print_titles(const vector<pair<int, int>>& results, const string& query) const {
        if (results.empty()) {
            cout << "No documents found for the search query '" << query << "'." << endl;
            return;
        }

        cout << "Search Results for query '" << query << "':" << endl;
        for (const auto& result : results) {
            int id = result.first;
            if (documents.count(id)) {
                const Document& doc = documents.at(id);
                cout << "Title: " << doc.title << " | Frequency: " << result.second << endl;
            }
        }
        cout << endl;
    }
};
/*
int main() {
    string folderPath = "./documents.txt";
    InvertedIndex index(folderPath);

    string query;
    while (true) {
        cout << "Enter a search query (or 'exit' to quit): ";
        getline(cin, query);
        if (query == "exit") {
            break;
        }
        vector<pair<int, int>> result = index.search(query);
        index.print_titles(result, query);
    }

    return 0;
}*/