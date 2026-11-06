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

    int matrizTotales[5][8] = {0};

    ifstream archBin("arribos.bin", ios::binary);
    if (!archBin.is_open())
    {
        cout << "Error al abrir arribos.bin" << endl;
        return 1;
    }

    Arribo reg;
    while (archBin.read(reinterpret_cast<char *>(&reg.pais), sizeof(int)))
    {
        archBin.read(reinterpret_cast<char *>(&reg.aerolinea), sizeof(int));
        archBin.read(reg.iata, 3);
        reg.iata[3] = '\0';
        archBin.read(reinterpret_cast<char *>(&reg.cantPasajeros), sizeof(int));
        archBin.read(reinterpret_cast<char *>(&reg.dia), sizeof(int));

        if (reg.pais >= 0 && reg.pais < 5 && reg.aerolinea >= 0 && reg.aerolinea < 8)
        {
            matrizTotales[reg.pais][reg.aerolinea] += reg.cantPasajeros;
        }

        for (int i = 0; i < 15; i++)
        {
            if (compararStrings(aeropuertos[i].codigo, reg.iata) == 0)
            {
                aeropuertos[i].pasajeros += reg.cantPasajeros;
                aeropuertos[i].vuelos++;
                break;
            }
        }
    }
    archBin.close();

    cout << "Listado de totales de pasajeros" << endl;
    cout << "Pais\t\tAerolinea\t\t\tCant. Pasajeros" << endl;
    int maxPasajeros = -1;
    int paisMax = -1;
    int aeroMax = -1;

    for (int p = 0; p < 5; p++)
    {
        for (int a = 0; a < 8; a++)
        {
            cout << paises[p] << "\t\t" << aerolineas[a];
        
            if (aerolineas[a] == "GOL" || aerolineas[a] == "IndiGo") {
                cout << "\t\t\t";
            } else {
                cout << "\t\t";
            }
        
            cout << matrizTotales[p][a] << endl;

            if (matrizTotales[p][a] > maxPasajeros)
            {
                maxPasajeros = matrizTotales[p][a];
                paisMax = p;
                aeroMax = a;
            }
        }
    }
    cout << endl;

    int minPasajeros = -1;
    int idxMin = -1;

    for (int i = 0; i < 15; i++)
    {
        if (aeropuertos[i].pasajeros > 0)
        {
            if (minPasajeros == -1 || aeropuertos[i].pasajeros < minPasajeros)
            {
                minPasajeros = aeropuertos[i].pasajeros;
                idxMin = i;
            }
        }
    }

    if (idxMin != -1)
    {
        cout << "Aeropuerto que envió menor cantidad de pasajeros: "
             << aeropuertos[idxMin].codigo << ": " << aeropuertos[idxMin].nombre << endl;
        cout << "Pasajeros enviados: " << aeropuertos[idxMin].pasajeros
             << " en " << aeropuertos[idxMin].vuelos << " vuelos." << endl;
    }
    cout << endl;

    long pasajerosPorDia[7] = {0};

    ifstream archBin2("arribos.bin", ios::binary);
    while (archBin2.read(reinterpret_cast<char *>(&reg.pais), sizeof(int)))
    {
        archBin2.read(reinterpret_cast<char *>(&reg.aerolinea), sizeof(int));
        archBin2.read(reg.iata, 3);
        reg.iata[3] = '\0';
        archBin2.read(reinterpret_cast<char *>(&reg.cantPasajeros), sizeof(int));
        archBin2.read(reinterpret_cast<char *>(&reg.dia), sizeof(int));

        if (reg.pais == paisMax && reg.aerolinea == aeroMax)
        {
            if (reg.dia >= 0 && reg.dia < 7)
            {
                pasajerosPorDia[reg.dia] += reg.cantPasajeros;
            }
        }
    }
    archBin2.close();

    string dias[7] = {"Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado", "Domingo"};
    int ordenDias[7] = {0, 1, 2, 3, 4, 5, 6};

    for (int i = 0; i < 6; i++)
    {
        for (int j = i + 1; j < 7; j++)
        {
            if (pasajerosPorDia[ordenDias[i]] < pasajerosPorDia[ordenDias[j]])
            {
                int aux = ordenDias[i];
                ordenDias[i] = ordenDias[j];
                ordenDias[j] = aux;
            }
        }
    }

    cout << "La mayor cantidad de pasajeros (" << maxPasajeros
         << ") se dio para la combinación " << paises[paisMax]
         << " con aerolínea " << aerolineas[aeroMax]
         << " según el siguiente detalle diario:" << endl;
    cout << "Día\tCantidad" << endl;
    for (int i = 0; i < 7; i++)
    {
        int d = ordenDias[i];
        cout << dias[d] << "\t" << pasajerosPorDia[d] << endl;
    }

    return 0;
}
