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

// Classe documento
class Document {
public:
    int id;
    string title;
    string content;

    Document() = default;
    Document(int id, const string& title, const string& content) : id(id), title(title), content(content) {}
};
// Classe índice invertido
class InvertedIndex {
public:
    unordered_map<string, unordered_map<int, int>> index;
    unordered_map<int, Document> documents;
    int next_id;

    InvertedIndex() : next_id(1) {}
    InvertedIndex(const string& folderPath) {
        load_documents(folderPath);
    }
    // Função para normalizar um termo (converter letras para minúsculas, remover caracteres não alfanuméricos e manter os espaços)
    string removeAccents(const string& term) const{
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

    string term_without_accents = term;

    for (auto& pair : accent_pairs) {
        size_t pos = term_without_accents.find(pair.first);
        while (pos != string::npos) {
            term_without_accents.replace(pos, pair.first.length(), 1, pair.second);
            pos = term_without_accents.find(pair.first, pos + 1);
        }   
    }
    return term_without_accents;
}

string normalize(const string& term) const {
    string normalized_term = removeAccents(term);

    // Remover números
    normalized_term.erase(remove_if(normalized_term.begin(), normalized_term.end(),
                                [](unsigned char c) { return isdigit(c); }),
                      normalized_term.end());

    // Remover caracteres não alfanuméricos, exceto espaços
    normalized_term.erase(remove_if(normalized_term.begin(), normalized_term.end(),
                                [](unsigned char c) { return !isalnum(c) && !isspace(c); }),
                      normalized_term.end());

    // Transforma em minúscula
    transform(normalized_term.begin(), normalized_term.end(), normalized_term.begin(),
              [](unsigned char c) { return tolower(c); });

    return normalized_term;
}



    // Função para carregar os documentos de um diretório
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
    // Função para obter os nomes dos arquivos em um diretório
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
    // Função para adicionar um documento ao índice invertido
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
    // Função para tokenizar uma string em palavras separadas
    vector<string> tokenize(const string& str) const {
        vector<string> tokens;
        string token;
        istringstream iss(str);
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }
    // Função para realizar uma busca no índice invertido com base em uma consulta (apenas documentos que contêm todos os termos)
// (com ordem de relevância)
vector<pair<int, int>> search(const string& query) const {
    string normalized_query = normalize(query);
    vector<string> terms = tokenize(normalized_query);
    unordered_map<int, int> document_scores;
    bool firstTerm = true; // Flag para indicar o primeiro termo da consulta

    for (const auto& term : terms) {
        if (index.count(term)) {
            const unordered_map<int, int>& term_postings = index.at(term);
            if (firstTerm) {
                // Se for o primeiro termo da consulta, inicializa a contagem com as ocorrências do termo
                for (const auto& posting : term_postings) {
                    document_scores[posting.first] = posting.second;
                }
                firstTerm = false;
            } else {
                // Para os demais termos, realiza a interseção com os documentos que já estão na contagem
                for (auto it = document_scores.begin(); it != document_scores.end(); ) {
                    int document_id = it->first;
                    if (term_postings.count(document_id)) {
                        it->second += term_postings.at(document_id);
                        ++it;
                    } else {
                        it = document_scores.erase(it);
                    }
                }
            }
        } else {
            // Se algum termo não estiver presente no índice, retorna uma lista vazia
            return {};
        }
    }

    // Ordena por relevância e critério de desempate
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

    // Função para imprimir os títulos dos documentos encontrados em uma busca
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
    // Função para imprimir o conteúdo normalizado de todos os documentos (apenas para testes)
    /*void print_normalized_content() const {
    cout << "Normalized Content of Documents:" << endl;
    for (const auto& doc : documents) {
        cout << "Document ID: " << doc.first << endl;
        cout << "Normalized Title: " << normalize(doc.second.title) << endl;
        cout << "Normalized Content: " << normalize(doc.second.content) << endl;
        cout << endl;
    }
}*/
};
// Main para execução de buscas interativas
int main() {
    string folderPath = "./documentos";
    InvertedIndex index(folderPath);

    /*index.print_normalized_content();//(apenas teste)*/

    string query;
    while (true) {
        cout << "Enter a search query (or 'exit' to quit): ";
        getline(cin, query);
        if (query == "exit") {
            break;
        }
        string normalized_query = index.normalize(query); // Normaliza a consulta
cout << "Original Query: " << query << endl;
        cout << "Normalized Query: " << normalized_query << endl;
        
        vector<pair<int, int>> result = index.search(normalized_query);
        index.print_titles(result, normalized_query);
    }
    return 0;
}