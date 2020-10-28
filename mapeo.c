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
}
/**
    ESTA FUNCION NO HACE NADA Y SE USA EN INSERTAR CUANDO HAY QUE AGRANDAR LA TABLA HASH.
    Se quiere borrar la lista vieja sin borrar las entradas que son copiadas a la lista nueva.
**/
void funcion_eliminar_entradas_vacia()
{
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
        int longitud_nueva=(m->longitud_tabla)*2;
        tLista *nuevo_arreglo=(tLista*)malloc(sizeof(tLista)*longitud_nueva);
        if(nuevo_arreglo==NULL)
            exit(MAP_ERROR_MEMORIA);
        for (int i=0;i!=longitud_nueva;i++)
        {
        crear_lista(&*(nuevo_arreglo+i));
        }

        int longitud_anterior=m->longitud_tabla;
        m->longitud_tabla=longitud_nueva;
        for (int i=0;i!=longitud_anterior;i++)
        {
            //VEO SI LA LISTA DE ENTRADAS ANTERIOR NO ESTA VACIA
            //VA *m o m solo? Con m solo da Warning con m* no
            if(l_longitud(*(m->tabla_hash+i))!=0)
            {
                tPosicion posicion_lista_entradas=l_primera(*(m->tabla_hash+i));
                for (int j=0;j!=l_longitud(*(m->tabla_hash+i));j++)
                {
                    tEntrada entrada_anterior=(tEntrada) l_recuperar((*(m->tabla_hash+i)),posicion_lista_entradas);
                    tClave clave_anterior=entrada_anterior->clave;
                    int clave_nueva=(m->hash_code(clave_anterior))%(m->longitud_tabla);
                    l_insertar(*(nuevo_arreglo+clave_nueva),l_primera(*(nuevo_arreglo+clave_nueva)),entrada_anterior);
                    posicion_lista_entradas=l_siguiente(*(m->tabla_hash+i),posicion_lista_entradas);
                }
            }

            l_destruir(&*(m->tabla_hash+i),funcion_eliminar_entradas_vacia);
            //Esto es para ver si se eliminaron correctamente, HAY QUE BORRARLO DESPUES
            if (*(m->tabla_hash+i)==NULL)
                printf("Se elimino ok \n");
            else
                printf("no se elimino \n");
						//-------------------------------------------------------//
           *(m->tabla_hash+i)=*(nuevo_arreglo+i);
        }
       (m->tabla_hash)=nuevo_arreglo;
       // o *(m->tabla_hash)=*nuevo_arreglo; ?
    }

