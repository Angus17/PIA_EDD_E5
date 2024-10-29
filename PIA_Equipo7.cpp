#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <cctype>
#include <clocale>
#include <regex>

#if defined(unix) || defined(__linux__)
    #include <stdio_ext.h>
#endif 

using namespace std;

struct Pila
{
    string nombre;
    double promedio_general;
    long int matricula;
    string direccion;
    string telefono;

    Pila *siguiente;
};

static void agregar_nodo_pila(Pila *&, const string &, const long int &, const string &, const string &, const double &);

class Lista
{
    public:
        string nombre;
        double promedio_general;
        Lista *siguiente;
        //Constructor
        Lista(string _nombre = "", double _promedio_general = 0.0L,  Lista *_siguiente = nullptr, long int _matricula = 0L, string _direccion = "", string _telefono = "") : nombre(_nombre), promedio_general(_promedio_general), siguiente(_siguiente), matricula(_matricula), direccion(_direccion), telefono(_telefono)
        {}; //Se construye cada nodo de la lista

        // ============================ GETTERS Y SETTERS ========================================================

        long int get_matricula() const
        {
            return matricula;
        }

        string get_direccion() const
        {
            return direccion;
        }

        string get_telefono() const
        {
            return telefono;
        }

        void set_matricula( long int &matricula )
        {
            this->matricula = matricula;
        }

        void set_direccion( string &direccion )
        {
            this->direccion = direccion;
        }

        void set_telefono( string &telefono)
        {
            this->telefono = telefono;
        }

        // =======================================================================================================

        // ============================================== MÉTODOS =============================================================

        static void agregar_nodo( Lista *& lista_completa, string nombre, long int matricula, string direccion, string telefono, double promedio_general )
        {
            Lista *nuevo_nodo = new Lista( nombre, promedio_general, nullptr, matricula, direccion, telefono);

            if ( lista_completa == nullptr )

                lista_completa = nuevo_nodo;

            else
            {
                Lista *actual = lista_completa;

                while ( actual->siguiente != nullptr )

                    actual = actual->siguiente;

                actual->siguiente = nuevo_nodo;
            }
            
        }

        static void mostrar_lista( Lista *lista_completa )
        {
            Lista *actual = lista_completa;

            cout << "---------------------------------------------------------" << endl;
            cout << "Nombre | Matricula | Dirección | Teléfono | Promedio general" << endl;
            cout << "---------------------------------------------------------" << endl;
            while ( actual!= nullptr )
            {
                cout << "Nombre: " << actual->nombre << endl;
                cout << "Matricula: " << actual->matricula << endl;
                cout << "Dirección: " << actual->direccion << endl;
                cout << "Teléfono: " << actual->telefono << endl;
                cout << "Promedio general: " << fixed << setprecision(2) << actual->promedio_general << endl;
                cout << "---------------------------------------------------------" << endl;
                actual = actual->siguiente;
            }
        }

        static void eliminar_nodo( Lista *& lista_completa, Pila *& pila_alumnos_eliminados, long int *id_number, string *name )
        {
            Lista *auxiliar = lista_completa;
            Lista *anterior = nullptr;
            Lista *auxiliar_pila = nullptr;

            if ( id_number != nullptr )
            {
                while ((auxiliar != nullptr) && (auxiliar->get_matricula() != *id_number))
                {
                    anterior = auxiliar;
                    auxiliar = auxiliar->siguiente;
                    
                }
            }
            else
            {
                while ((auxiliar != nullptr) && (auxiliar->nombre != *name))
                {
                    anterior = auxiliar;
                    auxiliar = auxiliar->siguiente;
                    
                }
            }

            if (auxiliar == nullptr) // No se encontró el nodo

                cout << "No fué encontrado el estudiante. . ." << endl;

            else if (anterior == nullptr) // Es el primer nodo
                {
                    auxiliar_pila = lista_completa;
                    lista_completa = lista_completa->siguiente;
                    agregar_nodo_pila(pila_alumnos_eliminados, auxiliar_pila->nombre, auxiliar_pila->get_matricula(), auxiliar_pila->get_direccion(), auxiliar_pila->get_telefono(), auxiliar_pila->promedio_general);
                    delete auxiliar;
                }
                else  // Es el ultimo nodo o cualquiera que no es el primero
                {
                    auxiliar_pila = anterior->siguiente;
                    anterior->siguiente = auxiliar->siguiente;
                    agregar_nodo_pila(pila_alumnos_eliminados, auxiliar_pila->nombre, auxiliar_pila->get_matricula(), auxiliar_pila->get_direccion(), auxiliar_pila->get_telefono(), auxiliar_pila->promedio_general);
                    delete auxiliar;
                }
            
        }

