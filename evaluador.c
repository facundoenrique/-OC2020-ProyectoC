#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mapeo.h"
#include "lista.h"
#define letras (caracteres[i]>=97 && caracteres[i]<=122)

/**
    Funcion para comparar dos claves, las claves en este caso son caracteres.
    El comparador retorna 0 si las dos claves son IGUALES.
**/

int comparacion_claves_evaluador(char* palabra1, char* palabra2)
{

    int i = 0;
    while ((*((palabra1+i))!='\0') && (*((palabra2+i))!='\0') && (*(palabra1+i)==*(palabra2+i)))
          i=i+1;
    if (*(palabra1+i)!='\0' || *(palabra2+i)!='\0')
       return 1;
    else return 0;

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

/**
    Funcion que elimina la clave que le pasan como parametro,
    en este caso se trata de caracteres.
**/
void funcion_eliminar_claves_evaluador(char *palabra)
{
    //Esta bien hecho asi? Hay que eliminar todos los char que componen la palabra
    int i=0;
    while (*(palabra+i)!='\0')
    {
        free(*(palabra+i));
        i=i+1;
    }
}

/**
    Funcion que elimina el valor que le pasan como parametro,
    en este caso se trata de un entero.
**/
void funcion_eliminar_valores_evaluador(int *valor)
{
    free(valor);
}


int ingreso_nro_de_operacion()
{
    int nro_operacion;
    int nro_valido=0;
    printf("Ingrese el numero de operacion que desea realizar: ");
    while (!nro_valido)
    {

        if(scanf("%i",&nro_operacion) != 1 )
        {
            printf("No ingreso un numero, ingrese un numero de operacion: ");
            /*limpio buffer*/
            while(getchar() != '\n');
        }
        else
        {
            if(nro_operacion<1 || nro_operacion>2)
            {
                printf("No ingreso un numero valido, ingrese nuevamente un numero de operación: ");
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
    m_destruir(mapeo,&funcion_eliminar_claves_evaluador,&funcion_eliminar_valores_evaluador);
    free(mapeo);
    printf("Operacion realizada con exito\n");
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
    *valor=m_recuperar(map,palabra);
    if (*valor==NULL){
        printf("La palabra no se encuentra en el texto. \n");
    }
    else
    {
        if (valor>1)
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
    printf("¿Qué operación desea realizar ahora? \n");
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

    char* caracteres; /*para leer cada renglon*/

    caracteres=(char*)malloc(100*sizeof(char));
    //tMapeo mapeo;
    tMapeo mapeo;
    crear_mapeo(&mapeo,5,&funcion_hash_evaluador,&comparacion_claves_evaluador);


    char* palabra;
    palabra = (char*)malloc(40*sizeof(char));
    palabra[0]='\0';
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
                palabra[j]='\0'; //le asigno fin de palabra, aunque no haya llegado al final
            }
            else   //si es un separador, puedo haber terminado de armar la palabra
            {

                int l = strlen(palabra);


                if (l>0)   // si longitud de palabra es mayor a 0 arme una cadena
                {

                    printf("%s \n",palabra);

                    //int valor=(int)m_recuperar(mapeo,palabra);
                    int *valor;
                    valor=(int*)malloc(sizeof(int));
                    *valor=(int)m_recuperar(mapeo,palabra);
                    if (*valor==NULL)
                    {
                        m_insertar(mapeo,palabra,1);
                        printf("La palabra '%s' no estaba en el mapeo y su valor es: '%d' \n",palabra,*valor);
                    }
                    else
                    {
                        m_insertar(mapeo,palabra,(*valor)+1);
                       printf("Incremento el valor de la palabra '%s' a '%d' \n",palabra,*valor);
                    }

                    int h=0;
                    while (h<40)  //borro palabra usada
                    {
                        palabra[h]=0;
//----->>>>>>>>>>>>>>>>> o palabra[h]=NULL; ?
                        h++;
                    }
                    j=0;
                }// fin if
            }//fin else
        }
    } //fin while

    free(caracteres);
    free(palabra);
    fclose(archivo_texto); //cierro archivo, ya no lo uso


    if(&(mapeo)->cantidad_elementos > 0)
    {
        printf("Menú de operaciones:\n");
        printf("1- Cantidad de apariciones\n");
        printf("2- Salir: permite salir del programa\n");
        operaciones(mapeo);
    }
    else
        printf("El archivo no contiene palabras válidas.\n");
}



/*

void menu_operaciones(FILE* archivo_texto)
{
    char* caracteres; //para leer cada renglon

    caracteres=(char*)malloc(100*sizeof(char));
    tMapeo *mapeo;
    //crear_mapeo(&mapeo,5,&funcion_hash_evaluador,&comparacion_claves_evaluador);
    crear_mapeo(&mapeo,20,funcion_hash_evaluador,comparacion_claves_evaluador);
    printf("Se crea bien el mapeo \n");          //-----------------------------DESPUES BORRAR ESTO//
    char* palabra;
    palabra = (char*)malloc(40*sizeof(char));
    while (feof(archivo_texto)==0)
    {
        fgets(caracteres,80,archivo_texto);

        int i=0;
        int longitud=strlen(caracteres);
        int j=0;
        for (i=0; i<=longitud; i++) //leo todo el renglon en busca de caracteres no validos
        {
            if (!separadores)  //si no son separadores armo palabra
            {
                //estoy dentro de una palabra.
                palabra[j]=caracteres[i];
                j++;
            }
            else   //si es un separador, puedo haber terminado de armar la palabra
            {
                int l = strlen(palabra);
                if (l>0)   // si longitud de palabra es mayor a 0 arme una cadena
                {
                    palabra[l]='\0'; //le asigno fin de palabra.
                    //Va esto de aca abajo o creo el int *valor?
                    //(int)tValor valor = m_recuperar(mapeo,palabra);
                    //int valor=(int)m_recuperar(mapeo,palabra);
                    int *valor;
                    valor=(int)malloc(sizeof(int));
                    *valor=m_recuperar(mapeo,palabra);
                    if (*valor==NULL){
//----------------------valor = 0;
                        m_insertar(mapeo,palabra,1);
                        printf("La palabra: %s no estaba en el mapeo. \n",palabra);
                    }else {
//---------------------valor++;
                       m_insertar(mapeo,palabra,valor+1);
                       printf("Incremento el valor de la clave: %s. \n",palabra);
                    }
//----------------------if( m_insertar(mapeo,palabra,valor) != NULL)
                        printf("Incremento el valor de la clave palabra. \n",palabra);

                    else
                        printf("La palabra '%s' no estaba en el mapeo \n",palabra);
                        //

                    int h=0;
                    while (h<40)  //borro palabra usada
                    {
                        palabra[h]=0;
   //------------------ o palabra[h]=NULL; ?
                        h++;
                    }
                    j=0;
                }//fin if
            }//fin else
        }
    }
    free(caracteres);
    free(palabra);
    fclose(archivo_texto); //cierro archivo, ya no lo uso

    if((*mapeo)->cantidad_elementos > 0)
    {
        printf("Menú de operaciones:\n");
        printf("1- Cantidad de apariciones\n");
        printf("2- Salir: permite salir del programa\n");
        operaciones(mapeo);
    }
    else
        printf("El archivo no contiene palabras válidas.\n");
}

*/

/**
**/

int main(int argc, char *argv[])    //--------- agregue un * mas
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
        printf ("Error ante la invocación del programa\n");
        return -2;
    }
    return 0; /*?*/
}


