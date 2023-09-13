#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <sstream>
#include <sys/wait.h>
#include <unistd.h>


int n1, n2, m1, m2;

std::vector<std::vector<int>> read_matrix(std::string filename, int &rows, int &cols) {

    std::ifstream input_file(filename);

    std::string line;
    std::getline(input_file, line);
    std::stringstream ss(line);

    ss >> rows >> cols;

    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) 
            input_file >> matrix[i][j];
    }

    input_file.close();

    return matrix;

}

void multiply_matrices(std::vector<std::vector<int>> &mat_A, std::vector<std::vector<int>> &mat_B, 
                       std::string filename, int start, int end, std::chrono::steady_clock::time_point &start_time) {

    std::ofstream output_file(filename);

    output_file << n1 << ' ' << m2 << std::endl;

    for(int idx = start; idx < end; ++idx) {

        int i = idx / m2;
        int j = idx % m2;

        int sum = 0;

        for(int k = 0; k < m1; ++k) 
            sum += mat_A[i][k] * mat_B[k][j];

        output_file << 'c' << i + 1 << j + 1 << ' ' << sum << std::endl;
    }

    auto end_time = std::chrono::steady_clock::now();

    output_file << std::chrono::duration<double, std::milli>(end_time - start_time).count();

    output_file.close();
}

int main(int argc, char *argv[]) {

    if (argc != 4) {
        std::cout << "ERRO >> Parâmetros: <matrix1.txt> <matrix2.txt> <P>\n";
        return 1;
    }

    // Lê as matrizes de entrada
    std::vector<std::vector<int>> mat_A = read_matrix(argv[1], n1, m1);
    std::vector<std::vector<int>> mat_B = read_matrix(argv[2], n2, m2);

    // Calcula a quantidade de processos necessários baseado na quantidade 
    // de elementos por processo (P)
    int P = std::atoi(argv[3]);
    int n_processes = ((n1 * m2) + P - 1)/P;

    if (m1 != n2) {
        std::cout << "ERRO >> As matrizes não podem ser multiplicadas!\n";
        return 1;
    }

    int start = 0;

    for (int i = 0; i < n_processes; ++i) {

        int end =  (start + P) < (n1 *  m2) ? (start + P) : (n1 * m2);
        
        auto start_time = std::chrono::steady_clock::now();

        pid_t pid = fork();

        if(pid == 0) { // Filho
            std::string filename = "processes/process" + std::to_string(i) + ".txt";
            multiply_matrices(mat_A, mat_B, filename, start, end, start_time); // e se a gente colocar mat_A e mat_B como parametros globais?
            exit(0);
        } 

        start = end;
    }

    // Aguarda todos os processos filhos terminarem a execução
    for (int p = 0; p < n_processes; ++p) 
        wait(NULL);

    return 0;
}