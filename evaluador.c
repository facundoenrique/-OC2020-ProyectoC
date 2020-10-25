#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mapeo.h>
#define separadores (caracteres[i]==44||caracteres[i]==46||caracteres[i]==58||caracteres[i]==59||caracteres[i]==73)



/**
* Funcion que compara 2 claves enteras
*/

int comparacion(tClave * clave1, tClave * clave2)
{

        int *a, *b;

        a = (int *) clave1;
        b = (int *) clave2;

        if (a==b)
            return 1
        else return 0;

}


int ingreso_nro_de_operacion()
{
    int nro_operacion;
    int nro_valido=0;
    printf("Ingrese el número de operación que desea realizar: ");
    while (!nro_valido)
    {

        if(scanf("%i",&nro_operacion) != 1 )
        {
            printf("No ingresó un número, ingrese un número de operación: ");
            /*limpio buffer*/
            while(getchar() != '\n');
        }
        else
        {
            if(nro_operacion<1 || nro_operacion>6)
            {
                printf("No ingresó un número válido, ingrese nuevamente un número de operación: ");
            }
            else
            {
                nro_valido=1;
            }
        }
    }
    return nro_operacion;
}

void salir(TMapeo mapeo)
{

    recorroYborro(trie,trie->raiz);
    m_destruir(mapeo,eliminarClave(),eliminarValor());
    free(mapeo);
    printf("Operación realizada con éxito\n");
    exit(0);
}

void operaciones(TTrie trie)
{
    printf("¿Qué operación desea realizar ahora? ");
    int nro_operacion = ingreso_nro_de_operacion();
    switch(nro_operacion)
    {
    case 1:
        mostrarPalabras(trie);
        operaciones(trie);
        break;
    case 2:
        salir(mapeo);
        break;

}


void menu_operaciones(FILE* archivo_texto)
{
    char* caracteres; /*para leer cada renglon*/

    caracteres=(char*)malloc(100*sizeof(char));

    TTrie trie =  crear_trie();
    char* palabra;
    palabra = (char*)malloc(40*sizeof(char));
    while (feof(archivo_texto)==0)
    {
        fgets(caracteres,80,archivo_texto);

        int i=0;
        int longitud=strlen(caracteres);
        int j=0;
        for (i=0; i<=longitud; i++) /*leo todo el renglon en busca de caracteres no validos*/
        {
            if (!separadores)  /*si no son separadores armo palabra*/
            {
                /*estoy dentro de una palabra.*/
                palabra[j]=caracteres[i];
                j++;
            }
            else   /*si es un separador, puedo haber terminado de armar la palabra*/
            {
                int l = strlen(palabra);
                if (l>0)   /* si longitud de palabra es mayor a 0 arme una cadena*/
                {
                    palabra[l]='\0'; //le asigno fin de palabra.
                    tValor valor = m_recuperar(mapeo,palabra);
                    if (valor==NULL){
                        valor = 0;
                    }
                    if( m_insertar(mapeo,palabra,valor) != NULL)
                        printf("Insertó la palabra '%s' o las letras restantes\n",palabra);

                    else /*=0*/
                        printf("La palabra '%s' ya estaba en el trie o está en el comienzo de otra palabra ya insertada, incrementé la cantidad en 1\n",palabra);
                    int h=0;
                    while (h<40)  /*borro palabra usada*/
                    {
                        palabra[h]=0;
                        h++;
                    }
                    j=0;
                }/*fin if*/
            }/*fin else*/
        }
    }
    free(caracteres);
    free(palabra);
    fclose(archivo_texto); //cierro archivo, ya no lo uso

    if(trie->cantidad_elementos > 0)
    {
        printf("Menú de operaciones:\n");
        printf("1- Cantidad de apariciones\n");
        printf("2- Salir: permite salir del programa\n");
        operaciones(trie);
    }
    else
        printf("El archivo no contiene palabras válidas.\n");
}

int main(int argc, char *argv[])
{
    printf("##### EVALUADOR DE ARCHIVO DE TEXTO#####\n");
    if(argc==2)
    {
        char* nombre_archivo_texto = argv[1];
        FILE* archivo_texto;

        if((archivo_texto= fopen(nombre_archivo_texto,"r"))==NULL)
        {
            /*Abro el archivo en modo lectura*/
            printf ("Error al intentar la apertura del archivo parametrizado.\n");
            return -1;
        }
        else
        {
            menu_operaciones(archivo_texto);
        }
    }
    else
    {
        printf ("Error ante la invicación del programa\n");
        return -2;
    }
    return 0; /*?*/
}