    private: // Atributos encapsulados
        long int matricula;
        string direccion;
        string telefono;
};

static void eliminar_nodo_pila(Pila *&, Lista *&);

static void dar_alta_alumnos( Lista *& );
static void dar_baja_alumnos( Lista *&, Pila *& );
static void lanzar_reportes( Lista *, Pila *);
static void recuperar_alumno( Pila *&, Lista *& );
static void modificar_datos( Lista *& );
static void crear_grupo( Lista * );            

static void cambiar_informacion( Lista *&, Lista *& );
static Lista *obtener_nodo( Lista *, const int & );
static void buscar_alumnos( Lista *&, long int *, string *);
static void ordenar_lista( Lista *& );
static Lista *&insertion_sort( Lista *&, Lista *& );

// =========== FUNCIONES PARA LOS REPORTES =================================

static void lanzar_reporte_aprobados( Lista * );
static void lanzar_reporte_aprobados_reprobados( Lista * );
static void lanzar_reporte_promedio_general( Lista *, double & );
static void lanzar_reporte_datos_generales( Lista * );
static void lanzar_reporte_inactivos( Pila * );

// =========================================================================


static void limpiar_buffer_STDIN();
static void limpiar_pantalla();
static void pausar_terminal();
static void mostrar_mensaje_error();


int main()
{
    int opcion;
    Lista *lista = nullptr;
    Pila *pila_descartados = nullptr;

    setlocale( LC_CTYPE, "es_MX.UTF-8" );

    do
    {
        do
        {
            limpiar_pantalla();

            cout << setw(70) << "**********************************************************\n" << endl;
            cout << setw(66) << "EDUCACIÓN PARA EL ÉXITO: MENÚ DE OPCIONES\n" << endl ;
            cout << setw(70) << "**********************************************************\n" << endl;
            cout << setw(43) <<"1. Dar de alta alumnos" << endl;
            cout << setw(43) <<"2. Dar de baja alumnos" << endl;
            cout << setw(41) <<"3. Recuperar alumnos" << endl;
            cout << setw(39) <<"4. Lanzar reportes" << endl;
            cout << setw(39) <<"5. Modificar datos" << endl;
            cout << setw(35) <<"6. Crear grupo" << endl;
            cout << setw(29) <<"7. Salir" << endl;
            cout << setw(50) <<"\nIngrese la opción deseada: ";
            limpiar_buffer_STDIN();
            cin >> opcion;

        } while ( opcion < 1 || opcion > 7 );

        limpiar_pantalla();

        switch ( opcion )
        {
            case 1:

                dar_alta_alumnos(lista);

                if ( lista->siguiente != nullptr )

                    ordenar_lista( lista );

            break;

            case 2:
                
                if ( lista == nullptr )

                    cout << "No hay registro de alumnos. . ." << endl;

                else

                    dar_baja_alumnos(lista, pila_descartados);

            break;

            case 3:

                if ( pila_descartados == nullptr )

                    cout << "No hay nada que recuperar, papelera vacía . . ." << endl;

                else
                {
                    recuperar_alumno(pila_descartados, lista);

                    if ( lista->siguiente != nullptr )

                        ordenar_lista( lista );
                    
                }
            break;

            case 4:
                
                if ( lista == nullptr )

                    cout << "No hay datos por los cuales lanzar reportes. . ." << endl;

                else

                    lanzar_reportes( lista, pila_descartados );

            break;

            case 5:

                if ( lista == nullptr )

                    cout << "No hay registro de alumnos. . ." << endl;

                else

                    modificar_datos( lista );

            break;

            case 6:
                if ( lista == nullptr )

                    cout << "No hay registro de alumnos. . ." << endl;

                else

                    crear_grupo( lista );
                
            break;

            case 7:

                while ( lista != nullptr )
                {
                    delete lista;
                    lista = lista->siguiente;
                }

                while ( pila_descartados != nullptr )
                {
                    delete pila_descartados;
                    pila_descartados = pila_descartados->siguiente;
                }
                
                
            break;
        }

        if ( opcion != 7 )

            pausar_terminal();

    } while ( opcion != 7 );

    return EXIT_SUCCESS;
}

