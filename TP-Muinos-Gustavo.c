//--------------------------------------------------------//
//      ALUMNO: GUSTAVO MUIÑOS                            //
//      CARRERA: TECNICATURA EN REDES INFORMÁTCAS         //
//      FECHA DE ENTREGA: 10/11/2022                      //
// ARCHIVOS: TP_Muinos.c, libreria.c, corredores_v1.csv   //
//--------------------------------------------------------//

//-----INCLUDES-----//
#include "libreria-Archivos.c"

//----FUNCTIONS-----//
void migrarDatos(FILE *archivoT, FILE *archivoB, struct Competidores competidor, struct Fechas fecha);

int main()
{
    // Se agrega esto en caso de querer generar numeros aleatorios
    srand(time(NULL));
    // Variables
    struct Competidores competidor;
    struct Fechas fecha;
    FILE *archivoT, *archivoB;
    int opc, opcLeer;
    char c = 'N';
    char nombreBackup[100];
    generarNombreBackup(nombreBackup);

    //-----MENU DE OPCIONES-----//
    do
    {
        printf("Ingrese una opcion para continuar:\n");
        printf("0: Salir\n");
        printf("1: Imprimir archivo txt inicial\n");
        printf("2: Crear un archivo binario 'competidores.dat'\n");
        printf("3: Migrar datos\n");
        printf("4: Leer los datos del archivo binario\n");
        printf("5: Dar de alta un competidor\n");
        printf("6: Buscar un competidor\n");
        printf("7: Modificar un competidor\n");
        printf("8: Dejar inactivo a un competidor\n");
        printf("9: Eliminar a un competidor\n");
        printf("10: Listar los competidores dados de baja\n");
        scanf("%d", &opc);
        fflush(stdin);
        system("cls");
        switch (opc)
        {
        case 0:
            printf("Gracias por usar el servicio!\n");
            break;
        case 1:
            printf("%s\n", NOMBRE_ARCHIVO_TXT);
            leerTXT(archivoT, NOMBRE_ARCHIVO_TXT);
            break;
        case 2:
            crearB(archivoB, NOMBRE_ARCHIVO_BIN);
            break;
        case 3:
            printf("--PRECAUCION--\n");
            printf("Este proceso sobreescribira los datos en el archivo %s\n", NOMBRE_ARCHIVO_BIN);
            printf("Desea continuar? S/N\n");
            scanf("%c", &c);
            fflush(stdin);
            mayus(&c);
            if (c == 'S')
            {
                migrarDatos(archivoT, archivoB, competidor, fecha);
            }
            break;
        case 4:
            printf("---------------------------\n");
            printf("Que desea leer del archivo?\n");
            printf("1: Todo\n");
            printf("2: Activos\n");
            printf("3: Buscar por pais\n");
            printf("4: Buscar por rango de tiempo\n");
            scanf("%d", &opcLeer);
            fflush(stdin);
            printf("-----------------------------\n");
            leerCompB(archivoB, NOMBRE_ARCHIVO_BIN, competidor, opcLeer);
            break;
        case 5:
            altaComp(archivoB, NOMBRE_ARCHIVO_BIN);
            break;
        case 6:
            buscarComp(archivoB, NOMBRE_ARCHIVO_BIN, 'r');
            break;
        case 7:
            buscarComp(archivoB, NOMBRE_ARCHIVO_BIN, 'w');
            break;
        case 8:
            buscarComp(archivoB, NOMBRE_ARCHIVO_BIN, 'd');
            break;
        case 9:
            eliminarInactivos(archivoB, NOMBRE_ARCHIVO_BIN, nombreBackup);
            break;
        case 10:
            leerBackup(archivoB, nombreBackup);
            break;
        default:
            printf("%s\n", error[1]);
            break;
        }
    } while (opc != 0);

    return 0;
}

//-----FUNCTIONS LOGIC-----//
void migrarDatos(FILE *archivoT, FILE *archivoB, struct Competidores competidor, struct Fechas fecha)
{
    // Se abren los 2 archivos
    archivoT = fopen(NOMBRE_ARCHIVO_TXT, "r");
    archivoB = fopen(NOMBRE_ARCHIVO_BIN, "wb");
    // Se inicia un bool para llevar control del primer registro y un char que se completa con cada linea del archivo
    int pRegistro = 1;
    char c[sizeof(struct Competidores)];
    if (archivoT == NULL && archivoB == NULL)
    {
        printf("%s\n", error[0]);
    }
    else
    { // Se recorre linea a linea el archivo de texto
        int id = 0;
        fseek(archivoT, 0, SEEK_SET);
        while (fgets(c, sizeof(struct Competidores), archivoT) != NULL)
        {
            // Pongo el puntero al principio del archivo para que guarde todos los registros
            if (pRegistro == 1)
            {
                fseek(archivoT, 0, SEEK_SET);
                pRegistro = 0;
                id = 1; // Esto lo hago porque en el primer registro no tengo el competidor.id como para comparar y ya se que es el primer dato
            }
            else
            {
                // Esta variable la uso para la comparación antes y despues del fscanf
                id = competidor.id;
            }
            fscanf(archivoT, "%d;%d;%d;%3s;%d;%d;%3s;%f", &competidor.index, &competidor.id, &competidor.fecha.dia, competidor.fecha.mes, &competidor.fecha.ano, &competidor.edad, competidor.pais, &competidor.tiempo);
            // Reviso que no se repitan datos
            if (id != competidor.id)
            {
                // Agrego el campo activo
                competidor.activo = 1;
                fwrite(&competidor, sizeof(struct Competidores), 1, archivoB);
            }
        }
        fclose(archivoB);
        fclose(archivoT);
    }

    system("cls");
    printf("Migracion completa!\n\n");

    return;
}
