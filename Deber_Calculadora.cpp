#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <limits>
#include <cstdlib>  

using namespace std;
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string BOLD = "\033[1m";
const string RESET = "\033[0m";

struct Calculadora {
    double numeros[2];
    double resultados[2];
    string operaciones[2];
    int opciones[2];
    string usuario;
};

double sumar(double a, double b) { return a + b; }
double restar(double a, double b) { return a - b; }
double multiplicar(double a, double b) { return a * b; }
double dividir(double a, double b) {
    if (b == 0) {
        cout << RED << "Error: Division entre cero no permitida." << RESET << endl;
        return 0;
    }
    return a / b;
}

long long mcd(long long a, long long b) {   
    if (b == 0) return llabs(a);          
    return mcd(b, a % b);
}

string decimalAFraccion(double valor) {
    int decimales = 4;  
    long long factor = 1;

    for (int i = 0; i < decimales; i++) {
        factor *= 10;
    }
    long long numerador = (long long) llround(valor * (double)factor);  
    long long denominador = factor;

    if (numerador == 0) return "0/1";
    long long divisor = mcd(numerador, denominador);
    numerador /= divisor;
    denominador /= divisor;
    if (denominador < 0) {
        denominador = -denominador;
        numerador = -numerador;
    }

    return to_string(numerador) + "/" + to_string(denominador);
}

string multiplicacionFraccion(double a, double b) {
    int decimales = 4;
    long long factor = 1;

    for (int i = 0; i < decimales; i++) {
        factor *= 10;
    }

    long long numA = (long long) llround(a * (double)factor);   
    long long numB = (long long) llround(b * (double)factor);   

    long long numerador = numA * numB;
    long long denominador = factor * factor;

    long long divisor = mcd(numerador, denominador);
    numerador /= divisor;
    denominador /= divisor;

    return to_string(numerador) + "/" + to_string(denominador);
}

string simboloOperacion(int opcion) {
    switch (opcion) {
        case 1: return " + ";
        case 2: return " - ";
        case 3: return " * ";
        case 4: return " / ";
        default: return " ? ";
    }
}

void mostrarCabecera() {
    cout << BOLD << MAGENTA << "\n=============================================" << RESET << endl;
    cout << BOLD << BLUE << "        PROYECTO CALCULADORA SIMPLE          " << RESET << endl;
    cout << MAGENTA << "        Universidad de las Fuerzas Armadas   " << RESET << endl;
    cout << MAGENTA << "              ESPE - 1er Semestre            " << RESET << endl;
    cout << MAGENTA << "        Estudiante: (Ingresa tu nombre)      " << RESET << endl;
    cout << BOLD << MAGENTA << "=============================================" << RESET << endl;
}

void ingresarUsuario(Calculadora &calc) {
    cout << CYAN << "Ingresa tu nombre (sin espacios): " << RESET;
    cin >> calc.usuario;  
}

double leerNumero() {
    double num;
    while (true) {
        cin >> num;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Entrada invalida. Ingresa un numero decimal: " << RESET;
        } else {
            return num;
        }
    }
}

void ingresarNumeros(Calculadora &calc) {
    cout << CYAN << "Ingresa el primer numero decimal: " << RESET;
    calc.numeros[0] = leerNumero();
    cout << CYAN << "Ingresa el segundo numero decimal: " << RESET;
    calc.numeros[1] = leerNumero();
}

void mostrarMenu() {
    cout << BOLD << BLUE << "\n======= MENU DE OPERACIONES =======" << RESET << endl;
    cout << GREEN << "1" << RESET << " - Sumar" << endl;
    cout << GREEN << "2" << RESET << " - Restar" << endl;
    cout << GREEN << "3" << RESET << " - Multiplicar" << endl;
    cout << GREEN << "4" << RESET << " - Dividir" << endl;
    cout << "===================================" << endl;
}

int seleccionarOperacion(int numOperacion) {
    int opcion;
    cout << YELLOW << "\nElige la operacion #" << numOperacion << " (1-4): " << RESET;
    cin >> opcion;
    while (opcion < 1 || opcion > 4) {
        cout << RED << "Opcion invalida. Intenta de nuevo (1-4): " << RESET;
        cin >> opcion;
    }
    return opcion;
}

void realizarOperacion(Calculadora &calc, int index, int opcion) {
    double a = calc.numeros[0];
    double b = calc.numeros[1];
    double resultado = 0.0;
    string fraccionResultado;

    calc.opciones[index] = opcion;

    switch (opcion) {
        case 1:
            resultado = sumar(a, b);
            fraccionResultado = decimalAFraccion(resultado);
            break;
        case 2:
            resultado = restar(a, b);
            fraccionResultado = decimalAFraccion(resultado);
            break;
        case 3:
            resultado = multiplicar(a, b);
            fraccionResultado = multiplicacionFraccion(a, b); 
            break;
        case 4:
            if (b == 0) {
                fraccionResultado = "Error: division entre cero";
                resultado = 0;
            } else {
                resultado = dividir(a, b);
                fraccionResultado = decimalAFraccion(resultado);
            }
            break;
    }
    calc.resultados[index] = resultado;
    calc.operaciones[index] = fraccionResultado;
}

void mostrarResumen(const Calculadora &calc) {
    cout << BOLD << "\n======= RESUMEN DE OPERACIONES =======" << RESET << endl;
    cout << "Estudiante: " << calc.usuario << endl;
    for (int i = 0; i < 2; i++) {
        cout << fixed << setprecision(4);
        cout << CYAN << (i + 1) << ". " << RESET
             << calc.numeros[0]
             << simboloOperacion(calc.opciones[i])
             << calc.numeros[1]
             << " = "
             << calc.operaciones[i]
             << endl;
    }
    cout << BOLD << "======================================" << RESET << endl;
}

bool deseaRepetir() {
    char respuesta;
    cout << YELLOW << "\nQuieres realizar otra sesion? (s/n): " << RESET;
    cin >> respuesta;
    return (respuesta == 's' || respuesta == 'S');
}

void pausa() {
    cout << GREEN << "\nPresiona ENTER para continuar..." << RESET << endl;
    cin.ignore();
    cin.get();
}

void lineaSeparadora() {
    cout << MAGENTA << "---------------------------------------------" << RESET << endl;
}

int main() {
    Calculadora calc;
    bool continuar = true;

    while (continuar) {
        mostrarCabecera();
        ingresarUsuario(calc);
        lineaSeparadora();
        ingresarNumeros(calc);
        for (int i = 0; i < 2; i++) {
            mostrarMenu();
            int opcion = seleccionarOperacion(i + 1);
            realizarOperacion(calc, i, opcion);
            lineaSeparadora();
        }
        mostrarResumen(calc);
        pausa();

        continuar = deseaRepetir();
    }
    
    cout << GREEN << "\nGracias por usar la calculadora. Proyecto finalizado." << RESET << endl;
    return 0;
}
