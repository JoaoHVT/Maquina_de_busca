#include "index.h"

string InvertedIndex::normalize(const string& term) const {
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

    for (auto& pair : accent_pairs) {
        size_t pos = normalized_term.find(pair.first);
        while (pos != string::npos) {
            normalized_term.replace(pos, pair.first.length(), 1, pair.second);
            pos = normalized_term.find(pair.first, pos + 1);
        }
    }

    normalized_term.erase(remove_if(normalized_term.begin(), normalized_term.end(),
                                    [](unsigned char c) { return isdigit(c); }),
                          normalized_term.end());

    normalized_term.erase(remove_if(normalized_term.begin(), normalized_term.end(),
                                    [](unsigned char c) { return !isalnum(c); }),
                          normalized_term.end());

    transform(normalized_term.begin(), normalized_term.end(), normalized_term.begin(),
              [](unsigned char c) { return tolower(c); });

    return normalized_term;
}

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

vector<string> InvertedIndex::get_file_names(const string& folderPath) {
    vector<string> fileNames;
    DIR* directory;
    dirent* entry;
    if ((directory = opendir(folderPath.c_str())) != nullptr) {
        while ((entry = readdir(directory)) != nullptr) {
            if (entry->d_type == DT_REG) {
                fileNames.push_back(entry->d_name);
            }
        }
        closedir(directory);
    } else {
        cout << "Failed to open the directory: " << folderPath << endl;
    }
    return fileNames;
}

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

vector<string> InvertedIndex::tokenize(const string& str) const {
    vector<string> tokens;
    string token;
    istringstream iss(str);
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

vector<pair<int, int>> InvertedIndex::search(const string& query) const {
    vector<string> terms = tokenize(query);
    unordered_map<int, int> term_counts;
    for (const auto& term : terms) {
        string normalized_term = normalize(term);
        if (index.count(normalized_term)) {
            for (const auto& doc_entry : index.at(normalized_term)) {
                int doc_id = doc_entry.first;
                int term_count = doc_entry.second;
                term_counts[doc_id] += term_count;
            }
        }
    }
    vector<pair<int, int>> results(term_counts.begin(), term_counts.end());
    sort(results.begin(), results.end(),
         [](const pair<int, int>& a, const pair<int, int>& b) {
             return a.second > b.second;
         });
    return results;
}

void InvertedIndex::print_titles(const vector<pair<int, int>>& results, const string& query) const {
    cout << "Search Results for '" << query << "':" << endl;
    for (const auto& result : results) {
        int doc_id = result.first;
        const Document& doc = documents.at(doc_id);
        cout << "Title: " << doc.title << endl;
        cout << "Frequency: " << result.second << endl;
        cout << "----------------------------------" << endl;
    }
}