static void dar_alta_alumnos( Lista *& lista )
{
    string name, address, cellphone;
    double average_grade;
    long int id;
    bool expresion_valida, dato_incorrecto;
    string respuesta;

    regex patron_direccion("(Calle|Av.) ([a-zA-ZÁ-Ý\u00f1\u00d1. ]+) #([0-9]+) (Colonia|Fracc.|Privada) ([a-zA-ZÁ-Ý\u00f1\u00d1 ]+), ([A-Za-zÁ-Ý\u00f1\u00d1 ]+), ([A-Za-zÁ-Ý\u00f1\u00d1 ]+)");
    regex patron_nombres("([ a-zA-ZÁ-Ý\u00f1\u00d1]+)");
    regex patron_telefono(R"(\d{2}-\d{4}-\d{4})");
    regex patron_respuesta("(si|no)");

    do
    {
        do
        {
            limpiar_pantalla();

            cout << "\nIngrese el nombre(s) y apellido(s) del alumno: ";
            limpiar_buffer_STDIN();
            getline(cin, name);

            expresion_valida = regex_match(name, patron_nombres);

            if (!expresion_valida)

                mostrar_mensaje_error();

        } while ( !expresion_valida );

        do
        {
            limpiar_pantalla();

            cout << "\nIngrese la matrícula del alumno: ";
            limpiar_buffer_STDIN();
            cin >> id;

            if ( ( dato_incorrecto = cin.fail() ) )
            {
                mostrar_mensaje_error();
                cin.clear();
            }
            
        } while ( dato_incorrecto );
        
        do
        {
            limpiar_pantalla();

            cout << "\nIngrese la dirección del alumno: " << endl;
            cout << "(Calle|Av.) (Nombre_calle) #(Numero) (Colonia|Fracc.|Privada) (Nombre_colonia), Municipio, Estado" << endl;
            cout << ": " ;
            limpiar_buffer_STDIN();
            getline(cin, address);

            expresion_valida = regex_match(address, patron_direccion);

            if (!expresion_valida)

                mostrar_mensaje_error();
                
        } while ( !expresion_valida );

        do
        {
            limpiar_pantalla();

            cout << "\nIngrese el teléfono del alumno: " << endl;
            cout << "Siguiendo el formato ##-####-####" << endl;
            cout << ": " ;
            limpiar_buffer_STDIN();
            getline(cin, cellphone);

            expresion_valida = regex_match(cellphone, patron_telefono);

            if (!expresion_valida)

                mostrar_mensaje_error();
        } while ( !expresion_valida );

        do
        {
            limpiar_pantalla();

            cout << "\nIngrese el promedio general del alumno: ";
            limpiar_buffer_STDIN();
            cin >> average_grade;

            if ( ( dato_incorrecto = cin.fail() ) )
            {
                mostrar_mensaje_error();
                cin.clear();
            }
        } while ( dato_incorrecto || ( average_grade < 0.0L || average_grade > 100.0L ) );
        
        lista->agregar_nodo( lista, name, id, address, cellphone, average_grade );

        do
        {
            limpiar_pantalla();

            cout << "\n¿Desea agregar otro alumno? (si/no): ";
            limpiar_buffer_STDIN();
            getline(cin, respuesta);

            expresion_valida = regex_match(respuesta, patron_respuesta);

            if (!expresion_valida)

                mostrar_mensaje_error();

        } while ( !expresion_valida );
        
    } while ( respuesta == "si" );
    
}

static void dar_baja_alumnos( Lista *& lista, Pila *& pila_eliminados)
{
    int opcion;
    long int id;
    string name;

    do
    {
        do
        {
            limpiar_pantalla();

            cout << "\n1. Eliminar por matrícula" << endl;
            cout << "2. Eliminar por nombre" << endl;
            cout << "3. Volver al menú principal" << endl;
            cout << "\nIngrese la opción deseada: ";
            limpiar_buffer_STDIN();
            cin >> opcion;
            
        } while ( opcion < 1 || opcion > 3 );
        

        switch ( opcion )
        {
            case 1:

                do
                {
                    limpiar_pantalla();

                    cout << "\nIngrese la matrícula del alumno a eliminar: ";
                    limpiar_buffer_STDIN();
                    cin >> id;

                } while ( id < 0L );

                lista->eliminar_nodo(lista, pila_eliminados, &id, nullptr);
                
            break;

            case 2:

                do
                {
                    limpiar_pantalla();

                    cout << "\nIngrese el nombre del alumno a eliminar: ";
                    limpiar_buffer_STDIN();
                    getline(cin, name);

                } while ( name.empty() );

                lista->eliminar_nodo(lista, pila_eliminados, nullptr, &name);

            break;

        }

        if ( opcion!= 3 )

            pausar_terminal();

    } while ( opcion != 3 );
    

}

