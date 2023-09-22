#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <sstream>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

int n1, n2, m1, m2;

//Struct que será passada como argumento no pthread_create
struct threads_arguments{
  std::vector<std::vector<int>> mat1;
  std::vector<std::vector<int>> mat2;
  std::string filename;
  int start;
  int end;
  std::chrono::steady_clock::time_point start_time;
};

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

void multiply_matrices(threads_arguments arguments) {

    std::ofstream output_file(arguments.filename);

    output_file << n1 << ' ' << m2 << std::endl;

    for(int idx = arguments.start; idx < arguments.end; ++idx) {

        int i = idx / m2;
        int j = idx % m2;

        int sum = 0;

        for(int k = 0; k < m1; ++k) 
            sum += arguments.mat1[i][k] * arguments.mat2[k][j];

        output_file << 'c' << i + 1 << j + 1 << ' ' << sum << std::endl;
    }

    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();

    output_file << std::chrono::duration<double, std::milli>(end_time - arguments.start_time).count();

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

    // Calcula a quantidade de arquivos necessários baseado na quantidade 
    // de elementos por processo (P)
    int P = std::atoi(argv[3]);
    int n_files = ((n1 * m2) + P - 1)/P;

    if (m1 != n2) {
        std::cout << "ERRO >> As matrizes não podem ser multiplicadas!\n";
        return 1;
    }

    int start = 0;

    //Declara n_files threads
    pthread_t threads[n_files];

    //Criação de um vector de tipo estruturado "threads_arguments"
    std::vector<threads_arguments> vector_struct;
    //Resize para garantir que haja espaço, assim evita bugs ao usar [i]
    vector_struct.resize(n_files);

    for (int i = 0; i < n_files; i++) {

        //Atribuição de valores as variaveis do tipo estruturado "threads_arguments" 
        vector_struct[i].mat1 = mat_A;
        vector_struct[i].mat2 = mat_B;
        vector_struct[i].filename = "threads/thread" + std::to_string(i) + ".txt";
        vector_struct[i].end = (start + P) < (n1 *  m2) ? (start + P) : (n1 * m2);
        vector_struct[i].start = start;
        vector_struct[i].start_time = std::chrono::steady_clock::now();

        //Criação das novas threads
        pthread_create(&threads[i], NULL, (void*(*)(void*))multiply_matrices, (void*)&vector_struct[i]);

        start = vector_struct[i].end;
    }

    //Função de Junção de Threads, espera todas as threads serem finalizadas
    for(int k = 0; k < n_files;k++) 
        pthread_join(threads[k], NULL);

    return 0;
}