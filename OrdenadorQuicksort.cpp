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
class OrdenadorQuicksort : public Ordenador<T> {
    public:
        void ordenar(vector<T> &lista) override {
            ordenar(lista, 0, lista.size() - 1);
        }

    private:
        void ordenar(vector<T> &lista, int posIni, int posFin) {
            if (posIni < posFin) {
                int posPivote = (posIni + posFin) / 2; // Use the middle element as pivot
                int i = posIni, j = posFin;
                T pivote = lista[posPivote];

                // Move elements <= pivot to the left and > pivot to the right
                i = posPivote + 1;
                while (i <= posFin) {
                    if (lista[i] <= pivote) {
                        lista[posPivote] = lista[i];
                        lista[i] = lista[posPivote + 1];
                        lista[posPivote + 1] = pivote;
                        posPivote++;
                    }
                    i++;
                }

                j = posPivote - 1;
                while (j >= posIni) {
                    if (lista[j] > pivote) {
                        lista[posPivote] = lista[j];
                        lista[j] = lista[posPivote - 1];
                        lista[posPivote - 1] = pivote;
                        posPivote--;
                    }
                    j--;
                }

                // Recursively sort the left and right sub-arrays
                ordenar(lista, posIni, posPivote - 1);
                ordenar(lista, posPivote + 1, posFin);
            }
        }
};

int main() {
    ifstream infile("listaquick.txt");
    ofstream outfile("ordenadasquick.txt");

    if (!infile.is_open()) {
        cerr << "No se pudo abrir el archivo 'listaquick.txt' para leer." << endl;
        return 1;
    }

    if (!outfile.is_open()) {
        cerr << "No se pudo abrir el archivo 'ordenadasquick.txt' para escribir." << endl;
        return 1;
    }

    string line;
    OrdenadorQuicksort<int> ordenador;

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

        // Sort the list using QuickSort
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

    cout << "Las listas han sido ordenadas y escritas en 'ordenadasquick.txt'." << endl;
    cout << "Tiempo transcurrido: " << duration.count() << " ms." << endl;

    return 0;
}
