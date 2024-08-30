#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono> // Para medir el tiempo

using namespace std;
using namespace std::chrono;

template <typename T>
class Ordenador {
public:
    virtual void ordenar(vector<T> &lista) = 0;
};

template <typename T>
class OrdenadorBubbleSort : public Ordenador<T> {
public:
    void ordenar(vector<T> &lista) override {
        int n = lista.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (lista[j] > lista[j + 1]) {
                    T temp = lista[j];
                    lista[j] = lista[j + 1];
                    lista[j + 1] = temp;
                }
            }
        }
    }
};

int main() {
    ifstream infile("listabubble.txt");
    ofstream outfile("ordenadasbubble.txt");

    if (!infile.is_open()) {
        cerr << "No se pudo abrir el archivo 'listabubble.txt' para leer." << endl;
        return 1;
    }

    if (!outfile.is_open()) {
        cerr << "No se pudo abrir el archivo 'ordenadasbubble.txt' para escribir." << endl;
        return 1;
    }

    string line;
    OrdenadorBubbleSort<int> ordenador;

    // Medir el tiempo total de procesamiento
    auto start_total = high_resolution_clock::now();

    // Leer cada línea, ordenarla y escribir en el archivo de salida
    while (getline(infile, line)) {
        vector<int> lista;
        stringstream ss(line);
        int number;

        // Parsear la línea en una lista de enteros
        while (ss >> number) {
            lista.push_back(number);
        }

        // Ordenar la lista
        ordenador.ordenar(lista);

        // Escribir la lista ordenada en el archivo de salida
        for (const auto &elemento : lista) {
            outfile << elemento << " ";
        }
        outfile << endl; // Nueva línea para cada lista ordenada
    }

    // Medir el tiempo total de procesamiento
    auto end_total = high_resolution_clock::now();
    auto duration_total = duration_cast<milliseconds>(end_total - start_total);

    infile.close();
    outfile.close();

    cout << "Las listas han sido ordenadas y escritas en 'ordenadasbubble.txt'." << endl;
    cout << "Tiempo total de procesamiento: " << duration_total.count() << " milisegundos" << endl;

    return 0;
}
