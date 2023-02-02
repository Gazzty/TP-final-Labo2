#include "libreria-general.c"

// FUNCIONES DE ARCHIVOS DE TEXTO
void crearTXT(FILE *archivo, char *nombre)
{
    archivo = fopen(nombre, "w");
    if (archivo != NULL)
    {
        printf("Archivo creado!\n");
        fclose(archivo);
    }
    else
    {
        printf("%s\n", error[0]);
    }
    return;
}
void cambiarTextoTXT(FILE *archivo, char *nombre)
{
    archivo = fopen(nombre, "w");
    char texto[1000];
    if (archivo != NULL)
    {
        printf("Ingrese el texto deseado:\n");
        gets(texto);
        fflush(stdin);
        fputs(texto, archivo);
        fclose(archivo);
    }
    else
    {
        printf("%s\n", error[0]);
    }
    return;
}
void agregarTextoTXT(FILE *archivo, char *nombre)
{
    archivo = fopen(nombre, "a");
    char texto[1000];
    if (archivo != NULL)
    {
        printf("Ingrese el texto deseado:\n");
        gets(texto);
        fflush(stdin);
        fputs(texto, archivo);
        fclose(archivo);
    }
    else
    {
        printf("%s\n", error[0]);
    }
    return;
}
void leerTXT(FILE *archivo, char *nombre)
{
    int index, nroComp, dia, anio, edad;
    char mes[4];
    char pais[4];
    float tiempo;
    char c[100];
    char *print;
    archivo = fopen(nombre, "r");

    if (archivo == NULL)
    {
        printf("%s", error[0]);
        return;
    }
    else
    {
        printf(" Orden\tCompetidor\tDia\tMes\tAnio\tEdad\tPais\tTiempo\t");

        while (fgets(c, 100, archivo))
        { // Esta func necesita una cadena, la cant de carac y el pA
            // index
            print = strtok(c, ";"); // strtok parte la cadena segun la delimitacion del 2do arg.
            index = atoi(print);    // atoi->a entero;atof->a float
            // nroComp
            print = strtok(NULL, ";");
            nroComp = atoi(print);
            // dia
            print = strtok(NULL, ";");
            dia = atoi(print);
            // mes
            print = strtok(NULL, ";");
            strcpy(mes, print); // strcpy pega "print" en "mes"
            // año
            print = strtok(NULL, ";");
            anio = atoi(print);
            // edad
            print = strtok(NULL, ";");
            edad = atoi(print);
            // pais
            print = strtok(NULL, ";");
            strcpy(pais, print);
            // tiempo
            print = strtok(NULL, ";");
            tiempo = atof(print);
            /* tiempo = tiempo + 0.000000; */
            printf("\n %d\t%d\t        %d\t%s\t%d\t%d\t%s\t%.6f", index, nroComp, dia, mes, anio, edad, pais, tiempo);
        }
        fclose(archivo);
    }
    printf("\n");
    return;
}
void borrarTXT(FILE *archivo, char *nombre)
{
    archivo = fopen(nombre, "w");
    if (archivo != NULL)
    {
        fputs("", archivo);
        fclose(archivo);
    }
    else
    {
        printf("%s\n", error[0]);
    }

    return;
}

