#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Función para sumar dos matrices
vector<vector<int>> sumarMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    vector<vector<int>> C(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

// Función para restar dos matrices
vector<vector<int>> restarMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    vector<vector<int>> C(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

// Función para dividir una matriz en submatrices
void dividirMatriz(const vector<vector<int>>& A, vector<vector<int>>& A11, vector<vector<int>>& A12, vector<vector<int>>& A21, vector<vector<int>>& A22) {
    int n = A.size();
    int m = n / 2;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + m];
            A21[i][j] = A[i + m][j];
            A22[i][j] = A[i + m][j + m];
        }
    }
}

// Función para combinar submatrices en una matriz
vector<vector<int>> combinarMatrices(const vector<vector<int>>& A11, const vector<vector<int>>& A12, const vector<vector<int>>& A21, const vector<vector<int>>& A22) {
    int n = A11.size() * 2;
    vector<vector<int>> C(n, vector<int>(n));
    int m = A11.size();
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            C[i][j] = A11[i][j];
            C[i][j + m] = A12[i][j];
            C[i + m][j] = A21[i][j];
            C[i + m][j + m] = A22[i][j];
        }
    }
    return C;
}

// Implementación del algoritmo de Strassen
void strassen(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    int n = A.size();
    if (n <= 2) {
        // Caso base: multiplicación directa de matrices 2x2
        vector<vector<int>> temp(2, vector<int>(2));
        temp[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0];
        temp[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1];
        temp[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0];
        temp[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1];
        C = temp;
        return;
    }

    vector<vector<int>> A11(n / 2, vector<int>(n / 2)), A12(n / 2, vector<int>(n / 2)),
                        A21(n / 2, vector<int>(n / 2)), A22(n / 2, vector<int>(n / 2)),
                        B11(n / 2, vector<int>(n / 2)), B12(n / 2, vector<int>(n / 2)),
                        B21(n / 2, vector<int>(n / 2)), B22(n / 2, vector<int>(n / 2)),
                        M1(n / 2, vector<int>(n / 2)), M2(n / 2, vector<int>(n / 2)),
                        M3(n / 2, vector<int>(n / 2)), M4(n / 2, vector<int>(n / 2)),
                        M5(n / 2, vector<int>(n / 2)), M6(n / 2, vector<int>(n / 2)),
                        M7(n / 2, vector<int>(n / 2)),
                        temp1(n / 2, vector<int>(n / 2)), temp2(n / 2, vector<int>(n / 2));

    dividirMatriz(A, A11, A12, A21, A22);
    dividirMatriz(B, B11, B12, B21, B22);

    temp1 = sumarMatrices(A11, A22);
    temp2 = sumarMatrices(B11, B22);
    strassen(temp1, temp2, M1);

    temp1 = sumarMatrices(A21, A22);
    strassen(temp1, B11, M2);

    temp1 = restarMatrices(B12, B22);
    strassen(A11, temp1, M3);

    temp1 = restarMatrices(B21, B11);
    strassen(A22, temp1, M4);

    temp1 = sumarMatrices(A11, A12);
    strassen(temp1, B22, M5);

    temp1 = restarMatrices(A21, A11);
    temp2 = sumarMatrices(B11, B12);
    strassen(temp1, temp2, M6);

    temp1 = restarMatrices(A12, A22);
    temp2 = sumarMatrices(B21, B22);
    strassen(temp1, temp2, M7);

    vector<vector<int>> C11 = sumarMatrices(restarMatrices(sumarMatrices(M1, M4), M5), M7);
    vector<vector<int>> C12 = sumarMatrices(M3, M5);
    vector<vector<int>> C21 = sumarMatrices(M2, M4);
    vector<vector<int>> C22 = sumarMatrices(restarMatrices(sumarMatrices(M1, M3), M2), M6);

    C = combinarMatrices(C11, C12, C21, C22);
}

int main() {
    ifstream infile("inputscubicos.txt");  // Abrir el archivo de entrada
    if (!infile.is_open()) {
        cerr << "No se pudo abrir el archivo 'inputscubicos.txt' para leer." << endl;
        return 1;
    }

    ofstream outfile("resultadoscubstrassen.txt");  // Abrir el archivo de salida
    if (!outfile.is_open()) {
        cerr << "No se pudo abrir el archivo 'resultadoscubstrassen.txt' para escribir." << endl;
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

        // Asegurar que el tamaño de las matrices es una potencia de 2
        int newSize = 1;
        while (newSize < max(sizes[i], sizes[i + 1])) {
            newSize *= 2;
        }
        vector<vector<int>> matrizA(newSize, vector<int>(newSize, 0));
        vector<vector<int>> matrizB(newSize, vector<int>(newSize, 0));
        vector<vector<int>> resultado(newSize, vector<int>(newSize, 0));

        // Copiar matrices originales en las matrices ampliadas
        for (int r = 0; r < sizes[i]; ++r) {
            for (int c = 0; c < sizes[i]; ++c) {
                matrizA[r][c] = matrices[i][r][c];
            }
        }

        for (int r = 0; r < sizes[i + 1]; ++r) {
            for (int c = 0; c < sizes[i + 1]; ++c) {
                matrizB[r][c] = matrices[i + 1][r][c];
            }
        }

        // Medir el tiempo de multiplicación
        auto start = high_resolution_clock::now();
        strassen(matrizA, matrizB, resultado);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);

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

        outfile << endl << "Resultado de la multiplicación (" << newSize << "x" << newSize << "):" << endl;
        for (const auto& fila : resultado) {
            for (int num : fila) {
                outfile << num << " ";
            }
            outfile << endl;
        }

        outfile << endl << "Tiempo de multiplicación: " << duration.count() << " milisegundos" << endl << endl;
    }

    // Medir el tiempo total de procesamiento
    auto end_total = high_resolution_clock::now();
    auto duration_total = duration_cast<milliseconds>(end_total - start_total);

    outfile.close(); // Cerrar el archivo de salida

    cout << "La multiplicación de matrices usando el algoritmo de Strassen ha sido escrita en 'resultadoscubstrassen.txt'." << endl;
    cout << "Tiempo total de procesamiento: " << duration_total.count() << " milisegundos" << endl;

    return 0;
}
