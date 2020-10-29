#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mapeo.h"
#include "lista.h"



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
    printf ("hash: %i \n",hash);

}

void ingresarDatos(tMapeo map){

    printf("#Ingrese palabras al mapeo#\n");
    char *palabra;
    palabra=(char*)malloc(40*sizeof(char));


    tLista *l=(tLista*)malloc(sizeof(tLista)*5);
    crear_lista(&*l);

    for (int i = 0;i<5;i++){
        printf("Ingrese una palabra \n");
        scanf("%s",palabra);

        int *valor;
        valor=(int*)malloc(sizeof(int));
        *valor=(int)m_recuperar(map,palabra);
        struct entrada *nueva_entrada=(struct entrada*)malloc(sizeof(struct entrada));

        nueva_entrada->clave=(funcion_hash_evaluador(palabra))%5;
        nueva_entrada->valor=valor;

        l_insertar(*l,l_primera(*l),nueva_entrada);
        if (*valor==NULL){
            printf("La palabra no se encuentra en el texto. \n");
            m_insertar(map,palabra,valor);
        }
        else
        {
            if (valor>1)
                printf("La palabra %s se encuentra %d veces en el texto. \n",palabra,valor);
            else
                printf("La palabra %s se encuentra 1 vez en el texto. \n",palabra);
        }

        int h=0;
        while (h<40)  //borro palabra usada
        {
                 palabra[h]=0;

                        h++;
             }
    }

    tPosicion paRecorrer;
    paRecorrer=(tPosicion)l_primera(*l);
    printf("voy a eliminar \n");
    for (int j=0 ; j<5 ; j++){

        tEntrada elem = (tEntrada)l_recuperar(*l,paRecorrer);
        tClave c = (int*)elem->clave;
        tValor v = (int*)elem->valor;

  //      tValor v = (tValor) m_recuperar(map,elem->clave); //tengo que mostrar que tengo adentro del mapeo
//        printf("clave en mapeo: %s",v);
        printf("clave-valor en lista: %s | %i \n",c,v);
        paRecorrer = (tEntrada)l_siguiente(*l,paRecorrer);
    }
    printf("\nElimine todo ");
}


int main(int argc, char *argv[])    //--------- agregue un * mas
{
    printf("##### TESTEANDO A MANO #####\n");
    tMapeo mapeo;
    crear_mapeo(&mapeo,5,&funcion_hash_evaluador,&comparacion_claves_evaluador);
    ingresarDatos(mapeo);

    return 0;
}