// FUNCIOES DE ARCHIVOS BINARIOS
void crearB(FILE *archivo, char *nombre)
{
    archivo = fopen(nombre, "wb");
    if (archivo == NULL)
    {
        printf("%s\n", error[0]);
    }
    else
    {
        printf("\nArchivo creado correctamente!\n\n");
        fclose(archivo);
    }
    return;
}
void leerCompB(FILE *archivo, char *nombre, struct Competidores competidor, int opcLeer)
{
    // Primero se revisa que se pueda abrir el archivo
    archivo = fopen(nombre, "rb");
    if (archivo == NULL)
    {
        printf("%s\n", error[0]);
    }
    else
    {
        // Variables
        char pais[100];
        float tiempoMin, tiempoMax;
        // Se obtiene la cantidad de registros
        fseek(archivo, 0, SEEK_END);
        int nReg = ftell(archivo) / sizeof(struct Competidores);
        fseek(archivo, 0, SEEK_SET);
        // Se hace un switch para saber que opcion eligio el usuario
        switch (opcLeer)
        {
        case 1: // IMPRIME TODO
            for (int i = 0; i < nReg; i++)
            {
                fread(&competidor, sizeof(struct Competidores), 1, archivo);
                imprimirCompetidor(competidor);
            }
            break;
        case 2: // Se imprime los competidores activos
            for (int i = 0; i < nReg; i++)
            {
                fread(&competidor, sizeof(struct Competidores), 1, archivo);
                // Se revisa que este activo
                if (competidor.activo == 1)
                {
                    imprimirCompetidor(competidor);
                }
            }
            break;
        case 3:
            // Filtra por país
            printf("Ingrese el pais:\n");
            scanf("%3s", pais);
            fflush(stdin);
            while (checkNumString(pais) == 1) // Se valida que sea un string
            {
                printf("%s\n", error[1]);
                scanf("%3s", pais);
                fflush(stdin);
            }
            // Se pasa a mayuscula para poder comparar
            mayus(pais);
            for (int i = 0; i < nReg; i++)
            {
                fread(&competidor, sizeof(struct Competidores), 1, archivo);
                // Se compara el pais
                if (strcmp(competidor.pais, pais) == 0)
                {
                    imprimirCompetidor(competidor);
                }
            }
            break;
        case 4:
            // Rango de tiempo
            // Se piden los valores
            printf("Ingrese el valor minimo:\n");
            readFloat(&tiempoMin);
            printf("Ingrese el valor maximo:\n");
            readFloat(&tiempoMax);
            // Se recorre el archivo
            for (int i = 0; i < nReg; i++)
            {
                // Se leen las lineas con los competidores
                fread(&competidor, sizeof(struct Competidores), 1, archivo);
                // Se compara el tiempo para ver si estan en el rango de tiempo pedido
                if (competidor.tiempo >= tiempoMin && competidor.tiempo <= tiempoMax)
                {
                    imprimirCompetidor(competidor);
                }
            }
            break;
        default:
            printf("%s\n", error[1]);
            break;
        }
        fclose(archivo);
    }

    return;
}
void vaciarB(FILE *archivo, char *nombre)
{
    archivo = fopen(nombre, "wb");
    fwrite("", sizeof(""), 1, archivo);
    fclose(archivo);
}
int existeComp(FILE *archivo, char *nombre, int index)
{
    struct Competidores competidor;
    struct Fechas fecha;
    archivo = fopen(nombre, "rb");
    if (archivo == NULL)
    {
        printf("%s\n", error[0]);
    }
    else
    {
        fseek(archivo, 0, SEEK_END);
        int nReg = ftell(archivo) / sizeof(struct Competidores);
        fseek(archivo, 0, SEEK_SET);
        for (int i = 0; i < nReg; i++)
        {
            fread(&competidor, sizeof(struct Competidores), 1, archivo);
            if (index == competidor.index)
            {
                return 1;
            }
        }
    }
    return 0;
}
int existeID(FILE *archivo, char *nombre, int id)
{
    struct Competidores competidor;
    struct Fechas fecha;
    archivo = fopen(nombre, "rb");
    if (archivo == NULL)
    {
        printf("%s\n", error[0]);
    }
    else
    {
        fseek(archivo, 0, SEEK_END);
        int nReg = ftell(archivo) / sizeof(struct Competidores);
        fseek(archivo, 0, SEEK_SET);
        for (int i = 0; i < nReg; i++)
        {
            fread(&competidor, sizeof(struct Competidores), 1, archivo);
            if (id == competidor.id)
            {
                return 1;
            }
        }
    }
    return 0;
}

