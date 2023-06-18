#include "index.h"

// Função para normalizar um termo (converter letras para minúsculas, remover caracteres não alfanuméricos e númemeros, mas manter os espaços)
string InvertedIndex::normalize(const string& term) const {
    string normalized_term = term;

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
    void InvertedIndex::load_documents(const string& folderPath) {
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
    vector<string> InvertedIndex::get_file_names(const string& folderPath) {
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
    void InvertedIndex::add_document(const Document& doc) {
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
vector<string> InvertedIndex::tokenize(const string& str) const {
    vector<string> tokens;
    string token;
    istringstream iss(str);
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}
// Função para realizar uma busca no índice invertido com base em uma consulta (apenas documentos que contêm todos os termos/com ordem de relevância)
vector<pair<int, int>> InvertedIndex::search(const string& query) const {
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
    void InvertedIndex::print_titles(const vector<pair<int, int>>& results, const string& query) const {
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