//--------------------------------------------------------------------------------------------------// TERMINO DE AGRANDAR EL ARREGLO

    int clave=(m->hash_code(c))%(m->longitud_tabla);
    tValor valor_a_retornar=NULL;

    if (l_longitud(*(m->tabla_hash+clave))==0)
    {
        struct entrada *nueva_entrada=(struct entrada*)malloc(sizeof(struct entrada));
        if (nueva_entrada==NULL)
            exit(MAP_ERROR_MEMORIA);
        nueva_entrada->clave=c;
        nueva_entrada->valor=v;
        l_insertar(*(m->tabla_hash+clave),l_primera(*(m->tabla_hash+clave)),nueva_entrada);
        m->cantidad_elementos=(m->cantidad_elementos)+1;
    }
    else
    {
        int esta_la_clave=0;
        tPosicion posicion_lista=l_primera(*(m->tabla_hash+clave));
        tEntrada entrada_que_se_esta_viendo=(tEntrada)l_recuperar(*(m->tabla_hash+clave),posicion_lista);

        while (esta_la_clave==0 && entrada_que_se_esta_viendo!=(tEntrada)l_recuperar(*(m->tabla_hash+clave),l_ultima(*(m->tabla_hash+clave))))
        {
            //Si la clave que se esta viendo y la que se quiere insertar son iguales el comparador da cero
            if (m->comparador(entrada_que_se_esta_viendo->clave,c)==0)
                esta_la_clave=1;
            else
            {
                    posicion_lista=l_siguiente(*(m->tabla_hash+clave),posicion_lista);
                    entrada_que_se_esta_viendo=l_recuperar(*(m->tabla_hash+clave),posicion_lista);
            }

        }

        if (esta_la_clave==0 && m->comparador(entrada_que_se_esta_viendo->clave,c)==0)
            esta_la_clave=1;

        if (esta_la_clave==1)
        {
            valor_a_retornar=entrada_que_se_esta_viendo->valor;
            entrada_que_se_esta_viendo->valor=v;
        }
        else
        {
            struct entrada *nueva_entrada=(struct entrada*)malloc(sizeof(struct entrada));
            if (nueva_entrada==NULL)
                exit(MAP_ERROR_MEMORIA);
            nueva_entrada->clave=c;
            nueva_entrada->valor=v;
            l_insertar(*(m->tabla_hash+clave),l_primera(*(m->tabla_hash+clave)),nueva_entrada);
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
    funcion_eliminar_claves=fEliminarC;
    funcion_eliminar_valores=fEliminarV;
    tPosicion posicion_lista_entradas=l_primera(*(m->tabla_hash+clave));
    tEntrada entrada_que_se_esta_viendo=(tEntrada)l_recuperar(*(m->tabla_hash+clave),posicion_lista_entradas);

    while (encontre_la_clave==0 && entrada_que_se_esta_viendo!=(tEntrada)l_recuperar(*(m->tabla_hash+clave),l_ultima(*(m->tabla_hash+clave))))
    {
       if (m->comparador(entrada_que_se_esta_viendo->clave,c)==0)
            encontre_la_clave=1;
        else
        {
            posicion_lista_entradas=l_siguiente(*(m->tabla_hash+clave),posicion_lista_entradas);
            entrada_que_se_esta_viendo=(tEntrada)l_recuperar(*(m->tabla_hash+clave),posicion_lista_entradas);
        }
    }

    if (encontre_la_clave==0 && (m->comparador(entrada_que_se_esta_viendo->clave,c)==0))
        encontre_la_clave=1;

    if (encontre_la_clave==1)
    {
        l_eliminar(*(m->tabla_hash+clave),posicion_lista_entradas,&funcion_eliminar_entradas);
       	m->cantidad_elementos=(m->cantidad_elementos)-1;
        entrada_que_se_esta_viendo->clave=NULL;
        entrada_que_se_esta_viendo->valor=NULL;
        entrada_que_se_esta_viendo=NULL;
}
}

/**
 Destruye el mapeo M, elimininando cada una de sus entradas.
 Las claves y valores almacenados en las entradas son eliminados mediante las funciones fEliminarC y fEliminarV.
**/
void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *))
{
    funcion_eliminar_claves=fEliminarC;
    funcion_eliminar_valores=fEliminarV;
    for (int i=0;i!=(*m)->longitud_tabla;i++)
    {
        //l_destruir(&*((*m)->tabla_hash+i),&funcion_eliminar_entradas); o asi?
        l_destruir((*m)->tabla_hash+i,&funcion_eliminar_entradas);
        free((*m)->tabla_hash+i);
    }
    free(m);
    *m=NULL;
    //m=NULL;
}

/**
 Recupera el valor correspondiente a la entrada con clave C en M, si esta existe.
 Retorna el valor correspondiente, o NULL en caso contrario.
**/

extern tValor m_recuperar(tMapeo m, tClave c)
{
    tValor valor_a_retornar=NULL;
    int clave=(m->hash_code(c))%(m->longitud_tabla);
    if (l_longitud(*(m->tabla_hash+clave))!=0)
    {
        int encontre_la_entrada=0;
        tPosicion posicion_lista_entradas=l_primera(*(m->tabla_hash+clave));
        tEntrada entrada_que_se_esta_viendo=(tEntrada)l_recuperar(*(m->tabla_hash+clave),posicion_lista_entradas);
        while (encontre_la_entrada==0 && entrada_que_se_esta_viendo!=(tEntrada)l_recuperar(*(m->tabla_hash+clave),l_ultima(*(m->tabla_hash+clave))))
        {
            if (m->comparador(entrada_que_se_esta_viendo->clave,c)==0)
            {
                encontre_la_entrada=1;
                valor_a_retornar=entrada_que_se_esta_viendo->valor;
            }
            else
            {
                posicion_lista_entradas=l_siguiente(*(m->tabla_hash+clave),posicion_lista_entradas);
                entrada_que_se_esta_viendo=(tEntrada)l_recuperar(*(m->tabla_hash+clave),posicion_lista_entradas);
            }
        }
        if (encontre_la_entrada==0 && m->comparador(entrada_que_se_esta_viendo->clave,c)==0)
            valor_a_retornar=entrada_que_se_esta_viendo->valor;
    }
    return valor_a_retornar;
}


