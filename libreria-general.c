//-------INCLUDES-------//
#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/* #include <conio.h> */
//-----DEFINES-----//
#define NOMBRE_ARCHIVO_BIN "competidores.dat"
#define NOMBRE_ARCHIVO_TXT "corredores_v1.csv"
//-----STRUCTS-----//
struct Fechas
{
    int dia, ano;
    char mes[3];
};

struct Competidores
{
    int index, id, edad, activo;
    struct Fechas fecha;
    char pais[3];
    float tiempo;
};
// Se definen los errores que pueden surgir
char error[][100] = {
    "Error abriendo el archivo",   // En caso de que falle el fopen
    "Ingrese una opcion valida\n", // En caso de que el usuario ingrese una opcion no esperada
    "ERROR, Ingrese un numero porfavor: ",
};

// FUNCIONES GENERALES
//  Se recorre el string y se pasa letra a letra a mayuscula
void mayus(char *c)
{
    for (int i = 0; i < strlen(c); i++)
    {
        c[i] = toupper(c[i]);
    }

    return;
}
// Lo mismo que en mayus pero con tolower
void min(char *c)
{
    for (int i = 0; i < strlen(c); i++)
    {
        c[i] = tolower(c[i]);
    }

    return;
}

// Check num
void readInt(int *num)
{
    int a = scanf("%d", num);
    fflush(stdin);
    while (a != 1)
    {
        printf("%s", error[2]);
        a = scanf("%d", num);
    }
    return;
}
void readFloat(float *num)
{
    int a = scanf("%f", num);
    fflush(stdin);
    while (a != 1)
    {
        printf("%s", error[2]);
        a = scanf("%f", num);
    }
    return;
}

// Imprime todos los datos de un competidor
void imprimirCompetidor(struct Competidores competidor)
{
    printf("---------------------------\n");
    printf("Nro de competidor: %d\n", competidor.index);
    printf("ID               : %d\n", competidor.id);
    printf("Fecha            : %d/%s/%d\n", competidor.fecha.dia, competidor.fecha.mes, competidor.fecha.ano);
    printf("Edad             : %d\n", competidor.edad);
    printf("Pais             : %s\n", competidor.pais);
    printf("Tiempo           : %.6f\n", competidor.tiempo);
    printf("Activo           : %d\n", competidor.activo);
    printf("---------------------------\n\n");
    return;
}
// Editar al competidor
void editarComp(struct Competidores *competidor)
{
    char c;
    int edad;
    do
    {
        // Se muestran los datos del competidor
        printf("Competidor a modificar: \n");
        imprimirCompetidor(*competidor);
        printf("Que desea modificar\n");
        printf("Presione 'e' para la edad o 't' para el tiempo (c para cancelar): ");
        scanf("%c", &c);
        fflush(stdin);
        // Se pasa a minuscula
        min(&c);
        switch (c)
        {
        case 'e':
            printf("Ingrese la nueva edad: ");
            readInt(&competidor->edad);
            return;
        case 't':
            printf("Ingrese el nuevo tiempo: ");
            readFloat(&competidor->tiempo);
            return;
        case 'c':
            break;
        default:
            printf("%s", error[1]);
            editarComp(competidor);
        }
    } while (c != 'c');

    return;
}

// Pasar mes en numero a mes en char
void mesToChar(char *mes)
{
    // Se pasa el mes a char para mantener coherencia
    if (strcmp(mes, "1") == 0)
    {
        strcpy(mes, "ene");
    }
    else if (strcmp(mes, "2") == 0)
    {
        strcpy(mes, "feb");
    }
    else if (strcmp(mes, "3") == 0)
    {
        strcpy(mes, "mar");
    }
    else if (strcmp(mes, "4") == 0)
    {
        strcpy(mes, "abr");
    }
    else if (strcmp(mes, "5") == 0)
    {
        strcpy(mes, "may");
    }
    else if (strcmp(mes, "6") == 0)
    {
        strcpy(mes, "jun");
    }
    else if (strcmp(mes, "7") == 0)
    {
        strcpy(mes, "jul");
    }
    else if (strcmp(mes, "8") == 0)
    {
        strcpy(mes, "ago");
    }
    else if (strcmp(mes, "9") == 0)
    {
        strcpy(mes, "sep");
    }
    else if (strcmp(mes, "10") == 0)
    {
        strcpy(mes, "oct");
    }
    else if (strcmp(mes, "11") == 0)
    {
        strcpy(mes, "nov");
    }
    else if (strcmp(mes, "12") == 0)
    {
        strcpy(mes, "dic");
    }

    return;
}

