#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mapeo.h"
#include "lista.h"
#define letras (caracteres[i]>=97 && caracteres[i]<=122)


void mostrarPalabra(tMapeo map){

    printf("# Operación 1 : Buscar palabra en el archivo #\n");
    char* palabra = malloc(40);
    printf("Ingrese una palabra para chequear si está en el archivo: ");
    scanf("%s",palabra);

    int valor = (int) m_recuperar(map,palabra);
    if (valor==NULL){
        printf("La palabra no se encuentra en el texto \n");
    }else {
        if (valor>1){
            printf("%s",palabra);
            printf(" se encuentra ");
            printf("%i ",valor);printf(" veces en el texto \n");
        }else {

            printf("%c",palabra);
            printf(" palabra se encuentra 1 vez en el texto \n");
        }
    }

}


//Funcion para comparar dos claves, las claves en este caso son caracteres
//No se si esta bien hecho asi, es la unica forma que se me ocurrio
//El comparador retorna 0 si las dos claves son IGUALES
int comparacion_claves_evaluador(char* palabra1, char* palabra2)
{
    int son_iguales=1;
    int i=0;
    while (son_iguales==1 && '\0'!=*(palabra1+i))
    {
        if (*(palabra1+i)!=*(palabra2+i))
            son_iguales=0;
        else
            i=i+1;
    }
    if (son_iguales==0) /*NO SON IGUALES*/
        return 1;
    else
        return 0;
}

//Una ayudante paso una pagina con codigos de hash y copie este
int funcion_hash_evaluador(char *palabra)
{
    long hash = 5381;
    int c;

    while (c = *palabra++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

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

void funcion_eliminar_valores_evaluador(int *valor)
{
    free(valor);
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
            if(nro_operacion<1 || nro_operacion>2)
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

void salir(tMapeo mapeo)
{


    m_destruir(&mapeo,&funcion_eliminar_claves_evaluador,&funcion_eliminar_valores_evaluador);
    free(mapeo);
    printf("Operación realizada con éxito\n");
    exit(0);
}

void operaciones(tMapeo map)
{
    printf("¿Qué operación desea realizar ahora? \n");
    int nro_operacion = ingreso_nro_de_operacion();
    switch(nro_operacion)
    {
    case 1:
        mostrarPalabra(map);    //  ----------> lo puse como comentario porque falta implementarlo y sino no compilaba
        operaciones(map); //vuelvo a llamar a operaciones para que el usuario decida como continuar.
        break;
    case 2:
        salir(map);
        break;

    }
}


void menu_operaciones(FILE* archivo_texto)
{

    char* caracteres; /*para leer cada renglon*/

    caracteres=(char*)malloc(100*sizeof(char));
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

                    int valor=(int)m_recuperar(mapeo,palabra);
                    if (valor==NULL){

                      m_insertar(mapeo,palabra,1);
                      printf("Incremento el valor de la clave '%s'. \n",palabra);
                    }else {

                       m_insertar(mapeo,palabra,valor+1);
                       printf("La palabra '%s' no estaba en el mapeo \n",palabra);
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


    if(mapeo->cantidad_elementos > 0)
    {
        printf("Menú de operaciones:\n");
        printf("1- Cantidad de apariciones\n");
        printf("2- Salir: permite salir del programa\n");
        operaciones(mapeo);
    }
    else
        printf("El archivo no contiene palabras válidas.\n");
}

int main(int argc, char **argv[])
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
            // se puede agregar un control de tipo de archivo, para ver si es de texto (.txt)
            printf("Archivo abierto con exito \n");
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
