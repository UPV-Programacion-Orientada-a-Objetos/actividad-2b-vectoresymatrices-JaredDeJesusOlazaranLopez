#include <iostream>
#include <cstring>
using namespace std;

typedef struct
{
    int idLote;
    char nombreComponente[50];
    float pesoUnitario;
    int cantidadTotal;
} LoteProduccion;

LoteProduccion **almacen = nullptr;
LoteProduccion *maestroLotes = nullptr;
int *indicesDisponibles = nullptr;
int filas = 0, columnas = 0;
int capacidad = 100;
int totalLotes = 0;

int *pilaIDLote = nullptr;
int *pilaResultado = nullptr;
int top = -1;
int maxPila = 10;

void inicializarSistema()
{
    cout << "Filas: ";
    cin >> filas;
    cout << "Columnas: ";
    cin >> columnas;

    almacen = new LoteProduccion *[filas];
    for (int i = 0; i < filas; i++)
    {
        almacen[i] = new LoteProduccion[columnas];
        for (int j = 0; j < columnas; j++)
            almacen[i][j].idLote = -1;
    }

    maestroLotes = new LoteProduccion[capacidad];
    indicesDisponibles = new int[capacidad];
    for (int i = 0; i < capacidad; i++)
        indicesDisponibles[i] = 0;

    pilaIDLote = new int[maxPila];
    pilaResultado = new int[maxPila];

    cout << "\n--- Almacén " << filas << "x" << columnas << " inicializado ---\n";
}

int buscarEspacioMaestro()
{
    for (int i = 0; i < capacidad; i++)
    {
        if (indicesDisponibles[i] == 0)
            return i;
    }
    return -1;
}

void colocarLote()
{
    int f, c;
    cout << "Fila: ";
    cin >> f;
    cout << "Columna: ";
    cin >> c;

    if (f < 0 || f >= filas || c < 0 || c >= columnas)
    {
        cout << "Posicion invalida.\n";
        return;
    }
    if (almacen[f][c].idLote != -1)
    {
        cout << "Celda ocupada.\n";
        return;
    }

    LoteProduccion nuevo;
    cout << "ID: ";
    cin >> nuevo.idLote;
    cout << "Nombre: ";
    cin.ignore();
    cin.getline(nuevo.nombreComponente, 50);
    cout << "Peso unitario: ";
    cin >> nuevo.pesoUnitario;
    cout << "Cantidad total: ";
    cin >> nuevo.cantidadTotal;

    int idx = buscarEspacioMaestro();
    if (idx == -1)
    {
        cout << "Capacidad llena.\n";
        return;
    }

    maestroLotes[idx] = nuevo;
    indicesDisponibles[idx] = 1;
    almacen[f][c] = nuevo;

    cout << "Lote " << nuevo.idLote << " (" << nuevo.nombreComponente
         << ") colocado en (" << f << "," << c << ")\n";
    totalLotes++;
}

void pushInspeccion()
{
    if (top == maxPila - 1)
    {
        for (int i = 0; i < maxPila - 1; i++)
        {
            pilaIDLote[i] = pilaIDLote[i + 1];
            pilaResultado[i] = pilaResultado[i + 1];
        }
        top--;
    }

    int id, res;
    cout << "ID de lote: ";
    cin >> id;
    cout << "Resultado (1=Aprob,0=Rech): ";
    cin >> res;

    top++;
    pilaIDLote[top] = id;
    pilaResultado[top] = res;

    cout << "Evento PUSH -> Lote " << id << " | Resultado " << res << "\n";
}

void popInspeccion()
{
    if (top == -1)
    {
        cout << "Pila vacia.\n";
        return;
    }
    cout << "POP -> Lote " << pilaIDLote[top]
         << " | Resultado " << pilaResultado[top] << "\n";
    top--;
}

void reporteFila()
{
    int f;
    cout << "Fila a reportar: ";
    cin >> f;
    if (f < 0 || f >= filas){
        cout << "Fila invalida.\n";
        return;
    }
    cout << "--- Reporte de Fila " << f << " ---\n";
    for (int j = 0; j < columnas; j++){
        if (almacen[f][j].idLote != -1)
            cout << "(" << f << "," << j << "): " << almacen[f][j].idLote
                 << " - " << almacen[f][j].nombreComponente << "\n";
    else
            cout << "(" << f << "," << j << "): Vacío\n";
    }
}

void buscarComponente()
{
    char nombre[50];
    cout << "Nombre a buscar: ";
    cin.ignore();
    cin.getline(nombre, 50);

    bool encontrado = false;
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            if (almacen[i][j].idLote != -1 &&
                strcmp(almacen[i][j].nombreComponente, nombre) == 0)
            {
                cout << "Encontrado en (" << i << "," << j << ") Cantidad: "
                     << almacen[i][j].cantidadTotal << "\n";
                encontrado = true;
            }
        }
    }
    if (!encontrado){
        cout << "No se encontro ese componente.\n";
    }
}

void liberarMemoria()
{
    for (int i = 0; i < filas; i++){
        delete[] almacen[i];
    }
    delete[] almacen;
    delete[] maestroLotes;
    delete[] indicesDisponibles;
    delete[] pilaIDLote;
    delete[] pilaResultado;
    cout << "\nMemoria liberada correctamente.\n";
}

int main()
{
    inicializarSistema();
    int op;

    do
    {
        cout << "\n--- MENU ---\n";
        cout << "1. Colocar lote\n";
        cout << "2. Control de calidad (Push)\n";
        cout << "3. Deshacer inspeccion (Pop)\n";
        cout << "4. Reporte por fila\n";
        cout << "5. Buscar componente\n";
        cout << "6. Salir\nOpcion: ";
        cin >> op;

        switch (op)
        {
        case 1:
            colocarLote();
            break;
        case 2:
            pushInspeccion();
            break;
        case 3:
            popInspeccion();
            break;
        case 4:
            reporteFila();
            break;
        case 5:
            buscarComponente();
            break;
        case 6:
            liberarMemoria();
            break;
        default:
            cout << "Opcion invalida.\n";
        }

    } while (op != 6);

    return 0;
}