static void lanzar_reportes( Lista *lista, Pila *eliminados)
{
    char opcion;
    double promedio;
    bool dato_incorrecto;

    do
    {
        do
        {
            limpiar_pantalla();

            cout << "a) Reporte de alumnos aprobados " << endl;
            cout << "b) %" << " de aprobados y " << "% " << "de reprobados" << endl;
            cout << "c) Reporte de calificaciones iguales o superiores a la ingresada" << endl;
            cout << "d) Reporte de datos generales" << endl;
            cout << "e) Reporte de alumnos inactivos " << endl;
            cout << "f) Regrear al menú principal " << endl;

            limpiar_buffer_STDIN();

            cin.get( opcion );
            
        } while ( opcion < 'a' || opcion > 'f' );

        limpiar_pantalla();

        switch ( opcion )
        {
            case 'a':
            // Reporte de alumnos aprobados

                lanzar_reporte_aprobados( lista );

            break;

            case 'b':
            // Porcentaje de aprobados y reprobados

                lanzar_reporte_aprobados_reprobados( lista );

            break;

            case 'c':
            // Reporte de calificaciones iguales o superiores a la ingresada

                do
                {
                    limpiar_pantalla();

                    cout << "\nIngrese el promedio general del alumno: ";
                    limpiar_buffer_STDIN();
                    cin >> promedio;

                    if ( ( dato_incorrecto = cin.fail() ) )
                    {
                        mostrar_mensaje_error();
                        cin.clear();
                    }
                } while ( dato_incorrecto || ( promedio < 0.0L || promedio > 100.0L ) );

                lanzar_reporte_promedio_general( lista, promedio );

            break;

            case 'd':

            // Reporte de datos generales
                if ( lista != nullptr )

                    lanzar_reporte_datos_generales(lista);

            break;

            case 'e':

            // Reporte de alumnos inactivos

                if ( eliminados != nullptr )

                    lanzar_reporte_inactivos( eliminados );

                else

                    cout << "\nNo hay alumnos dados de baja. . ." << endl;

            break;
        }

        if ( opcion != 'f' )

            pausar_terminal();

    } while ( opcion != 'f' );
}