// Dar de alta competidor
void altaComp(FILE *archivo, char *nombre)
{
    // Variables
    char activo;
    struct Competidores competidor;
    archivo = fopen(nombre, "r+b");
    if (archivo == NULL)
    {
        printf("%s\n", error[0]);
    }
    else
    {
        // Se piden todos los datos del competidor
        // Posicion
        printf("Ingrese la posicion en la que va a ingresar al competidor: ");
        readInt(&competidor.index);
        // Se revisa que no exista el competidor, que la posicion sea mayor a 0
        while (((existeComp(archivo, NOMBRE_ARCHIVO_BIN, competidor.index) == 1) || (competidor.index <= 0)))
        {
            // Dependiendo el error se hace un output diferente
            if (competidor.index <= 0)
            {
                printf("ERROR, ingrese una posicion mayor a 0: ");
            }
            else if (existeComp(archivo, NOMBRE_ARCHIVO_BIN, competidor.index) == 1)
            {
                printf("ERROR, la posicion ya existe, ingresela nuevamente: ");
            }
            readInt(&competidor.index);
        }

        // ID(
        printf("Ingrese el ID del competidor: ");
        readInt(&competidor.id);
        // Se valida que el ID sea mayor a 0 y que sea un numero
        while (competidor.id <= 0 || existeID(archivo, NOMBRE_ARCHIVO_BIN, competidor.id) == 1)
        {
            if (competidor.id <= 0)
            {
                printf("Error, ingrese un ID mayor a 0: ");
            }
            if (existeID(archivo, NOMBRE_ARCHIVO_BIN, competidor.id) == 1)
            {
                printf("El ID ya existe, ingrese otro: ");
            }
            readInt(&competidor.id);
        }
        // Fechas
        printf("Ingrese el dia: ");
        readInt(&competidor.fecha.dia);
        printf("Ingrese el mes: ");
        scanf("%3s", competidor.fecha.mes);
        fflush(stdin);
        // Se cambia el input del usuario a char en caso de que ingrese un int
        mesToChar(competidor.fecha.mes);
        min(competidor.fecha.mes);
        printf("Ingrese el anio: ");
        readInt(&competidor.fecha.ano);
        int fDate = fechaFutura(competidor.fecha.dia, competidor.fecha.mes, competidor.fecha.ano);
        while ((validarFecha(competidor.fecha.dia, competidor.fecha.mes, competidor.fecha.ano) == 1) || (fDate == 0))
        {
            printf("Ingrese el dia: ");
            readInt(&competidor.fecha.dia);
            printf("Ingrese el mes: ");
            scanf("%3s", competidor.fecha.mes);
            fflush(stdin);
            // Se cambia el input del usuario a char en caso de que ingrese un int
            mesToChar(competidor.fecha.mes);
            min(competidor.fecha.mes);
            printf("Ingrese el anio: ");
            readInt(&competidor.fecha.ano);
            fDate = fechaFutura(competidor.fecha.dia, competidor.fecha.mes, competidor.fecha.ano);
        }

        // Edad
        printf("Ingrese la edad: ");
        readInt(&competidor.edad);
        while (competidor.edad <= 0)
        {
            printf("%s", error[1]);
            readInt(&competidor.edad);
        }
        // Pais
        printf("Ingrese el pais: ");
        scanf("%3s", competidor.pais);
        fflush(stdin);
        while (checkNumString(competidor.pais) == 1)
        {
            printf("Ingrese un texto valido: ");
            scanf("%3s", competidor.pais);
            fflush(stdin);
            mayus(competidor.pais);
        }
        mayus(competidor.pais); // Se pasa a mayúscula
        // Tiempo
        printf("Ingrese el tiempo: ");
        readFloat(&competidor.tiempo);
        while (competidor.tiempo <= 0)
        {
            printf("%s", error[1]);
            readFloat(&competidor.tiempo);
        }

        // Se pone el competidor en activo
        competidor.activo = 1;

        // Se ingresan esos datos al archivo
        // Se pone el puntero al principio del archivo
        fseek(archivo, 0, SEEK_SET);
        // Se pone el puntero en la posición donde se va a guardar al competidor
        fseek(archivo, (competidor.index - 1) * sizeof(struct Competidores), SEEK_SET);
        fwrite(&competidor, sizeof(struct Competidores), 1, archivo);
        fclose(archivo);
    }
    return;
}
// Filtra los competidores por orden o ID y dependiendo de la opción deja leer o editar
// Filtrar competidor y hace una accion dependiendo la opción que haya elegido el usuario
void buscarComp(FILE *archivo, char *nombre, char opcRW)
{
    int opc, nroOrden = 0, nroComp = 0, nReg = 0, isNum;
    int existe = 1;
    char c;
    struct Competidores competidor;
    archivo = fopen(nombre, "r+b");
    if (archivo == NULL)
    {
        printf("%s\n", error[0]);
    }
    else
    {
        // Se pide una opcion al usuario
        printf("------------------------\n");
        printf("1: Numero de orden\n");
        printf("2: Numero de competidor\n");
        readInt(&opc);
        // Se compara esa opcion y se hace una busqueda acorde
        switch (opc)
        {
        case 1:
            printf("Ingrese el numero de orden: ");
            readInt(&nroOrden);
            // Se valida que sea un numero mayor a 0
            while (nroOrden <= 0)
            {
                printf("Ingrese un numero mayor a 0: ");
                readInt(&nroOrden);
            }
            fseek(archivo, 0, SEEK_END);
            nReg = ftell(archivo) / sizeof(struct Competidores);
            fseek(archivo, 0, SEEK_SET);
            // Se recorre el archivo
            for (int i = 0; i < nReg; i++)
            {
                // Se lee la linea del archivo y se guardan los datos en competidor
                fread(&competidor, sizeof(struct Competidores), 1, archivo);
                // Se compara el nro de orden
                if (nroOrden == competidor.index)
                {
                    // Diferentes opciones para leer o editar
                    switch (opcRW)
                    {
                    case 'r': // LEER
                        imprimirCompetidor(competidor);
                        break;
                    case 'w': // EDITAR
                        editarComp(&competidor);
                        if (competidor.index == 1)
                        {
                            fseek(archivo, 0, SEEK_SET);
                            fwrite(&competidor, sizeof(struct Competidores), 1, archivo);
                        }
                        else
                        {
                            fseek(archivo, 0, SEEK_SET);
                            fseek(archivo, (competidor.index - 1) * sizeof(struct Competidores), SEEK_CUR);
                            fwrite(&competidor, sizeof(struct Competidores), 1, archivo);
                        }
                        fclose(archivo);
                        return;
                        break;
                    case 'd':
                        printf("El competidor va a pasar a estado inactivo\n");
                        printf("DESEA CONTINUAR? (S/N)\n");
                        scanf("%c", &c);
                        fflush(stdin);
                        mayus(&c);
                        if (c == 'S')
                        {
                            competidor.activo = 0;
                            if (competidor.index == 1)
                            {
                                fseek(archivo, 0, SEEK_SET);
                                fwrite(&competidor, sizeof(struct Competidores), 1, archivo);
                            }
                            else
                            {
                                fseek(archivo, 0, SEEK_SET);
                                fseek(archivo, (competidor.index - 1) * sizeof(struct Competidores), SEEK_CUR);
                                fwrite(&competidor, sizeof(struct Competidores), 1, archivo);
                            }
                            system("cls");
                            printf("El competidor ahora esta inactivo\n\n");
                        }
                        else
                        {
                            system("cls");
                            printf("No se realizó ninguna modificación\n\n");
                        }
                        fclose(archivo);
                        return;
                        break;
                    default:
                        break;
                    }
                    existe = 0;
                }
            }
            if (existe == 1)
            {
                system("cls");
                printf("El competidor ingresado no existe\n\n");
            }
            break;
        case 2:
            printf("Ingrese el numero de competidor: ");
            readInt(&nroComp);
            // Se valida que sea un numero mayor a 0 y un numero
            while (nroComp <= 0)
            {

                printf("Ingrese un numero mayor a 0: ");
                readInt(&nroComp);
            }
            fseek(archivo, 0, SEEK_END);
            nReg = ftell(archivo) / sizeof(struct Competidores);
            fseek(archivo, 0, SEEK_SET);
            // Se recorre el archivo
            for (int i = 0; i < nReg; i++)
            {
                // Se lee la linea del archivo y se guardan los datos en competidor
                fread(&competidor, sizeof(struct Competidores), 1, archivo);
                // Se compara el ID
                if (nroComp == competidor.id)
                {
                    // Diferentes opciones para leer o editar
                    switch (opcRW)
                    {
                    case 'r':
                        imprimirCompetidor(competidor);
                        break;
                    case 'w':
                        editarComp(&competidor);
                        if (competidor.index == 1)
                        {
                            fseek(archivo, 0, SEEK_SET);
                            fwrite(&competidor, sizeof(struct Competidores), 1, archivo);
                        }
                        else
                        {
                            fseek(archivo, 0, SEEK_SET);
                            fseek(archivo, (competidor.index - 1) * sizeof(struct Competidores), SEEK_CUR);
                            fwrite(&competidor, sizeof(struct Competidores), 1, archivo);
                        }
                        fclose(archivo);
                        return;
                        break;
                    case 'd':
                        printf("El competidor va a pasar a estado inactivo\n");
                        printf("DESEA CONTINUAR? (S/N)\n");
                        scanf("%c", &c);
                        fflush(stdin);
                        mayus(&c);
                        if (c == 'S')
                        {
                            competidor.activo = 0;
                            if (competidor.index == 1)
                            {
                                fseek(archivo, 0, SEEK_SET);
                                fwrite(&competidor, sizeof(struct Competidores), 1, archivo);
                            }
                            else
                            {
                                fseek(archivo, 0, SEEK_SET);
                                fseek(archivo, (competidor.index - 1) * sizeof(struct Competidores), SEEK_CUR);
                                fwrite(&competidor, sizeof(struct Competidores), 1, archivo);
                            }
                            system("cls");
                            printf("El competidor ahora está inactivo\n");
                        }
                        else
                        {
                            system("cls");
                            printf("No se realizó ninguna modificación\n\n");
                        }
                        fclose(archivo);
                        return;
                        break;
                    default:
                        break;
                    }
                    existe = 0;
                }
            }
            if (existe == 1)
            {
                system("cls");
                printf("El competidor ingresado no existe\n\n");
            }
            break;
        default:
            system("cls");
            printf("%s\n", error[1]);
            break;
        }

        fclose(archivo);
    }

    return;
}

