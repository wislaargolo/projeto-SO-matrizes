# Projeto de Multiplicação de Matrizes: Sequencial vs Paralelo

## Descrição
Neste projeto, implementamos a multiplicação de matrizes usando várias abordagens: sequencial, paralelo com threads e paralelo com processos

## Funcionalidades

1. **Auxiliar**: Programa para gerar matrizes aleatórias. Ele recebe quatro argumentos de linha de comando (n1, m1, n2, m2), que representam as dimensões de duas matrizes que serão multiplicadas. 

2. **Sequencial**: Programa que realiza a multiplicação de matrizes de forma sequencial. Recebe como entrada dois arquivos contendo as matrizes M1 e M2 e executa a multiplicação, salvando o resultado e o tempo de execução em um arquivo.

3. **Paralelo - Threads**: Programa faz a multiplicação de matrizes usando threads. Além dos arquivos de entrada, ele também recebe um número inteiro P, que indica a quantidade de elementos a serem processados por cada thread.

4. **Paralelo - Processos**: Similar ao programa de threads, mas usando processos em vez de threads para a multiplicação de matrizes.