static void recuperar_alumno( Pila *&eliminados, Lista *&lista_completa )
{
    int opcion;
    string por_nombre;
    long int por_matricula;
    bool dato_incorrecto, expresion_valida, encontrado = false;

    Pila *auxiliar_pila = eliminados;
    Pila *temporal_pila = nullptr;

    regex patron_nombres("([ a-zA-ZÁ-Ý\u00f1\u00d1]+)");

    do
    {
        do
        {
            limpiar_pantalla();
            cout << "Buscar alumno por su:" << endl;
            cout << "1. Matrícula" << endl;
            cout << "2. Nombre" << endl;
            cout << "3. Volver al menú principal" << endl;
            cout << "Ingrese su opción: ";
            limpiar_buffer_STDIN();
            cin >> opcion;

        } while ( opcion < 1 || opcion > 3 );

        limpiar_pantalla();

        switch ( opcion )
        {
            case 1:
                do
                {
                    cout << "Ingrese la matrícula del alumno que desea recuperar: ";
                    limpiar_buffer_STDIN();
                    cin >> por_matricula;

                    if ( ( dato_incorrecto = cin.fail() ) )
                    {
                        mostrar_mensaje_error();
                        cin.clear();
                    }

                } while ( por_matricula < 0 || dato_incorrecto );

                while ( auxiliar_pila != nullptr && !encontrado )
                {
                    if ( auxiliar_pila->matricula == por_matricula )

                        encontrado = true;
                    else

                        agregar_nodo_pila( temporal_pila, auxiliar_pila->nombre, auxiliar_pila->matricula, auxiliar_pila->direccion, auxiliar_pila->telefono, auxiliar_pila->promedio_general );


                    auxiliar_pila = auxiliar_pila->siguiente;
                }

                if ( encontrado )
                {
                    eliminar_nodo_pila( auxiliar_pila, lista_completa );

                    while ( temporal_pila != nullptr )
                    {
                        agregar_nodo_pila( eliminados, temporal_pila->nombre, temporal_pila->matricula, temporal_pila->direccion, temporal_pila->telefono, temporal_pila->promedio_general );
                        temporal_pila = temporal_pila->siguiente;
                    }

                    cout << "Alumno recuperado exitosamente!. . ." << endl;
                }
                else
                {

                    while ( temporal_pila != nullptr )
                    {
                        agregar_nodo_pila( eliminados, temporal_pila->nombre, temporal_pila->matricula, temporal_pila->direccion, temporal_pila->telefono, temporal_pila->promedio_general );
                        temporal_pila = temporal_pila->siguiente;
                    }

                    cout << "El alumno no ha sido encontrado. . ." << endl;
                    
                }

            break;

            case 2:

                do
                {
                    limpiar_pantalla();
                    cout << "Ingrese el nombre del alumno que desea recuperar: ";
                    limpiar_buffer_STDIN();
                    getline(cin, por_nombre);

                    expresion_valida = regex_match( por_nombre, patron_nombres );

                    if ( !expresion_valida ) 

                        mostrar_mensaje_error();

                } while ( !expresion_valida );

                while ( auxiliar_pila != nullptr && !encontrado )
                {
                    if ( auxiliar_pila->nombre == por_nombre )

                        encontrado = true;

                    else

                        agregar_nodo_pila( temporal_pila, auxiliar_pila->nombre, auxiliar_pila->matricula, auxiliar_pila->direccion, auxiliar_pila->telefono, auxiliar_pila->promedio_general );


                    auxiliar_pila = auxiliar_pila->siguiente;
                }

                if ( encontrado )
                {
                    eliminar_nodo_pila( auxiliar_pila, lista_completa );

                    while ( temporal_pila != nullptr )
                    {
                        agregar_nodo_pila( eliminados, temporal_pila->nombre, temporal_pila->matricula, temporal_pila->direccion, temporal_pila->telefono, temporal_pila->promedio_general );
                        temporal_pila = temporal_pila->siguiente;
                    }
                    
                    cout << "Alumno recuperado exitosamente!. . ." << endl;
                }
                else
                {

                    while ( temporal_pila != nullptr )
                    {
                        agregar_nodo_pila( eliminados, temporal_pila->nombre, temporal_pila->matricula, temporal_pila->direccion, temporal_pila->telefono, temporal_pila->promedio_general );
                        temporal_pila = temporal_pila->siguiente;
                    }

                    cout << "El alumno no ha sido encontrado. . ." << endl;
                    
                }

            break;

        }

        if ( opcion != 3 )

            pausar_terminal();

    } while ( opcion != 3 );
}

static void ordenar_lista( Lista *& lista )
{
    // Se implementa algoritmo Insertion Sort

    Lista *auxiliar = lista;
    Lista *sorted = nullptr;
    Lista *next = nullptr;

    while ( auxiliar != nullptr )
    {
        next = auxiliar->siguiente;

        sorted = insertion_sort( sorted, auxiliar );

        auxiliar = next;
    }

    lista = sorted;

}

static Lista *&insertion_sort(Lista *& lista, Lista *& nodo)
{
    // No hay nada que ordenar, ya está ordenado
    if (lista == nullptr || lista->get_matricula() >= nodo->get_matricula())
    {
        nodo->siguiente = lista;
        return nodo;
    }

    // Encontrar la posición correcta para insertar el nodo
    Lista *actual = lista;
    while (actual->siguiente != nullptr && actual->siguiente->get_matricula() < nodo->get_matricula())

        actual = actual->siguiente;

    // Insertar el nuevo nodo en la posición correcta
    nodo->siguiente = actual->siguiente;
    actual->siguiente = nodo;

    return lista;
}

static void agregar_nodo_pila( Pila *& pila, const string &nombre, const long int &matricula, const string &direccion, const string &telefono, const double &promedio_general )
{
    Pila *nuevo_nodo = new Pila( );

    nuevo_nodo->nombre = nombre;
    nuevo_nodo->matricula = matricula;
    nuevo_nodo->direccion = direccion;
    nuevo_nodo->telefono = telefono;
    nuevo_nodo->promedio_general = promedio_general;

    nuevo_nodo->siguiente = pila;
    pila = nuevo_nodo;
}

