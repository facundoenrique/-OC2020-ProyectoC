#include <stdlib.h>
#include <stdio.h>
#include "mapeo.h"
#include "lista.h"

// ---------------------------------------------------------------------------------------------
// Definicion de prototipos y documentacion de las funciones.
// Obs.: la tabla hash debe mantener control del factor de carga en todo momento.
// Obs.: el factor de carga maximo permitido equivale al 75% de la longitud de la tabla.
// ---------------------------------------------------------------------------------------------

/**
    Funciones globales para usarlas en m_eliminar y m_destruir
**/

void (*funcion_eliminar_valores)(void*);
void (*funcion_eliminar_claves)(void*);



/**
    Funcion que busca el mayor numero entre a y b
**/

int MAX(int a, int b)
{
    if (a>b)
        return a;
    else
        return b;
}


/**
    ESTA FUNCION NO HACE NADA Y SE USA EN INSERTAR CUANDO HAY QUE AGRANDAR LA TABLA HASH.
    Se quiere borrar la lista vieja sin borrar las entradas que son copiadas a la lista nueva.
**/
void funcion_eliminar_entradas_vacia()
{
}


/**

**/
void funcion_eliminar_entradas_nueva(tElemento e)
{
    tEntrada entrada=(tEntrada) e;
    free(entrada);
}


