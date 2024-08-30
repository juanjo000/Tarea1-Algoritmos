#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono> // Para el temporizador

using namespace std;
using namespace std::chrono; // Para el temporizador

template <typename T>
class Ordenador {
    public:
        virtual void ordenar(vector<T> &lista) = 0;
};

template <typename T>
class OrdenadorMergeSort : public Ordenador<T> {
    public:
        void ordenar(vector<T> &lista) override {
            this->ordenar(lista, 0, lista.size() - 1);
        }

    private:
        void ordenar(vector<T> &lista, int posIni, int posFin) {
            if (posIni < posFin) {
                // Ordenamos la parte izquierda y derecha del array
                int posCentral = (posIni + posFin) / 2;
                this->ordenar(lista, posIni, posCentral);
                this->ordenar(lista, posCentral + 1, posFin);
                
                // Mezclamos
                merge(lista, posIni, posCentral, posFin);
            }
        }

        void merge(vector<T> &lista, int posIni, int posCentral, int posFin) {
            vector<T> temp(lista.size());
            int i = posIni, j = posCentral + 1, k = posIni;

            while (i <= posCentral && j <= posFin) {
                if (lista[i] <= lista[j]) {
                    temp[k++] = lista[i++];
                } else {
                    temp[k++] = lista[j++];
                }
            }

            while (i <= posCentral) {
                temp[k++] = lista[i++];
            }

            while (j <= posFin) {
                temp[k++] = lista[j++];
            }

            for (i = posIni; i <= posFin; ++i) {
                lista[i] = temp[i];
            }
        }
};

int main() {
    ifstream infile("listamerge.txt");
    ofstream outfile("ordenadasmerge.txt");

    if (!infile.is_open()) {
        cerr << "No se pudo abrir el archivo 'listamerge.txt' para leer." << endl;
        return 1;
    }

    if (!outfile.is_open()) {
        cerr << "No se pudo abrir el archivo 'ordenadasmerge.txt' para escribir." << endl;
        return 1;
    }

    string line;
    OrdenadorMergeSort<int> ordenador;

    // Start the timer
    auto start = high_resolution_clock::now();

    // Read each line, sort it, and write to the output file
    while (getline(infile, line)) {
        vector<int> lista;
        stringstream ss(line);
        int number;

        // Parse the line into a list of integers
        while (ss >> number) {
            lista.push_back(number);
        }

        // Sort the list using Merge Sort
        ordenador.ordenar(lista);

        // Write the sorted list to the output file
        for (const auto &elemento : lista) {
            outfile << elemento << " ";
        }
        outfile << endl; // New line for each sorted list
    }

    // Stop the timer and compute the duration
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    infile.close();
    outfile.close();

    cout << "Las listas han sido ordenadas y escritas en 'ordenadasmerge.txt'." << endl;
    cout << "Tiempo transcurrido: " << duration.count() << " ms." << endl;

    return 0;
}