static void eliminar_nodo_pila(Pila *&actual, Lista *& lista_completa)
{
    Pila *auxiliar = actual;
    actual = actual->siguiente;
    lista_completa->agregar_nodo(lista_completa, auxiliar->nombre, auxiliar->matricula, auxiliar->direccion, auxiliar->telefono, auxiliar->promedio_general);
    delete auxiliar;
}

static void lanzar_reporte_aprobados( Lista *lista_completa )
{
    Lista *auxiliar = lista_completa;

    while ( auxiliar != nullptr )
    {
        if ( auxiliar->promedio_general >= 70.0L )
        {
            cout << "\nNombre: " << auxiliar->nombre << endl;
            cout << "Matrícula: " << auxiliar->get_matricula() << endl;
            cout << "Promedio general: " << fixed << setprecision(2) << auxiliar->promedio_general << endl << endl;
        }

        auxiliar = auxiliar->siguiente;
    }
    

}

static void lanzar_reporte_aprobados_reprobados( Lista *lista_completa )
{
    double porcentaje_reprobados, porcentaje_aprobados;
    int aprobados = 0, reprobados = 0;

    Lista *auxiliar = lista_completa;

    while ( auxiliar != nullptr )
    {
        if ( auxiliar->promedio_general >= 70.0L )

            aprobados++;

        else

            reprobados++;


        auxiliar = auxiliar->siguiente;
    }
    

    porcentaje_aprobados =  static_cast<double>( aprobados ) / (aprobados + reprobados) * 100;
    porcentaje_reprobados = static_cast<double>( reprobados ) / (aprobados + reprobados) * 100;
    
    cout << "\nPorcentaje de alumnos aprobados: " << fixed << setprecision(2) << porcentaje_aprobados << " %" << endl;
    cout << "Porcentaje de alumnos reprobados: " << fixed << setprecision(2) << porcentaje_reprobados << " %" << endl;

}

static void lanzar_reporte_promedio_general( Lista *lista_completa, double &promedio_ingresado )
{
    Lista *auxiliar = lista_completa;

    while ( auxiliar != nullptr )
    {
        if ( auxiliar->promedio_general >= promedio_ingresado )
        {
            cout << "\nNombre: " << auxiliar->nombre << endl;
            cout << "Promedio general: " << fixed << setprecision(2) << auxiliar->promedio_general << endl << endl;
        }
        
        auxiliar = auxiliar->siguiente;
    }
    

}

static void lanzar_reporte_datos_generales( Lista *lista_completa )
{
    lista_completa->mostrar_lista( lista_completa );
}

static void lanzar_reporte_inactivos( Pila *pila_eliminados )
{
    Pila *auxiliar = pila_eliminados;

    while ( auxiliar != nullptr )
    {
        cout << "\nNombre: " << auxiliar->nombre << endl;
        cout << "Matrícula: " << auxiliar->matricula << endl;
        
        auxiliar = auxiliar->siguiente;
    }
    

}

static void modificar_datos( Lista *&lista_completa )
{
    int opcion;
    long int por_matricula;
    string por_nombre;
    bool expresion_valida;

    regex patron_nombres("([ a-zA-ZÁ-Ý\u00f1\u00d1]+)");

    do
    {
        do
        {
            limpiar_pantalla();
            cout << "Buscar alumno por su:" << endl;
            cout << "1. Matrícula" << endl;
            cout << "2. Nombre" << endl;
            cout << "3. Volver al menú principal" << endl;
            cout << "Ingrese su opción: ";
            limpiar_buffer_STDIN();
            cin >> opcion;

        } while ( opcion < 1 || opcion > 3 );

        limpiar_pantalla();

        switch ( opcion )
        {
            case 1:
                do
                {
                    cout << "Ingrese la matrícula del alumno que desea modificar: ";
                    limpiar_buffer_STDIN();
                    cin >> por_matricula;

                } while ( por_matricula < 0 );

                buscar_alumnos( lista_completa, &por_matricula, nullptr );

            break;

            case 2:

                do
                {
                    limpiar_pantalla();
                    cout << "Ingrese el nombre del alumno que desea modificar: ";
                    limpiar_buffer_STDIN();
                    getline(cin, por_nombre);

                    expresion_valida = regex_match( por_nombre, patron_nombres );

                    if ( !expresion_valida ) 

                        mostrar_mensaje_error();

                } while ( !expresion_valida );

                buscar_alumnos( lista_completa, nullptr, &por_nombre );

            break;

        }

        if ( opcion != 3 )

            pausar_terminal();

    } while ( opcion != 3 );
    

}