// VALIDAR FECHAS
int validarFecha(int dia, char mes[3], int ano)
{
    // Se valida que el mes y el dia esten correctos
    if (ano <= 0)
    {
        printf("Fecha incorrecta, intente nuevamente\n");
        return 1;
    }
    if ((strcmp(mes, "ene") == 0 || strcmp(mes, "mar") == 0 || strcmp(mes, "may") == 0 || strcmp(mes, "jul") == 0 || strcmp(mes, "ago") == 0 || strcmp(mes, "oct") == 0 || strcmp(mes, "dic") == 0 && (dia <= 31 && dia > 0)))
    {
        return 0;
    }
    if ((strcmp(mes, "abr") == 0 || strcmp(mes, "jun") == 0 || strcmp(mes, "sep") == 0 || strcmp(mes, "nov") == 0) && (dia <= 30 && dia > 0))
    {
        return 0;
    }
    // Años biciestos y febrero
    if ((strcmp(mes, "feb") == 0 && (dia <= 28 && dia > 0) && (ano % 4 != 0)) || (strcmp(mes, "feb") == 0 && (dia <= 29 && dia > 0) && (ano % 4 == 0)))
    {
        return 0;
    }
    printf("Fecha incorrecta, intente nuevamente\n");
    return 1;
}
int fechaFutura(int dia, char mes[3], int ano)
{
    // Valido que la fecha no sea mayor a la actual
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int anioActual = tm.tm_year + 1900;
    int mesActual = tm.tm_mon + 1;
    int diaActual = tm.tm_mday;

    // Paso mes a int para poder validar
    int mesN;
    if (strcmp(mes, "ene") == 0)
    {
        mesN = 1;
    }
    if (strcmp(mes, "feb") == 0)
    {
        mesN = 2;
    }
    if (strcmp(mes, "mar") == 0)
    {
        mesN = 3;
    }
    if (strcmp(mes, "abr") == 0)
    {
        mesN = 4;
    }
    if (strcmp(mes, "may") == 0)
    {
        mesN = 5;
    }
    if (strcmp(mes, "jun") == 0)
    {
        mesN = 6;
    }
    if (strcmp(mes, "jul") == 0)
    {
        mesN = 7;
    }
    if (strcmp(mes, "ago") == 0)
    {
        mesN = 8;
    }
    if (strcmp(mes, "sep") == 0)
    {
        mesN = 9;
    }
    if (strcmp(mes, "oct") == 0)
    {
        mesN = 10;
    }
    if (strcmp(mes, "nov") == 0)
    {
        mesN = 11;
    }
    if (strcmp(mes, "dic") == 0)
    {
        mesN = 12;
    }
    // Se compara la fecha ingresada con la actual
    if ((ano >= anioActual) && (mesN >= mesActual) && (dia > diaActual))
    {
        printf("Fecha a futuro, intente nuevamente\n");
        return 0;
    }
    return 1;
}

// Valida que el string no tenga numeros
int checkNumString(char *c)
{
    // Recorre un string y valida si tiene un numero adentro
    for (int i = 0; i < sizeof(c); i++)
    {
        if (c[i] >= '0' && c[i] <= '9')
        {
            return 1;
        }
    }
    return 0;
}
// Reemplazar caracter de cadena
void cambiarCaracter(char *cadena, char reemplazado, char aReemplazar)
{
    for (int i = 0; i < strlen(cadena) - 1; i++)
    {
        if (cadena[i] == reemplazado)
        {
            cadena[i] = aReemplazar;
        }
    }
    return;
}
// Genera el nombre del archivo Backup
void generarNombreBackup(char *nombre)
{
    // Datos de la fecha y el nombre base
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strcpy(nombre, "competidores_bajas_");
    int anio = tm.tm_year + 1900;
    int mes = tm.tm_mon + 1;
    int dia = tm.tm_mday;

    // Se convierte a string
    char aux[10];
    itoa(anio, aux, 10);
    strcat(aux, " "); // Se agrega el espacio
    strcat(nombre, aux);
    itoa(mes, aux, 10);
    strcat(aux, " ");
    strcat(nombre, aux);
    itoa(dia, aux, 10);
    strcat(nombre, aux);
    strcat(nombre, ".xyz"); // Se agrega la extensión del archivo
    return;
}
