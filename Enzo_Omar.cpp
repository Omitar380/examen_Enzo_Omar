#include <iostream>
#include <string>

using namespace std;

// Guarda el dia, mes y anio en que se realizo un prestamo.
struct Fecha
{
    int dia;
    int mes;
    int anio;
};

// Nodo de la lista de libros: almacena sus datos y apunta al siguiente libro.
struct Libro
{
    int codigo_libro;
    string nombre_libro;
    Libro *sig;
};

// Nodo de la lista de prestamos: relaciona un libro con un estudiante y una fecha.
struct Prestamo
{
    int codigo_libro;
    int codigo_estudiante;
    Fecha fecha_prestamo;
    Prestamo *sig;
};

// Agrega un nuevo libro al final de la lista cab.
void insertar_libro(Libro *&cab, int codigo, const string &nombre);
// Agrega al final de cab un prestamo con su libro, estudiante y fecha.
void insertar_prestamo(Prestamo *&cab, int codigo_libro, int codigo_estudiante, int dia, int mes, int anio);
// Ordena los nodos de la lista de libros por codigo ascendente.
void ordenar_libros(Libro *cab);
// Imprime todos los libros y todos los prestamos registrados.
void imprimir_listas(Libro *cab1, Prestamo *cab2);
// Imprime los libros cuyo codigo no aparece en la lista de prestamos.
void libros_nunca_prestados(Libro *cab1, Prestamo *cab2);
// Cuenta y retorna cuantas veces aparece el codigo en la lista de prestamos.
int veces_prestado(Prestamo *cab2, int codigo);
// Busca un libro por codigo y retorna su nodo o nullptr si no existe.
Libro *buscar_libro(Libro *cab1, int codigo);
// Libera la memoria ocupada por ambas listas.
void liberar_listas(Libro *&cab1, Prestamo *&cab2);

int main()
{
    // cab1 guarda los libros y cab2 guarda los prestamos en listas simples.
    Libro *cab1 = nullptr;
    Prestamo *cab2 = nullptr;

    // Datos de prueba: los libros se insertan desordenados para probar el ordenamiento.
    insertar_libro(cab1, 105, "El diario de Ana Frank");
    insertar_libro(cab1, 102, "Don Quijote de la Mancha");
    insertar_libro(cab1, 108, "El Principito");
    insertar_libro(cab1, 101, "1984");
    insertar_libro(cab1, 110, "Ficciones");

    // Datos de prueba de prestamos (libro, estudiante, dia, mes y anio).
    insertar_prestamo(cab2, 102, 2023001, 15, 4, 2024);
    insertar_prestamo(cab2, 102, 2023005, 20, 4, 2024);
    insertar_prestamo(cab2, 108, 2023010, 21, 4, 2024);
    insertar_prestamo(cab2, 101, 2023012, 22, 4, 2024);
    insertar_prestamo(cab2, 102, 2023015, 25, 4, 2024);

    cout << "--- SISTEMA DE BIBLIOTECA XYZ ---\n";

    // Requisito 2: ordena la lista cab1 ascendentemente por codigo.
    cout << "\n[2] Libros ordenados por codigo:\n";
    ordenar_libros(cab1);

    // Requisito 3: muestra los datos de las dos listas.
    cout << "\n[3] Listas de libros y prestamos:\n";
    imprimir_listas(cab1, cab2);

    // Requisito 4: el procedimiento imprime los codigos nunca prestados.
    cout << "\n[4] Libros que nunca se han prestado:\n";
    libros_nunca_prestados(cab1, cab2);

    // Requisito 5: solicita el codigo que se usara para buscar y contar prestamos.
    int codigo;
    cout << "\n[5] Ingrese el codigo del libro que desea consultar: ";
    if (!(cin >> codigo))
    {
        cerr << "Error: debe ingresar un codigo numerico.\n";
        liberar_listas(cab1, cab2);
        return 1;
    }

    // Se busca el libro para poder imprimir su nombre junto con el total de prestamos.
    Libro *libro_consultado = buscar_libro(cab1, codigo);
    if (libro_consultado == nullptr)
    {
        cout << "El libro con codigo " << codigo
             << " no existe en la biblioteca.\n";
    }
    else
    {
        int cantidad = veces_prestado(cab2, codigo);
        cout << "Codigo: " << libro_consultado->codigo_libro
             << " | Nombre: " << libro_consultado->nombre_libro
             << " | Veces prestado: " << cantidad << '\n';
    }

    // Libera todos los nodos antes de finalizar el programa.
    liberar_listas(cab1, cab2);
    return 0;
}

// Crea un nodo de libro y lo enlaza al final de la lista.
void insertar_libro(Libro *&cab, int codigo, const string &nombre)
{
    // El nuevo nodo inicia sin un siguiente nodo.
    Libro *nuevo = new Libro{codigo, nombre, nullptr};

    if (cab == nullptr)
    {
        cab = nuevo;
        return;
    }

    Libro *actual = cab;
    while (actual->sig != nullptr)
    {
        actual = actual->sig;
    }
    actual->sig = nuevo;
}

