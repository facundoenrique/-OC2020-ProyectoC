
#include <stdlib.h>
#include <stdio.h>
#include "mapeo.h"
#include "lista.h"

int comparacion_claves_evaluador(char* palabra1, char* palabra2)
{
    printf("palabra1: %s \n",palabra1);
    printf("palabra2: %s \n",palabra2);
    int i = 0;
    while ((*((palabra1+i))!='\0') && (*((palabra2+i))!='\0') && (*(palabra1+i)==*(palabra2+i)))
          i=i+1;
    if (*(palabra1+i)!='\0' && *(palabra2+i)!='\0')
       return 1;
    else return 0;

}

int main()    //LISTA CON ENTRADAS FUNCIONA BIEN
{

    tLista *lista;
    crear_lista(&lista);

    int *valor;
    valor=(int*)malloc(sizeof(int));

    struct entrada *nueva_entrada;
    nueva_entrada=(struct entrada*)malloc(sizeof(struct entrada));

    struct entrada *nueva_entrada1;
    nueva_entrada1=(struct entrada*)malloc(sizeof(struct entrada));

    struct entrada *nueva_entrada2;
    nueva_entrada2=(struct entrada*)malloc(sizeof(struct entrada));


    char *palabra;
    palabra=(char*)malloc(sizeof(char));
    printf("Palabra 1: \n");
    scanf("%s",palabra);

    //INSERTO EN LA LISTA:
    nueva_entrada->clave=palabra;
    nueva_entrada->valor=0;
    l_insertar(lista,l_primera(lista),nueva_entrada);

    tEntrada entrada_recuperada=(tEntrada)l_recuperar(lista,l_primera(lista));
    tEntrada entrada_recuperada1;
    tEntrada entrada_recuperada2;
    printf(" La clave es: %s \n",entrada_recuperada->clave);
    printf(" El valor es: %d \n",entrada_recuperada->valor);




    char *palabra2;
    palabra2=(char*)malloc(sizeof(char));
    printf("Palabra 2: \n");
    scanf("%s",palabra2);


    nueva_entrada1->clave=palabra2;
    nueva_entrada1->valor=1;

    l_insertar(lista,l_primera(lista),nueva_entrada1);
    entrada_recuperada=(tEntrada)l_recuperar(lista,l_primera(lista));
    printf(" 1) %s | %i\n",entrada_recuperada->clave,entrada_recuperada->valor);




    entrada_recuperada1=(tEntrada)l_recuperar(lista,l_siguiente(lista,l_primera(lista)));
    printf(" 2) %s | %i\n",entrada_recuperada1->clave,entrada_recuperada1->valor);


    char *palabra3;
    palabra3=(char*)malloc(sizeof(char));
    printf("Palabra 3: \n");
    scanf("%s",palabra3);



    nueva_entrada2->clave=palabra3;
    nueva_entrada2->valor=2;

    l_insertar(lista,l_primera(lista),nueva_entrada2);
    entrada_recuperada=(tEntrada)l_recuperar(lista,l_primera(lista));
    printf(" 1) %s | %i\n",entrada_recuperada->clave,entrada_recuperada->valor);


    entrada_recuperada1=(tEntrada)l_recuperar(lista,l_siguiente(lista,l_primera(lista)));
    printf(" 2) %s | %i\n",entrada_recuperada1->clave,entrada_recuperada1->valor);



    entrada_recuperada2=(tEntrada)l_recuperar(lista,l_siguiente(lista,l_siguiente(lista,l_primera(lista))));
    printf(" 3) %s | %i\n",entrada_recuperada2->clave,entrada_recuperada2->valor);


/*
    int comp=comparacion_claves_evaluador(entrada_recuperada->clave,entrada_recuperada1->clave);
    if (comp==0)
        printf("Las claves son iguales\n");
    else
        printf("No son iguales\n");
*/




}





