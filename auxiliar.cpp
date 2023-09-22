#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

void random_matrix(int n, int m, std::ofstream& file) {
    std::srand(std::time(nullptr));

    file << n << " " << m << "\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            file << (std::rand() % 10 + 1) << " ";
        }
        file << "\n";
    }
    file << "\n";
}

int main(int argc, char *argv[]) {

    if (argc != 5) {
        std::cout << "ERRO >> Parâmetros: <n1> <m1> <n2> <m2>\n";
        return 1;
    }

    // Recebe os argumentos da main
    int n1 = std::atoi(argv[1]);
    int m1 = std::atoi(argv[2]);
    int n2 = std::atoi(argv[3]);
    int m2 = std::atoi(argv[4]);

    // Gera duas matrizes com valores aleatórios e salva em arquivos de saída
    std::ofstream file1("m1_" + std::to_string(n1) + "x" + std::to_string(m1) + ".txt");
    random_matrix(n1, m1, file1);
    file1.close();

    std::ofstream file2("m2_" + std::to_string(n1) + "x" + std::to_string(m1) + ".txt");
    random_matrix(n2, m2, file2);
    file2.close();

    return 0;
}
