#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mapeo.h"
#include "lista.h"
#define letras (caracteres[i]>=97 && caracteres[i]<=122)

/**
    Funciones globales para usarlas en m_eliminar y m_destruir
**/

void (*funcion_eliminar_valores)(void*);
void (*funcion_eliminar_claves)(void*);


/**
    Funcion eliminar entrada que utiliza las funciones globales funcion_eliminar_claves y funcion_eliminar_valores,
    y libera el espacio de la entrada
**/

void funcion_eliminar_entradas(tElemento e)
{
    tEntrada entrada=(tEntrada)e;
    funcion_eliminar_claves(entrada->clave);
    funcion_eliminar_valores(entrada->valor);
    free(entrada);
    printf("Elimino la entrada");
}



/**
    Funcion que elimina la clave que le pasan como parametro,
    en este caso se trata de caracteres.
**/
void funcion_eliminar_claves_evaluador(char *palabra)
{
    free(palabra);
}

/**
    Funcion que elimina el valor que le pasan como parametro,
    en este caso se trata de un entero.
**/
void funcion_eliminar_valores_evaluador(int *valor)
{
    free(valor);
}


/**
    Funcion para comparar dos claves, las claves en este caso son caracteres.
    El comparador retorna 0 si las dos claves son IGUALES.
**/

int comparacion_claves_evaluador(char* palabra1, char* palabra2)
{

    int i = 0;
    if (strlen(palabra1)==strlen(palabra2)){
        while ((*((palabra1+i))!='\0') && (*((palabra2+i))!='\0') && (*(palabra1+i)==*(palabra2+i)))
              i=i+1;
        if (*(palabra1+i)!='\0' && *(palabra2+i)!='\0')
            return 1;
        else return 0;
    }else return 1;



}

//Una ayudante paso una pagina con codigos de hash y copie este
/**
    Funcion hash que convierte los caracteres en un entero.
**/

int funcion_hash_evaluador(char *palabra)
{
    long hash = 5381;
    int c;

    while (c = *palabra++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}



int ingreso_nro_de_operacion()
{
    int nro_operacion;
    int nro_valido=0;
    printf("Ingrese el n%cmero de operacion que desea realizar: ",163);
    while (!nro_valido)
    {

        if(scanf("%i",&nro_operacion) != 1 )
        {
            printf("No ingreso un n%cmero, ingrese un n%cmero de operaci%cn: ",163,163,162);
            /*limpio buffer*/
            while(getchar() != '\n');
        }
        else
        {
            if(nro_operacion<1 || nro_operacion>2)
            {
                printf("No ingreso un n%cmero valido, ingrese nuevamente un n%cmero de operaci%cn: ",163,163,162);
            }
            else
            {
                nro_valido=1;
            }
        }
    }
    return nro_operacion;
}

/**
    Funcion que permite salir del programa, liberando toda la memoria ocupada.
**/

void salir(tMapeo mapeo)
{
    m_destruir(&mapeo,(void*)&funcion_eliminar_claves_evaluador,(void*)&funcion_eliminar_valores_evaluador);
    free(mapeo);
    printf("Operaci%cn realizada con exito\n",162);
    exit(0);
}

/**
    Funcion que permite al usuario ingresar la palabra que quiere buscar en el archivo y muestra si esta
    presente o no, y en caso de estarlo la cantidad de veces que aparece.
**/

void mostrar_palabra(tMapeo map){

    printf("# Operacion 1 : Buscar palabra en el archivo #\n");
    char *palabra;
    palabra=(char*)malloc(40*sizeof(char));
    printf("Ingrese una palabra para chequear si esta en el archivo: \n");
    scanf("%s",palabra);
    printf("La palabra es: %s \n",palabra);

    int *valor;
    valor=(int*)malloc(sizeof(int));
    *valor=(int)m_recuperar(map,(tClave)palabra);
    if ((int*)(*valor)==NULL){
        printf("La palabra no se encuentra en el texto. \n");
    }
    else
    {
        if ((int)valor>1)
            printf("La palabra %s se encuentra %d veces en el texto. \n",palabra,*valor);
        else
            printf("La palabra %s se encuentra 1 vez en el texto. \n",palabra);
    }
}

/**
    Funcion que permite elegir al usuario la operacion que desea realizar.
**/

void operaciones(tMapeo map)
{
    printf("¿Qu%c operaci%cn desea realizar ahora? \n",163,162);
    int nro_operacion = ingreso_nro_de_operacion();
    switch(nro_operacion)
    {
    case 1:
        //      ----------> lo puse como comentario porque falta implementarlo y sino no compilaba
        mostrar_palabra(map);
        operaciones(map);
        break;
    case 2:
        salir(map);
        break;

    }
}

/**
    Muestra al usuario las operaciones que puede realizar.
**/

//EL NUEVO

void menu_operaciones(FILE* archivo_texto)
{


    //tMapeo mapeo;
    tMapeo mapeo;
    crear_mapeo(&mapeo,10,(void*)&funcion_hash_evaluador,(void*)&comparacion_claves_evaluador);

    char* caracteres; /*para leer cada renglon*/
    caracteres=(char*)malloc(100*sizeof(char));

 char* palabra;
        palabra = (char*)malloc(20*sizeof(char));



    while (feof(archivo_texto)==0)
    {


        fgets(caracteres,100,archivo_texto);

        int i=0;
        int longitud=strlen(caracteres); //longitud del renglon
        int j=0;

        for (i=0; i<=longitud; i++) //leo todo el renglon en busca de caracteres no validos
        {
            if (letras)  //si es una letra armo una palabra
            {
                //estoy dentro de una palabra.
                palabra[j]=caracteres[i];
                j++;
                //le asigno fin de palabra, aunque no haya llegado al final
            }
            else   //si es un separador, puedo haber terminado de armar la palabra
            {
                 palabra[j]='\0';
                int l = strlen(palabra);
                if (l>0)   // si longitud de palabra es mayor a 0 arme una cadena
                {




                    int *valor;
                    valor=(int*)malloc(sizeof(int));

                    *valor=(int)m_recuperar(mapeo,palabra);
                    if ((int*)*valor==NULL)
                    {
                        *valor=0;
                    }


                    m_insertar(mapeo,palabra,((*valor)+1));

                    palabra=NULL;
                    palabra = (char*)malloc(20*sizeof(char));


                    j=0; //reinicio el indice para armar la palabra siguiente.
                }// fin if
            }//fin else

        }//fin for

    } //fin while

 //   free(caracteres);

    fclose(archivo_texto); //cierro archivo, ya no lo uso


    if(&(mapeo)->cantidad_elementos > 0)
    {
        printf("Men%c de operaciones:\n",163);
        printf("1- Cantidad de apariciones\n");
        printf("2- Salir: permite salir del programa\n");
        operaciones(mapeo);
    }
    else
        printf("El archivo no contiene palabras v%clidas.\n",160);
}





int main(int argc, char *argv[])    //--------- agregue un * mas
{
    printf("##### EVALUADOR DE ARCHIVO DE TEXTO #####\n");
    if(argc==2)
    {
        char* nombre_archivo_texto = argv[1];
        FILE* archivo_texto;

        if((archivo_texto= fopen(nombre_archivo_texto,"r"))==NULL)
        {
            /*Abro el archivo en modo lectura*/
            printf ("Error al intentar la apertura del archivo.\n");
            return -1;
        }
        else
        {
            menu_operaciones(archivo_texto);
        }
    }
    else
    {
        printf ("Error ante la invocaci%cn del programa\n",162);
        return -2;
    }
    return 0; /*?*/
}