// Baja fisica
void eliminarInactivos(FILE *archivo, char *nombre, char *nombreBackup)
{
    printf("Nombre del archivo: %s\n", nombreBackup);

    int existe = 1;
    int temp; // variable auxiliar para guardar el indice del competidor

    struct Competidores competidor;

    archivo = fopen(nombre, "r+b");
    if (archivo == NULL)
    {
        printf("%s\n", error[0]);
    }
    else
    {
        // Primero obtengo la cantidad de registros
        fseek(archivo, 0, SEEK_END);
        int nReg = ftell(archivo) / sizeof(struct Competidores);
        fseek(archivo, 0, SEEK_SET);
        // Se recorre el archivo
        for (int i = 0; i < nReg; i++)
        {
            // Se revisa que existan competidores inactivos
            fread(&competidor, sizeof(struct Competidores), 1, archivo);
            if (competidor.activo == 0 && competidor.index > 0)
            {
                existe = 0;
            }
        }
        if (existe == 1)
        {
            system("cls");
            printf("No se encontraron competidores inactivos\n");
            return;
        }
        else
        {
            FILE *archivoBackup;
            archivoBackup = fopen(nombreBackup, "ab");
            if (archivoBackup == NULL)
            {
                printf("%s\n", error[0]);
            }
            else
            {
                // Se lee al competidor y si esta inactivo se guarda en el backup, y luego se reemplazan los datos por 0
                for (int i = 0; i < nReg; i++)
                {
                    fseek(archivo, 0, SEEK_SET);
                    fseek(archivo, i * sizeof(struct Competidores), SEEK_CUR);
                    fread(&competidor, sizeof(struct Competidores), 1, archivo);
                    if ((competidor.activo == 0) && (competidor.index > 0))
                    {
                        if (competidor.index == 1)
                        {
                            // BACKUP POINER
                            fseek(archivoBackup, 0, SEEK_SET);
                            fwrite(&competidor, sizeof(struct Competidores), 1, archivoBackup);

                            // Se borra el competidor del archivo original
                            competidor.index = 0;
                            competidor.id = 0;
                            competidor.fecha.dia = 0;
                            strcpy(competidor.fecha.mes, "0  ");
                            competidor.fecha.ano = 0;
                            competidor.edad = 0;
                            strcpy(competidor.pais, "0  ");
                            competidor.tiempo = 0;
                            fseek(archivo, 0, SEEK_SET);
                            fwrite(&competidor, sizeof(struct Competidores), 1, archivo);
                        }
                        else
                        {
                            // BACKUP POINTER
                            fseek(archivoBackup, 0, SEEK_SET);
                            fseek(archivoBackup, (competidor.index - 1) * sizeof(struct Competidores), SEEK_CUR);
                            fwrite(&competidor, sizeof(struct Competidores), 1, archivoBackup);

                            // Se borra el competidor del archivo original
                            temp = competidor.index;
                            competidor.index = 0;
                            competidor.id = 0;
                            competidor.fecha.dia = 0;
                            strcpy(competidor.fecha.mes, "0  ");
                            competidor.fecha.ano = 0;
                            competidor.edad = 0;
                            strcpy(competidor.pais, "0  ");
                            competidor.tiempo = 0;
                            fseek(archivo, 0, SEEK_SET);
                            fseek(archivo, (temp - 1) * sizeof(struct Competidores), SEEK_CUR);
                            fwrite(&competidor, sizeof(struct Competidores), 1, archivo);
                        }
                    }
                }
                system("cls");
                printf("Se completo el archivo %s con los datos de los competidores inactivos\n", nombreBackup);
                printf("Y se eliminaron los mismos del archivo %s\n", nombre);
            }
            fclose(archivoBackup);
        }
        fclose(archivo);
    }

    return;
}

void leerBackup(FILE *archivo, char *nombre)
{
    // Se abre el archivo, y se obtiene la cantidad de registros
    archivo = fopen(nombre, "rb");
    if (archivo == NULL)
    {
        printf("%s\n", error[0]);
    }
    else
    {
        printf("Nombre del backup: %s\n", nombre);
        fseek(archivo, 0, SEEK_END);
        int nReg = ftell(archivo) / sizeof(struct Competidores);
        fseek(archivo, 0, SEEK_SET);

        printf("Cantidad: %d\n", nReg);

        struct Competidores competidor;

        // Se recorre el archivo y se imprimen los competidores
        for (int i = 0; i < nReg; i++)
        {
            // Se lee al competidor
            fread(&competidor, sizeof(struct Competidores), 1, archivo);
            // Se imprimen los resultados
            imprimirCompetidor(competidor);
        }
        fclose(archivo);
    }
    return;
}