static void crear_grupo( Lista *lista_completa )
{
    // Pendiente

    Lista *auxiliar = lista_completa;
    int contador_nodos = 0;
    int numero_grupos, nodos_por_grupo, i, j, nodos_sobrantes, nodos_grupo_actual;

    while ( auxiliar != nullptr )
    {
        contador_nodos++;
        auxiliar = auxiliar->siguiente;
    }

    do
    {
        limpiar_pantalla();
        cout << "Ingrese el número de grupos que desea crear: ";
        limpiar_buffer_STDIN();
        cin >> numero_grupos;

    } while ( numero_grupos > contador_nodos || numero_grupos <= 0 );
    
    nodos_por_grupo = contador_nodos / numero_grupos;
    nodos_sobrantes = contador_nodos % numero_grupos;

    auxiliar = lista_completa;

    for ( i = 0; i < numero_grupos; i++)
    {
        nodos_grupo_actual = nodos_por_grupo + ( i < nodos_sobrantes ? 1 : 0 ); // Dados los alumnos sobrantes, se añaden a los primeros grupos

        cout << "\nGrupo " << i + 1 << ":" << endl;

        for ( j = 0; j < nodos_grupo_actual && auxiliar != nullptr; j++)
        {
            cout << "Nombre: " << auxiliar->nombre << endl;
            cout << "Matrícula: " << auxiliar->get_matricula() << endl;
            cout << "Promedio general: " << fixed << setprecision(2) << auxiliar->promedio_general << endl << endl;

            auxiliar = auxiliar->siguiente;
        }
        
    }
}

static void buscar_alumnos( Lista *&lista, long int *id_number, string *name)
{
    Lista *auxiliar = lista;

    if ( id_number != nullptr )
    {
        // Busqueda binaria para matriculas
        int contador_nodos = 0;
        int inicio = 0;
        int fin = 0;
        
        // Bucle contador de nodos
        while ( auxiliar != nullptr )
        {
            contador_nodos++;
            auxiliar = auxiliar->siguiente;
        }

        auxiliar = lista;

        fin = contador_nodos - 1;

        while ( inicio <= fin )
        {
            int medio = ( inicio + fin ) / 2;
            Lista *nodo_medio = obtener_nodo( auxiliar, medio );

            if (nodo_medio == nullptr)
            {
                limpiar_pantalla();
                cout << "No se encontró al alumno con la matrícula ingresada. . ." << endl;
                return; // No se encontró el alumno
            }
            else if (nodo_medio->get_matricula() == *id_number)
                {
                    // Nodo encontrado

                    cambiar_informacion( nodo_medio, lista );            
                    return;
                }
                else if (nodo_medio->get_matricula() < *id_number) 

                        inicio = medio + 1; // Buscar en la mitad derecha

                    else 

                        fin = medio - 1; // Buscar en la mitad izquierda

        }
    }
    else
    {
        while ( auxiliar != nullptr )
        {
            if (auxiliar->nombre == *name )
            {
                // Nodo encontrado

                cambiar_informacion( auxiliar, lista );
                return;
            }

            auxiliar = auxiliar->siguiente;
        }
        
    }
    
}

static Lista *obtener_nodo( Lista *lista, const int &posicion_actual )
{
    int i;

    for ( i = 0; i < posicion_actual && lista != nullptr; i++)

        lista = lista->siguiente;

    return lista;
}