/**
 Inicializa un mapeo vacio, con capacidad inicial igual al MAX(10, CI).
 Una referencia al mapeo creado es referenciada en *M.
 A todo efecto, el valor hash para las claves sera computado mediante la funcion fHash.
 A todo efecto, la comparacion de claves se realizara mediante la funcion fComparacion.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/

extern void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *))
{
    *m=(tMapeo)malloc(sizeof(struct mapeo));
    if (*m==NULL)
        exit(MAP_ERROR_MEMORIA);
    int longitud_t=MAX(10,ci);
    (*m)->longitud_tabla=longitud_t;
    (*m)->cantidad_elementos = 0;
    (*m)->comparador=fComparacion;
    (*m)->hash_code=fHash;

    (*m)->tabla_hash=(tLista*)malloc(sizeof(tLista)*longitud_t);
    if ((*m)->tabla_hash==NULL)
        exit(MAP_ERROR_MEMORIA);

    for (int i=0;i!=longitud_t;i++)
    {
        crear_lista(&*((*m)->tabla_hash+i));
    }
}



/**
 Inserta una entrada con clave C y valor V, en M.
 Si una entrada con clave C y valor Vi ya existe en M, modifica Vi por V.
 Retorna NULL si la clave C no existia en M, o Vi en caso contrario.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/

extern tValor m_insertar(tMapeo m, tClave c, tValor v)
{
    //-------------------------------------------------------------------------------------------------//
    //CONTROLO SI LA CANTIDAD DE ELEMENTOS AGREGANDO LA NUEVA ENTRADA ES MAYOR O IGUAL AL 75%
    //De ser asi agrando el arreglo, inicializo las nuevas listas y copio las entradas en las posiciones correspondientes
    //de la nueva lista



    if ((m->cantidad_elementos)+1>=(75*(m->longitud_tabla)/100))
    {

        int longitud_anterior=m->longitud_tabla;
        int longitud_nueva=longitud_anterior*2;
        tLista *tabla_anterior=m->tabla_hash;
        m->longitud_tabla=longitud_nueva;
        m->tabla_hash=(tLista*)malloc(sizeof(tLista)*longitud_nueva);
        if (m->tabla_hash==NULL)
            exit(MAP_ERROR_MEMORIA);
        for (int i=0;i!=longitud_nueva;i++)
        {
            crear_lista(&*(m->tabla_hash+i));
        }

        for (int i=0;i!=longitud_anterior;i++)
        {
            //VEO SI LA LISTA DE ENTRADAS ANTERIOR NO ESTA VACIA
            //VA *m o m solo? Con m solo da Warning con m* no
            if(l_longitud(*(tabla_anterior+i))!=0)
            {
                tPosicion posicion_lista_entradas=l_primera(*(tabla_anterior+i));
                for (int j=0;j!=l_longitud(*(tabla_anterior+i));j++)
                {
                    tEntrada entrada_anterior=(tEntrada) l_recuperar(*(tabla_anterior+i),posicion_lista_entradas);
                    //TEXTO PARA VER QUE HAY EN LA ENTRADA QUE SE COPIA EN LA TABLA NUEVA:

                    tClave clave_anterior=entrada_anterior->clave;
                    int clave_nueva=(m->hash_code(clave_anterior))%(m->longitud_tabla);

                    l_insertar(*(m->tabla_hash+clave_nueva),l_primera(*(m->tabla_hash+clave_nueva)),entrada_anterior);
                    posicion_lista_entradas=l_siguiente(*(tabla_anterior+i),posicion_lista_entradas);
                }
            }

            l_destruir(&*(tabla_anterior+i),funcion_eliminar_entradas_vacia);


        }


    }



    //--------------------------------------------------------------------------------------------------// TERMINO DE AGRANDAR EL ARREGLO

    int valor_hash=(m->hash_code(c))%(m->longitud_tabla);
    tValor valor_a_retornar=NULL;


    if (l_longitud(*(m->tabla_hash+valor_hash))==0)
    {
        tEntrada nueva_entrada=(tEntrada)malloc(sizeof(struct entrada));
        if (nueva_entrada==NULL)
            exit(MAP_ERROR_MEMORIA);
        nueva_entrada->clave=c;
        nueva_entrada->valor=v;
        l_insertar(*(m->tabla_hash+valor_hash),l_primera(*(m->tabla_hash+valor_hash)),nueva_entrada);
        m->cantidad_elementos=(m->cantidad_elementos)+1;
    }
    else
    {

        int esta_la_clave=0;

        //en estos 2 renglones que siguen pasan cosas raras.
        tPosicion posicion_lista=l_primera(*(m->tabla_hash+valor_hash));
        tEntrada entrada_que_se_esta_viendo=(tEntrada)l_recuperar(*(m->tabla_hash+valor_hash),posicion_lista);

        while (esta_la_clave==0 && entrada_que_se_esta_viendo!=(tEntrada)l_recuperar(*(m->tabla_hash+valor_hash),l_ultima(*(m->tabla_hash+valor_hash))))
        {
            //Si la clave que se esta viendo y la que se quiere insertar son iguales el comparador da cero
            if (m->comparador(entrada_que_se_esta_viendo->clave,c)==0){
                esta_la_clave=1;
            }

            else
            {
                    posicion_lista=l_siguiente(*(m->tabla_hash+valor_hash),posicion_lista);
                    entrada_que_se_esta_viendo=l_recuperar(*(m->tabla_hash+valor_hash),posicion_lista);
            }

        }


        if (esta_la_clave==0 && m->comparador(entrada_que_se_esta_viendo->clave,c)==0)
           {
                esta_la_clave=1;
           }

        if (esta_la_clave==1)
        {
            valor_a_retornar=entrada_que_se_esta_viendo->valor;
            entrada_que_se_esta_viendo->valor=v;
        }
        else
        {
            tEntrada nueva_entrada=(tEntrada)malloc(sizeof(struct entrada));
            if (nueva_entrada==NULL)
                exit(MAP_ERROR_MEMORIA);
            nueva_entrada->clave=c;
            nueva_entrada->valor=v;
            l_insertar(*(m->tabla_hash+valor_hash),l_primera(*(m->tabla_hash+valor_hash)),nueva_entrada);
            m->cantidad_elementos=(m->cantidad_elementos)+1;
        }

    }

    return valor_a_retornar;

}

/**
 Elimina la entrada con clave C en M, si esta existe.
 La clave y el valor de la entrada son eliminados mediante las funciones fEliminarC y fEliminarV.
**/

