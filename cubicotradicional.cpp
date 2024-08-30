#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Función para multiplicar dos matrices
void multiplicarMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    int n = A.size(); // Suponemos que las matrices son cuadradas y del mismo tamaño
    int m = A[0].size();
    int p = B[0].size();

    // Inicializar la matriz de resultado C con ceros
    C.assign(n, vector<int>(p, 0));

    // Multiplicación de matrices
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            for (int k = 0; k < m; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    ifstream infile("inputscubicos.txt");  // Abrir el archivo de entrada
    if (!infile.is_open()) {
        cerr << "No se pudo abrir el archivo 'inputscubicos.txt' para leer." << endl;
        return 1;
    }

    ofstream outfile("resultadoscubtradicional.txt");  // Abrir el archivo de salida
    if (!outfile.is_open()) {
        cerr << "No se pudo abrir el archivo 'resultadoscubtradicional.txt' para escribir." << endl;
        return 1;
    }

    vector<vector<vector<int>>> matrices;
    vector<int> sizes;
    int size;

    // Leer todas las matrices del archivo
    while (infile >> size) {
        vector<vector<int>> matriz(size, vector<int>(size));
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                infile >> matriz[i][j];
            }
        }
        matrices.push_back(matriz);
        sizes.push_back(size);
    }

    infile.close();  // Cerrar el archivo de entrada

    if (matrices.size() % 2 != 0) {
        cerr << "El número de matrices no es par." << endl;
        return 1;
    }

    // Medir el tiempo total de procesamiento
    auto start_total = high_resolution_clock::now();

    // Multiplicar las matrices de dos en dos
    for (size_t i = 0; i < matrices.size(); i += 2) {
        if (sizes[i] != sizes[i + 1]) {
            cerr << "Las dimensiones de las matrices no son compatibles para la multiplicación: "
                 << sizes[i] << " x " << sizes[i] << " y " << sizes[i + 1] << " x " << sizes[i + 1] << endl;
            return 1;
        }

        vector<vector<int>> resultado;
        multiplicarMatrices(matrices[i], matrices[i + 1], resultado);

        // Escribir las matrices y el resultado en el archivo de salida
        outfile << "Matriz A (" << sizes[i] << "x" << sizes[i] << "):" << endl;
        for (const auto& fila : matrices[i]) {
            for (int num : fila) {
                outfile << num << " ";
            }
            outfile << endl;
        }

        outfile << endl << "Matriz B (" << sizes[i + 1] << "x" << sizes[i + 1] << "):" << endl;
        for (const auto& fila : matrices[i + 1]) {
            for (int num : fila) {
                outfile << num << " ";
            }
            outfile << endl;
        }

        outfile << endl << "Resultado de la multiplicación (" << sizes[i] << "x" << sizes[i + 1] << "):" << endl;
        for (const auto& fila : resultado) {
            for (int num : fila) {
                outfile << num << " ";
            }
            outfile << endl;
        }

        outfile << endl; // Línea en blanco entre los resultados
    }

    // Medir el tiempo total de procesamiento
    auto end_total = high_resolution_clock::now();
    auto duration_total = duration_cast<milliseconds>(end_total - start_total);

    outfile.close(); // Cerrar el archivo de salida

    cout << "La multiplicación de matrices ha sido escrita en 'resultadoscubtradicional.txt'." << endl;
    cout << "Tiempo total de procesamiento: " << duration_total.count() << " milisegundos" << endl;

    return 0;
}