static void cambiar_informacion( Lista *&nodo, Lista *&lista_completa )
{

    int opcion;

    string name, address, cellphone;
    double average_grade;
    long int id;
    bool expresion_valida, dato_incorrecto;
    string respuesta;

    regex patron_respuesta("(si|no)");
    regex patron_nombres("([ a-zA-ZÁ-Ý\u00f1\u00d1]+)");
    regex patron_direccion("(Calle|Av.) ([a-zA-ZÁ-Ý\u00f1\u00d1. ]+) #([0-9]{1,4}) (Colonia|Fracc.|Privada) ([a-zA-ZÁ-Ý\u00f1\u00d1 ]+), ([A-Za-zÁ-Ý\u00f1\u00d1 ]+), ([A-Za-zÁ-Ý\u00f1\u00d1 ]+)");
    regex patron_telefono(R"(\d{2}-\d{4}-\d{4})");

    do
    {
        do
        {
            limpiar_pantalla();

            cout << "¿Qué información desea cambiar?" << endl;
            cout << "1. Nombre" << endl;
            cout << "2. Matrícula" << endl;
            cout << "3. Dirección" << endl;
            cout << "4. Teléfono" << endl;
            cout << "5. Promedio general" << endl;
            cout << "6. Volver al menú principal" << endl;
            cout << "Ingrese su opción: ";
            limpiar_buffer_STDIN();
            cin >> opcion;

        } while ( opcion < 1 || opcion > 6 );

        limpiar_pantalla();

        switch (opcion)
        {
            case 1:

                do
                {
                    limpiar_pantalla();

                    cout << "\nIngrese el nombre(s) y apellido(s) del alumno: ";
                    limpiar_buffer_STDIN();
                    getline(cin, name);

                    expresion_valida = regex_match(name, patron_nombres);

                    if (!expresion_valida)

                        mostrar_mensaje_error();

                } while ( !expresion_valida );

                nodo->nombre = name;

            break;

            case 2:
                do
                {
                    limpiar_pantalla();

                    cout << "\nIngrese la matrícula del alumno: ";
                    limpiar_buffer_STDIN();
                    cin >> id;

                    if ( ( dato_incorrecto = cin.fail() ) )
                    {
                        mostrar_mensaje_error();
                        cin.clear();
                    }
                    
                } while ( dato_incorrecto );

                nodo->set_matricula( id );

            break;

            case 3:

                do
                {
                    limpiar_pantalla();

                    cout << "\nIngrese la dirección del alumno: ";
                    limpiar_buffer_STDIN();
                    getline(cin, address);

                    expresion_valida = regex_match(address, patron_direccion);

                    if (!expresion_valida)

                        mostrar_mensaje_error();
                        
                } while ( !expresion_valida );

                nodo->set_direccion( address );
        
            break;

            case 4:

                do
                {
                    limpiar_pantalla();

                    cout << "\nIngrese el teléfono del alumno: ";
                    limpiar_buffer_STDIN();
                    getline(cin, cellphone);

                    expresion_valida = regex_match(cellphone, patron_telefono);

                    if (!expresion_valida)

                        mostrar_mensaje_error();
                } while ( !expresion_valida );

                nodo->set_telefono( cellphone );

            break;

            case 5:

                do
                {
                    limpiar_pantalla();

                    cout << "\nIngrese el promedio general del alumno: ";
                    limpiar_buffer_STDIN();
                    cin >> average_grade;

                    if ( ( dato_incorrecto = cin.fail() ) )
                    {
                        mostrar_mensaje_error();
                        cin.clear();
                    }
                } while ( dato_incorrecto || ( average_grade < 0.0L || average_grade > 100.0L ) );

                nodo->promedio_general = average_grade;

            break;
        }

        if ( opcion != 6 )
        {
            cout << "INFORMACIÓN ACTUALIZADA EXITOSAMENTE!" << endl;

            pausar_terminal();

            do
            {
                limpiar_pantalla();

                cout << "¿Desea realizar otro cambio? (si/no): ";
                limpiar_buffer_STDIN();
                getline(cin, respuesta);
                expresion_valida = regex_match( respuesta, patron_respuesta );

                if ( !expresion_valida )

                    mostrar_mensaje_error();

            } while ( !expresion_valida );
        }
        else if ( opcion == 2 && lista_completa->siguiente != nullptr )

                ordenar_lista( lista_completa );

    } while ( opcion != 6 && respuesta == "si" );
}

static void limpiar_buffer_STDIN()
{
    #if defined(_WIN32) || defined(_WIN64)
        rewind(stdin);
    #elif defined(unix)
        __fpurge(stdin);
    #endif
}

static void limpiar_pantalla()
{
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #elif defined(unix)
        system("clear");
    #endif
}

static void pausar_terminal()
{
    #if defined(_WIN32) || defined(_WIN64)
        system("pause");
    #elif defined(unix)
        cout << "Presione ENTER para continuar. . . ." << endl;
        limpiar_buffer_STDIN();
        cin.get();
        limpiar_pantalla();
    #endif


}

static void mostrar_mensaje_error()
{
    cerr << "ERROR DE CAMPO, ingrese otro valor válido. . . ." << endl ;
    limpiar_buffer_STDIN();
    cin.get();
}