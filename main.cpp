#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Aeropuerto
{
    char codigo[4];
    string nombre;
    int pasajeros;
    int vuelos;
};

struct Arribo
{
    int pais;
    int aerolinea;
    char iata[4];
    int cantPasajeros;
    int dia;
};

template <typename T>
struct Nodo
{
    T info;
    Nodo<T> *sig;
};

template <typename T>
int criterioAsc(T a, T b)
{
    return a - b;
}

template <typename T>
void insertarOrdenado(Nodo<T> *&lista, T valor, int (*criterio)(T, T))
{
    Nodo<T> **p = &lista;

    while (*p != nullptr && criterio((*p)->info, valor) < 0)
    {
        p = &((*p)->sig);
    }

    Nodo<T> *nuevo = new Nodo<T>();
    nuevo->info = valor;
    nuevo->sig = *p;
    *p = nuevo;
}

int compararStrings(const char *s1, const char *s2)
{
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
    {
        i++;
    }
    return s1[i] - s2[i];
}

int main()
{
    string paises[5];
    string aerolineas[8];
    Aeropuerto aeropuertos[15];
    int cantAeropuertos = 0;

    ifstream archTexto("nombres.txt");
    if (!archTexto.is_open())
    {
        cout << "Error al abrir nombres.txt" << endl;
        return 1;
    }

    for (int i = 0; i < 5; i++)
    {
        getline(archTexto, paises[i]);
    }

    for (int i = 0; i < 8; i++)
    {
        getline(archTexto, aerolineas[i]);
    }

    string linea;
    while (cantAeropuertos < 15 && getline(archTexto, linea))
    {
        if (linea.empty())
            continue;
        size_t posTab = linea.find('\t');
        if (posTab != string::npos)
        {
            string cod = linea.substr(0, posTab);
            string nom = linea.substr(posTab + 1);

            for (int k = 0; k < 3; k++)
            {
                aeropuertos[cantAeropuertos].codigo[k] = cod[k];
            }
            aeropuertos[cantAeropuertos].codigo[3] = '\0';

            aeropuertos[cantAeropuertos].nombre = nom;
            aeropuertos[cantAeropuertos].pasajeros = 0;
            aeropuertos[cantAeropuertos].vuelos = 0;
            cantAeropuertos++;
        }
    }
    archTexto.close();

    return 0;
}