// Crea un nodo de prestamo con todos sus datos y lo enlaza al final de cab.
void insertar_prestamo(Prestamo *&cab, int codigo_libro, int codigo_estudiante, int dia, int mes, int anio)
{
    Prestamo *nuevo = new Prestamo{
        codigo_libro, codigo_estudiante, {dia, mes, anio}, nullptr};

    if (cab == nullptr)
    {
        cab = nuevo;
        return;
    }

    Prestamo *actual = cab;
    while (actual->sig != nullptr)
    {
        actual = actual->sig;
    }
    actual->sig = nuevo;
}

// Procedimiento SI-NO: compara libros contiguos e intercambia sus datos si estan desordenados.
void ordenar_libros(Libro *cab)
{
    // Una lista vacia o de un solo nodo ya esta ordenada.
    if (cab == nullptr || cab->sig == nullptr)
    {
        return;
    }

    bool intercambiado;
    do
    {
        intercambiado = false;
        Libro *actual = cab;
        while (actual->sig != nullptr)
        {
            if (actual->codigo_libro > actual->sig->codigo_libro)
            {
                // Intercambia codigo y nombre para mantener juntos los datos del libro.
                int codigo_temporal = actual->codigo_libro;
                string nombre_temporal = actual->nombre_libro;

                actual->codigo_libro = actual->sig->codigo_libro;
                actual->nombre_libro = actual->sig->nombre_libro;
                actual->sig->codigo_libro = codigo_temporal;
                actual->sig->nombre_libro = nombre_temporal;
                intercambiado = true;
            }
            actual = actual->sig;
        }
    } while (intercambiado);
}

// Procedimiento SI-NO: recorre e imprime la lista de libros y la de prestamos.
void imprimir_listas(Libro *cab1, Prestamo *cab2)
{
    cout << "\n--- LISTA DE LIBROS (cab1) ---\n";
    if (cab1 == nullptr)
    {
        cout << "No hay libros registrados.\n";
    }
    else
    {
        for (Libro *actual = cab1; actual != nullptr; actual = actual->sig)
        {
            cout << "Codigo: " << actual->codigo_libro
                 << " | Nombre: " << actual->nombre_libro << '\n';
        }
    }

    cout << "\n--- LISTA DE PRESTAMOS (cab2) ---\n";
    if (cab2 == nullptr)
    {
        cout << "No hay prestamos registrados.\n";
    }
    else
    {
        for (Prestamo *actual = cab2; actual != nullptr; actual = actual->sig)
        {
            cout << "Codigo libro: " << actual->codigo_libro
                 << " | Codigo estudiante: " << actual->codigo_estudiante
                 << " | Fecha: " << actual->fecha_prestamo.dia << '/'
                 << actual->fecha_prestamo.mes << '/'
                 << actual->fecha_prestamo.anio << '\n';
        }
    }
}

// Procedimiento SI-NO: imprime los libros que no tienen ningun prestamo asociado.
void libros_nunca_prestados(Libro *cab1, Prestamo *cab2)
{
    bool hay_libros_sin_prestamos = false;

    for (Libro *libro = cab1; libro != nullptr; libro = libro->sig)
    {
        // Para cada libro, busca si su codigo aparece en algun prestamo.
        bool prestado = false;
        for (Prestamo *prestamo = cab2; prestamo != nullptr; prestamo = prestamo->sig)
        {
            if (prestamo->codigo_libro == libro->codigo_libro)
            {
                prestado = true;
                break;
            }
        }

        if (!prestado)
        {
            cout << "Codigo: " << libro->codigo_libro
                 << " | Nombre: " << libro->nombre_libro << '\n';
            hay_libros_sin_prestamos = true;
        }
    }

    if (!hay_libros_sin_prestamos)
    {
        cout << "No hay libros sin prestamos.\n";
    }
}

// Funcion SI-SI: recibe un codigo y retorna cuantas veces fue prestado.
int veces_prestado(Prestamo *cab2, int codigo)
{
    int contador = 0;
    for (Prestamo *actual = cab2; actual != nullptr; actual = actual->sig)
    {
        if (actual->codigo_libro == codigo)
        {
            ++contador;
        }
    }
    return contador;
}

// Recorre cab1 y devuelve la direccion del libro con ese codigo, si existe.
Libro *buscar_libro(Libro *cab1, int codigo)
{
    for (Libro *actual = cab1; actual != nullptr; actual = actual->sig)
    {
        if (actual->codigo_libro == codigo)
        {
            return actual;
        }
    }
    return nullptr;
}

// Elimina uno por uno los nodos de las dos listas para evitar fugas de memoria.
void liberar_listas(Libro *&cab1, Prestamo *&cab2)
{
    while (cab1 != nullptr)
    {
        Libro *siguiente = cab1->sig;
        delete cab1;
        cab1 = siguiente;
    }

    while (cab2 != nullptr)
    {
        Prestamo *siguiente = cab2->sig;
        delete cab2;
        cab2 = siguiente;
    }
}
