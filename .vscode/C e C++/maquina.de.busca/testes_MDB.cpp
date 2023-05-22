#include "maquina_de_busca.cpp"

void test_tokenize() {
    InvertedIndex index;
    cout << "Starting test_tokenize\n" << endl;
    int tests_passed = 0;
    int total_tests = 0;
    vector<string> tokens = index.tokenize("Hello world!");
    assert(tokens.size() == 2);
    assert(tokens[0] == "Hello");
    assert(tokens[1] == "world!");
    cout << "Test tokenize passed: " << tests_passed << "/" << total_tests << endl;
    cout << endl;
}

void test_normalize() {
    InvertedIndex index;
    cout << "Starting test_normalize\n" << endl;

    int tests_passed = 0;
    int total_tests = 0;

    // Test Case 1
    string term1 = index.normalize("Guarda-Chuva");
    total_tests++;
    if (term1 == "guardachuva") {
        tests_passed++;
        cout << "Test normalize - Test Case 1 passed!" << endl;
    } else {
        cout << "Test normalize - Test Case 1 failed!" << endl;
    }

    // Test Case 2
    string term2 = index.normalize("guarda chuva");
    total_tests++;
    if (term2 == "guardachuva") {
        tests_passed++;
        cout << "Test normalize - Test Case 2 passed!" << endl;
    } else {
        cout << "Test normalize - Test Case 2 failed!" << endl;
    }

    // Test Case 3
    string term3 = index.normalize("guarda_Chuva-1");
    total_tests++;
    if (term3 == "guardachuva") {
        tests_passed++;
        cout << "Test normalize - Test Case 3 passed!" << endl;
    } else {
        cout << "Test normalize - Test Case 3 failed!" << endl;
    }

    // Test Case 4
    string term4 = index.normalize("123Guarda-Chuva456");
    total_tests++;
    if (term4 == "guardachuva") {
        tests_passed++;
        cout << "Test normalize - Test Case 4 passed!" << endl;
    } else {
        cout << "Test normalize - Test Case 4 failed!" << endl;
    }

    // Test Case 5
    string term5 = index.normalize("ÉlÊtrIco");
    total_tests++;
    if (term5 == "eletrico") {
        tests_passed++;
        cout << "Test normalize - Test Case 5 passed!" << endl;
    } else {
        cout << "Test normalize - Test Case 5 failed!" << endl;
    }

    // Test Case 6
    string term6 = index.normalize("gU4rd4-C*HU_va");
    total_tests++;
    if (term6 == "gurdchuva") {
        tests_passed++;
        cout << "Test normalize - Test Case 6 passed!" << endl;
    } else {
        cout << "Test normalize - Test Case 6 failed!" << endl;
    }

    cout << "All tests normalize passed: " << tests_passed << "/" << total_tests << endl;
    cout << endl;
}

void test_load_documents() {
    string folderPath = "./documents.txt";
    InvertedIndex index(folderPath);

    assert(index.documents.size() == 3);  // Verificar se o número correto de documentos foi carregado

    cout << "Test load_documents passed!" << endl;
}

void test_search() {
    string folderPath = "./documents.txt";
    InvertedIndex index(folderPath);

    // Testar pesquisa por um termo presente em um documento
    string query1 = "casa";
    vector<pair<int, int>> result1 = index.search(query1);
    assert(!result1.empty());
    
    // Testar pesquisa por um termo ausente em todos os documentos
    string query2 = "pao";
    vector<pair<int, int>> result2 = index.search(query2);
    assert(result2.empty());
    
    cout << "Test search passed!" << endl;
}
void test_print_titles() {
string folderPath = "./documents.txt";
InvertedIndex index(folderPath);
// Testar a função print_titles com resultados não vazios
string query1 = "casa";
vector<pair<int, int>> result1 = index.search(query1);
cout << "Search query: " << query1 << endl;
index.print_titles(result1, query1);

// Testar a função print_titles com resultados vazios
string query2 = "pao";
vector<pair<int, int>> result2 = index.search(query2);
cout << "Search query: " << query2 << endl;
index.print_titles(result2, query2);

cout << "Test print_titles passed!" << endl;
}

int run_tests() {
    int tests_passed = 0;

    // Teste 1: test_tokenize
    try {
        test_tokenize();
        cout << "Test test_tokenize passed!\n" << endl;
        tests_passed++;
    } catch (const exception& e) {
        cerr << "Test test_tokenize failed: " << e.what() << endl;
    }

    // Teste 2: test_normalize
    try {
        test_normalize();
        cout << "Test test_normalize passed!\n" << endl;
        tests_passed++;
    } catch (const exception& e) {
        cerr << "Test test_normalize failed: " << e.what() << endl;
    }

    // Teste 3: test_load_documents
    try {
        test_load_documents();
        cout << "Test test_load_documents passed!\n" << endl;
        tests_passed++;
    } catch (const exception& e) {
        cerr << "Test test_load_documents failed: " << e.what() << endl;
    }

    // Teste 4: test_search
    try {
        test_search();
        cout << "Test test_search passed!" << endl;
        tests_passed++;
    } catch (const exception& e) {
        cerr << "Test test_search failed: " << e.what() << endl;
    }
    //Teste 5: test_print_titles
    try {
        test_print_titles();
        cout << "Test test_print_titles passed!" << endl;
        tests_passed++;
    } catch (const exception& e) {
        cerr << "Test test_print_titles failed: " << e.what() << endl;
    }
    return tests_passed;
}
int main() {
    int tests_passed = run_tests();
    int total_tests = 5;
    cout << "All tests passed: " << tests_passed << "/" << total_tests << endl;

    return 0;
}
