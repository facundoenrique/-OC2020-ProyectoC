#include <stdlib.h>
#include <stdio.h>
#include "mapeo.h"
#include "lista.h"

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

int comparacion_claves_evaluador(char* palabra1, char* palabra2)
{

    int i = 0;
    if (strlen(palabra1)==strlen(palabra2)){
        while ((*((palabra1+i))!='\0') && (*((palabra2+i))!='\0') && (*(palabra1+i)==*(palabra2+i)))
              i=i+1;
        if (*(palabra1+i)!='\0' && *(palabra2+i)!='\0')
        {
            ;return 1;
        }

        else {
                    ;return 0;
        }
    }else {

        return 1;
    }


}



int main()
{
	tMapeo map;
    crear_mapeo(&map,5,&funcion_hash_evaluador,&comparacion_claves_evaluador);
    char *palabra;
    palabra=(char*)malloc(sizeof(char));

    int *valor;
    int *valor_recuperado;

    valor=(int*)malloc(sizeof(int));
    valor_recuperado=(int*)malloc(sizeof(int));

    printf("Palabra 1: \n");
    scanf("%s",palabra);
        int hash = funcion_hash_evaluador(palabra)%(map->longitud_tabla);

    *valor_recuperado=m_recuperar(map,(tClave)palabra);
    printf("valor_recuperado antes %d | %i\n",*valor_recuperado,hash);
    *valor=m_insertar(map,(tClave)palabra,(*valor_recuperado+1));
    *valor_recuperado=m_recuperar(map,(tClave)palabra);
    if (valor==NULL)
        printf("El valor era nulo\n");
    else
        printf("el valor era: %d | el valor_recuperado es: %d\n \n",*valor,*valor_recuperado);

    printf("palabra: %s\n",palabra);



     palabra=NULL;
    palabra=(char*)malloc(sizeof(char));
    printf("Palabra 2: \n");
    scanf("%s",palabra);
    hash = funcion_hash_evaluador(palabra)%(map->longitud_tabla);
    *valor_recuperado=m_recuperar(map,(tClave)palabra);
    printf("valor_recuperado antes %d | %i \n",*valor_recuperado,hash);
    *valor=m_insertar(map,(tClave)palabra,(*valor_recuperado+1));
    *valor_recuperado=m_recuperar(map,(tClave)palabra);
    if (valor==NULL)
        printf("El valor era nulo\n");
    else
        printf("el valor era: %d | el valor_recuperado es: %d\n \n",*valor,*valor_recuperado);



    palabra=NULL;
    palabra=(char*)malloc(sizeof(char));
    printf("Palabra 3: \n");
    scanf("%s",palabra);
    hash = funcion_hash_evaluador(palabra)%(map->longitud_tabla);
    *valor_recuperado=m_recuperar(map,(tClave)palabra);
    printf("valor_recuperado antes %d | %i \n",*valor_recuperado,hash);
    *valor=m_insertar(map,palabra,(*valor_recuperado+1));
    *valor_recuperado=m_recuperar(map,palabra);
    if (valor==NULL)
        printf("El valor era nulo\n");
    else
        printf("el valor era: %d | el valor_recuperado es: %d\n \n",*valor,*valor_recuperado);



    palabra=NULL;
    palabra=(char*)malloc(sizeof(char));
    printf("Palabra 4: \n");
    scanf("%s",palabra);
    hash = funcion_hash_evaluador(palabra)%(map->longitud_tabla);

    *valor_recuperado=m_recuperar(map,(tClave)palabra);
    printf("valor_recuperado antes %d | %i \n",*valor_recuperado,hash);
    *valor=m_insertar(map,palabra,(*valor_recuperado+1));
    *valor_recuperado=m_recuperar(map,palabra);
    if (valor==NULL)
        printf("El valor era nulo\n");
    else
        printf("el valor era: %d | el valor_recuperado es: %d\n \n",*valor,*valor_recuperado);


/*

    palabra=NULL;
    palabra=(char*)malloc(sizeof(char));
    printf("Palabra 5: \n");
    scanf("%s",palabra);
    *valor_recuperado=m_recuperar(map,palabra);
    printf("valor_recuperado antes %d | \n",*valor_recuperado);
    *valor=m_insertar(map,palabra,(*valor_recuperado+1));
    *valor_recuperado=m_recuperar(map,palabra);
    if (valor==NULL)
        printf("El valor era nulo\n");
    else
        printf("el valor era: %d | el valor_recuperado es: %d\n \n",*valor,*valor_recuperado);



    palabra=NULL;
    palabra=(char*)malloc(sizeof(char));
    printf("Palabra 6: \n");
    scanf("%s",palabra);
    *valor_recuperado=m_recuperar(map,palabra);
    printf("valor_recuperado antes %d | \n",*valor_recuperado);
    *valor=m_insertar(map,palabra,(*valor_recuperado+1));
    *valor_recuperado=m_recuperar(map,palabra);
    if (valor==NULL)
        printf("El valor era nulo\n");
    else
        printf("el valor era: %d | el valor_recuperado es: %d\n \n",*valor,*valor_recuperado);


    palabra=NULL;
    palabra=(char*)malloc(sizeof(char));
    printf("Palabra 7: \n");
    scanf("%s",palabra);
    *valor_recuperado=m_recuperar(map,palabra);
    printf("valor_recuperado antes %d | \n",*valor_recuperado);
    *valor=m_insertar(map,palabra,(*valor_recuperado+1));
    *valor_recuperado=m_recuperar(map,palabra);
    if (valor==NULL)
        printf("El valor era nulo\n");
    else
        printf("el valor era: %d | el valor_recuperado es: %d\n \n",*valor,*valor_recuperado);

*/

/*
    palabra=NULL;
    palabra=(char*)malloc(sizeof(char));
    printf("Palabra a eliminar: \n");
    scanf("%s",palabra);
    m_eliminar(map,palabra,&funcion_eliminar_claves_evaluador,&funcion_eliminar_valores_evaluador);


    palabra=NULL;
    palabra=(char*)malloc(sizeof(char));
    printf("Palabra a eliminar: \n");
    scanf("%s",palabra);
    m_eliminar(map,palabra,&funcion_eliminar_claves_evaluador,&funcion_eliminar_valores_evaluador);

    palabra=NULL;
    palabra=(char*)malloc(sizeof(char));
    printf("Palabra a eliminar: \n");
        scanf("%s",palabra);
    m_eliminar(map,palabra,&funcion_eliminar_claves_evaluador,&funcion_eliminar_valores_evaluador);

    palabra=NULL;
    palabra=(char*)malloc(sizeof(char));
    printf("Palabra a eliminar: \n");
    scanf("%s",palabra);
    m_eliminar(map,palabra,&funcion_eliminar_claves_evaluador,&funcion_eliminar_valores_evaluador);
*/
    printf("destruir romper todo\n");
    m_destruir(&map,funcion_eliminar_claves_evaluador,funcion_eliminar_valores_evaluador);
}