extern void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *))
{
    int clave=(m->hash_code(c))%(m->longitud_tabla);
    int encontre_la_clave=0;
    //funcion_eliminar_claves=fEliminarC;
    //funcion_eliminar_valores=fEliminarV;
    if(l_longitud(*(m->tabla_hash+clave))!=0)
    {
    tPosicion posicion_lista_entradas=l_primera(*(m->tabla_hash+clave));
    tEntrada entrada_que_se_esta_viendo=(tEntrada)l_recuperar(*(m->tabla_hash+clave),posicion_lista_entradas);


    while (encontre_la_clave==0 && entrada_que_se_esta_viendo!=(tEntrada)l_recuperar(*(m->tabla_hash+clave),l_ultima(*(m->tabla_hash+clave))))
    {
       if (m->comparador(entrada_que_se_esta_viendo->clave,c)==0)
       {
            encontre_la_clave=1;
       }
        else
        {
            posicion_lista_entradas=l_siguiente(*(m->tabla_hash+clave),posicion_lista_entradas);
            entrada_que_se_esta_viendo=(tEntrada)l_recuperar(*(m->tabla_hash+clave),posicion_lista_entradas);
        }
    }

    if (encontre_la_clave==0 && (m->comparador(entrada_que_se_esta_viendo->clave,c)==0))
    {
       encontre_la_clave=1;
    }

    if (encontre_la_clave==1)
    {

        fEliminarC(entrada_que_se_esta_viendo->clave);

        fEliminarV(entrada_que_se_esta_viendo->valor);

        l_eliminar(*(m->tabla_hash+clave),posicion_lista_entradas,&funcion_eliminar_entradas_nueva);

        m->cantidad_elementos=(m->cantidad_elementos)-1;



    }

    }


}

/**
 Destruye el mapeo M, elimininando cada una de sus entradas.
 Las claves y valores almacenados en las entradas son eliminados mediante las funciones fEliminarC y fEliminarV.
**/
extern void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *))
{
    funcion_eliminar_claves=fEliminarC;
    funcion_eliminar_valores=fEliminarV;
    tMapeo mapeo_auxiliar=*m;


    // SEGUNDA FORMA
    //int cantidad_arreglo=(*m)->longitud_tabla;
    int cantidad_arreglo=mapeo_auxiliar->longitud_tabla;

    for(int i=0;i!=cantidad_arreglo;i++)
    {
        //if(*((*m)->tabla_hash+i)!=NULL)
        if (*(mapeo_auxiliar->tabla_hash+i)!=NULL)
        {
        int j=0;
        while (j!=l_longitud(*(mapeo_auxiliar->tabla_hash+i)))
        {
            tEntrada entrada_a_eliminar=l_recuperar(*(mapeo_auxiliar->tabla_hash+i),l_primera(*(mapeo_auxiliar->tabla_hash+i)));
            fEliminarC(entrada_a_eliminar->clave);
            fEliminarV(entrada_a_eliminar->valor);
            l_eliminar(*(mapeo_auxiliar->tabla_hash+i),l_primera(*(mapeo_auxiliar->tabla_hash+i)),&funcion_eliminar_entradas_nueva);

        }
        l_destruir(&(*(mapeo_auxiliar->tabla_hash+i)),&funcion_eliminar_entradas_vacia);
        }
    }
}

/**
 Recupera el valor correspondiente a la entrada con clave C en M, si esta existe.
 Retorna el valor correspondiente, o NULL en caso contrario.
**/

extern tValor m_recuperar(tMapeo m, tClave c)
{


    tValor valor_a_retornar=NULL;
    int valor_hash=(m->hash_code(c))%(m->longitud_tabla);


    if (l_longitud(*(m->tabla_hash+valor_hash))>0)
    {

        int encontre_la_entrada=0;

        tLista *l = (tLista*) *(m->tabla_hash+valor_hash);
        tPosicion posicion_lista_entradas = (tPosicion)l_primera((tLista)l);
        tEntrada entrada_que_se_esta_viendo= NULL;
        entrada_que_se_esta_viendo = (tEntrada)l_recuperar((tLista)l,posicion_lista_entradas);


        int i = 0;

        while (encontre_la_entrada==0 && posicion_lista_entradas!=l_ultima((tLista)l))
        {


                if (m->comparador(entrada_que_se_esta_viendo->clave,c)==0)
                {
                    encontre_la_entrada=1;
                    valor_a_retornar=entrada_que_se_esta_viendo->valor;
                }
                else
                {
                    posicion_lista_entradas=l_siguiente(*(m->tabla_hash+valor_hash),posicion_lista_entradas);
                    entrada_que_se_esta_viendo=(tEntrada)l_recuperar(*(m->tabla_hash+valor_hash),posicion_lista_entradas);
                }


            i++;
        }



        if (encontre_la_entrada==0 && m->comparador(entrada_que_se_esta_viendo->clave,c)==0)
            {
                valor_a_retornar=entrada_que_se_esta_viendo->valor;

            }
    }
    return valor_a_retornar;
}


