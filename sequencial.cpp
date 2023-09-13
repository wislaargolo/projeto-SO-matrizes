#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <sstream>

int n1, n2, m1, m2;

std::vector<std::vector<int>> read_matrix(std::string filename, int &rows, int &cols) {

    std::ifstream input_file(filename);

    input_file >> rows >> cols;

    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) input_file >> matrix[i][j];
    }

    input_file.close();

    return matrix;

}

double multiply_matrices(std::vector<std::vector<int>> &mat_A, std::vector<std::vector<int>> &mat_B, 
                         std::vector<std::vector<int>> &result) {

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < m2; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < m1; ++k) result[i][j] += mat_A[i][k] * mat_B[k][j];
        }
    }

    auto end = std::chrono::steady_clock::now();
    
    return std::chrono::duration<double, std::milli>(end - start).count();
}


int main(int argc, char *argv[]) {

    if (argc != 3) {
        std::cout << "ERRO >> Parâmetros: <matrix1.txt> <matrix2.txt>\n";
        return 1;
    }

    // Lê as matrizes de entrada
    std::vector<std::vector<int>> mat_A = read_matrix(argv[1], n1, m1);
    std::vector<std::vector<int>> mat_B = read_matrix(argv[2], n2, m2);

    if (m1 != n2) {
        std::cout << "ERRO >> As matrizes não podem ser multiplicadas!\n";
        return 1;
    }

   // for (int k = 0; k < 10; ++k) {

        // Inicializa a matriz resultado
        std::vector<std::vector<int>> result(n1, std::vector<int>(m2, 0));

        // Obtem a matriz resultado e o tempo necessário para obtê-la
        double time = multiply_matrices(mat_A, mat_B, result);

        // Salva a matriz resultado e o tempo num arquivo de saída                  
        std::ofstream output_file("sequencial/sequencial" + std::to_string(n1)
                                 + "x" + std::to_string(m2));

        output_file << n1 << ' ' << m2 << std::endl;

        for(int i = 0; i < n1; ++i) {
            for(int j = 0; j < m2; ++j) 
                output_file  << 'c' << i + 1 << j + 1 << ' ' << result[i][j] << std::endl;
        }

        output_file << time << std::endl;

        output_file.close();
   // }

   return 0;
 
}
