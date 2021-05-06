#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "defendiendo_torres.h"
#include "utiles.h"
#include "animos.h"

#define CARACTER_DEFAULT 'A'
#define ESPACIO 	"        "
#define VERDE 		"\033[1m\033[32m"
#define FIN  		"\033[0m"
#define LINEA 		"\n-------------------------------------------------\n"
#define AMARILLO 	"\033[1m\033[33m"
#define BLANCO 		"\033[1m\033[37m"
#define ROJO 		"\033[1m\033[31m"
#define AZUL 		"\033[1m\033[34m"
#define MAGENTA 	"\033[1m\033[35m"
#define FONDO 	    "\033[1m\033[40m"

#define MAX_COLOR  			17
#define MAX_NOMBRE_DEFENSOR 10

#define ESTE 	'E'
#define OESTE 	'O'
#define NORTE 	'N'
#define SUR 	'S'

#define ELFO 	'L'
#define ENANO 	'G'

#define VIDA_INICIAL_TORRE_1	600
#define VIDA_INICIAL_TORRE_2	600
#define COSTO_DEF_EXTRA_TORRE_1 50
#define COSTO_DEF_EXTRA_TORRE_2 50
#define DEF_EXTRA_TORRE_1 		"ENANO"
#define DEF_EXTRA_TORRE_2 		"ELFO"

#define VERDADERO				1
#define FALSO					0

#define TAMANIO_NIVEL_1 				15
#define CARDINAL_NIVEL_1 				ESTE
#define ENEMIGOS_NIVEL_1 				100
#define ELFOS_NIVEL_1 					0
#define ENANOS_NIVEL_1  				5
#define PODER_AGREGAR_ELFO_NIVEL_1		FALSO
#define PODER_AGREGAR_ENANO_NIVEL_1		VERDADERO
#define ENEMIGOS_DEFENSOR_EXTRA_NIVEL_1 25
#define CANT_CAMINOS_NIVEL_1			1

#define TAMANIO_NIVEL_2 				15
#define CARDINAL_NIVEL_2 				OESTE
#define ENEMIGOS_NIVEL_2 				200
#define ELFOS_NIVEL_2 					5
#define ENANOS_NIVEL_2  				0
#define PODER_AGREGAR_ELFO_NIVEL_2		VERDADERO
#define PODER_AGREGAR_ENANO_NIVEL_2		FALSO
#define ENEMIGOS_DEFENSOR_EXTRA_NIVEL_2 50
#define CANT_CAMINOS_NIVEL_2			1

#define TAMANIO_NIVEL_3			 		20
#define CARDINAL_NIVEL_3 				NORTE
#define ENEMIGOS_NIVEL_3 				300
#define ELFOS_NIVEL_3 					3
#define ENANOS_NIVEL_3  				3
#define PODER_AGREGAR_ELFO_NIVEL_3		VERDADERO
#define PODER_AGREGAR_ENANO_NIVEL_3		VERDADERO
#define ENEMIGOS_DEFENSOR_EXTRA_NIVEL_3 50
#define CANT_CAMINOS_NIVEL_3			2

#define TAMANIO_NIVEL_4					20
#define CARDINAL_NIVEL_4 				SUR
#define ENEMIGOS_NIVEL_4 				500
#define ELFOS_NIVEL_4 					4
#define ENANOS_NIVEL_4  				4
#define PODER_AGREGAR_ELFO_NIVEL_4		VERDADERO
#define PODER_AGREGAR_ENANO_NIVEL_4		VERDADERO
#define ENEMIGOS_DEFENSOR_EXTRA_NIVEL_4 50
#define CANT_CAMINOS_NIVEL_4			2

#define VACIO 							0

#define DISTANCIA_ENTRADA	3
#define DISTANCIA_TORRES	3

#define POSIBLE 0
#define NO_POSIBLE -1

#define PERDIDO -1
#define JUGANDO 0
#define GANADO 1

#define TIEMPO_ENTRE_TURNOS 0.5

#define SI "S"
#define NO "N"

#define MARGEN 		"     "

/*Pre-condiciones: 
    "tope" debe ser > 0;
Post-condiciones: 
    la función retornará un valor entre 0 y tope.*/
int generar_posicion(int tope){
	
	return (rand()%tope); 
}

/*Pre-condiciones: 
    "nivel" debe ser 1, 2, 3 o 4;
Post-condiciones: 
    la función retorna "tamanio" que es TAMANIO_NIVEL_1, TAMANIO_NIVEL_2, TAMANIO_NIVEL_3 o TAMANIO_NIVEL_4.*/
int tamanio_tablero_segun_nivel(int nivel){
	int tamanio;

	switch (nivel){
		case 1:
			tamanio = TAMANIO_NIVEL_1;
			break;
		case 2:
			tamanio = TAMANIO_NIVEL_2;
			break;
		case 3:
			tamanio = TAMANIO_NIVEL_3;
			break;
		case 4:
			tamanio = TAMANIO_NIVEL_4;
			break;
	}

	return tamanio;
}

/*Pre-condiciones: 
    "nivel" debe ser 1, 2, 3 o 4.
Post-condiciones: 
    la función retorna un valor que puede ser CARDINAL_NIVEL_1, CARDINAL_NIVEL_2, CARDINAL_NIVEL_3 o CARDINAL_NIVEL_4.*/
char punto_cardinal_segun_nivel(int nivel){
	char punto_cardinal;

	switch (nivel){
		case 1:
			punto_cardinal = CARDINAL_NIVEL_1;
			break;	
		case 2:
			punto_cardinal = CARDINAL_NIVEL_2;
			break;
		case 3:
			punto_cardinal = CARDINAL_NIVEL_3;
			break;
		case 4:
			punto_cardinal = CARDINAL_NIVEL_4;
			break;
	}

	return punto_cardinal;
}

/*Pre-condiciones: 
    "punto_cardinal" debe ser NORTE, SUR, ESTE u OESTE;
    "tope" positivo, con valores que pueden ser TAMANIO_NIVEL_1, TAMANIO_NIVEL_2, TAMANIO_NIVEL_3 o TAMANIO_NIVEL_4.
Post-condiciones: 
    la función modifica los valores en las posiciones recibidas con valores >= 0 y < "tope" dependiendo del valor de "punto_cardinal".*/
void generar_entrada_y_torre_segun_pto_cardinal(int punto_cardinal, coordenada_t *entrada, coordenada_t *torre, int tope){
	
	switch(punto_cardinal){
		case ESTE:
			entrada->fil = generar_posicion(tope);
			entrada->col = tope - 1;
			torre->fil = generar_posicion(tope);
			torre->col = 0;
			break;
		case OESTE:
			entrada->fil = generar_posicion(tope);
			entrada->col = 0;
			torre->fil = generar_posicion(tope);
			torre->col = tope - 1;
			break;
		case NORTE:
			entrada->fil = 0;
			entrada->col = generar_posicion(tope);
			torre->fil = tope - 1;
			torre->col = generar_posicion(tope);
			break;
		case SUR:
			entrada->fil = tope - 1;
			entrada->col = generar_posicion(tope);
			torre->fil = 0;
			torre->col = generar_posicion(tope);
			break;
	}
}

/*Pre-condiciones: 
    "nivel" debe ser 1, 2, 3 o 4.
Post-condiciones: 
    la función retorna un valor que puede ser CANT_CAMINOS_NIVEL_1, CANT_CAMINOS_NIVEL_2, CANT_CAMINOS_NIVEL_3 o CANT_CAMINOS_NIVEL_4.*/
int cantidad_caminos_segun_nivel(int nivel){
	int cantidad;

	switch(nivel){
		case 1:
			cantidad = CANT_CAMINOS_NIVEL_1;
			break;
		case 2:
			cantidad = CANT_CAMINOS_NIVEL_2;
			break;
		case 3:
			cantidad = CANT_CAMINOS_NIVEL_3;
			break;
		case 4:
			cantidad = CANT_CAMINOS_NIVEL_4;
			break;
	}

	return cantidad;
}

/*Pre-condiciones: 
    "punto" debe ser NORTE o SUR.
Post-condiciones: 
    la función retorna VERDADERO si "columna_torre_1" y "columna_torre_2" no están colocadas válidamente según "punto" o si la distancia entre ellas es < DISTANCIA_TORRES.*/
bool torres_no_geograficamente_segun_pto_card(int columna_torre_1, int columna_torre_2, char punto){
	bool respuesta;

	if (punto == NORTE)
		respuesta = ( (columna_torre_1 > columna_torre_2) || ( abs(columna_torre_1 - columna_torre_2) < DISTANCIA_TORRES ) );
	else
		respuesta = ( (columna_torre_2 > columna_torre_1) || ( abs(columna_torre_1 - columna_torre_2) < DISTANCIA_TORRES ) );
	
	return respuesta;
}

/*Pre-condiciones: 
    "punto" debe ser NORTE o SUR.
Post-condiciones: 
    la función retorna VERDADERO si "columna_entrada_1" y "columna_entrada_2" no están colocadas válidamente según "punto" o si la distancia entre ellas es < DISTANCIA_TORRES.*/
bool entradas_no_geograficamente_segun_pto_card(int columna_entrada_1, int columna_entrada_2, char punto){
	bool respuesta;

	if (punto == NORTE)
		respuesta = ( (columna_entrada_1 > columna_entrada_2) || ( abs(columna_entrada_1 - columna_entrada_2) < DISTANCIA_ENTRADA ) );
	else
		respuesta = ( (columna_entrada_2 > columna_entrada_1) || ( abs(columna_entrada_1 - columna_entrada_2) < DISTANCIA_ENTRADA ) );

	return respuesta;
}

/*Pre-condiciones: 
    "tope" debe ser positivo y menor a "MAX_LONGITUD_CAMINO".
Post-condiciones: 
    la función retorna VERDADERO si "entrada" o "torre" son CAMINO en "camino[]".*/
bool hay_camino_en_entradas (coordenada_t camino[MAX_LONGITUD_CAMINO], int tope, coordenada_t entrada, coordenada_t torre){
	bool respuesta = false;

	for (int i = 0; i < tope; i++){

		if ( (camino[i].fil == entrada.fil) && (camino[i].col == entrada.col) )
			respuesta = true;
		else if ( (camino[i].fil == torre.fil) && (camino[i].col == torre.col) )
			respuesta = true;
	}

	return respuesta;
}

/*Pre-condiciones: 
    "tope_obtenido" debe ser positivo y menor a "MAX_LONGITUD_CAMINO".
Post-condiciones: 
    la función modifica los valores en las posiciones recibidas con "camino_obtenido" según "tope_obtenido".*/
void cargar_camino(int *tope, int tope_obtenido, coordenada_t camino[MAX_LONGITUD_CAMINO], coordenada_t camino_obtenido[MAX_LONGITUD_CAMINO]){
	
	*tope = tope_obtenido;
	for (int i = 0; i < (*tope); i++)
		camino[i] = camino_obtenido[i];
}

/*Pre-condiciones: 
    "nivel_actual" debe ser 1, 2, 3 o 4;
    "tamanio_tablero" positivo y menor a MAX_FILAS y MAX_COLUMNAS;
Post-condiciones: 
    la función modificará las posiciones de "nivel" a partir de los datos obtenidos, con los caminos generados.*/
void obtener_camino_segun_nivel(nivel_t *nivel, int nivel_actual, int tamanio_tablero){
	int tope_camino_1, tope_camino_2, cant_caminos;
	char punto_cardinal;
	coordenada_t camino_1[MAX_LONGITUD_CAMINO], entrada_1, torre_1;
	coordenada_t camino_2[MAX_LONGITUD_CAMINO], entrada_2, torre_2;

	punto_cardinal = punto_cardinal_segun_nivel(nivel_actual);
	cant_caminos = cantidad_caminos_segun_nivel(nivel_actual);

	if (cant_caminos == 2){

		do{

			generar_entrada_y_torre_segun_pto_cardinal(punto_cardinal, &entrada_1, &torre_1, tamanio_tablero);
			generar_entrada_y_torre_segun_pto_cardinal(punto_cardinal, &entrada_2, &torre_2, tamanio_tablero);
		} while( ( torres_no_geograficamente_segun_pto_card(torre_1.col, torre_2.col, punto_cardinal) )
			|| ( entradas_no_geograficamente_segun_pto_card(entrada_1.col, entrada_2.col, punto_cardinal) ) );	
		do{

			obtener_camino(camino_1, &tope_camino_1, entrada_1, torre_1);
			obtener_camino(camino_2, &tope_camino_2, entrada_2, torre_2);
		} while ( ( hay_camino_en_entradas(camino_1, tope_camino_1,entrada_2, torre_2) )
			|| ( hay_camino_en_entradas(camino_2, tope_camino_2, entrada_1, torre_1) ) );

		cargar_camino(&(nivel->tope_camino_2), tope_camino_2, nivel->camino_2, camino_2);
	} else{

		generar_entrada_y_torre_segun_pto_cardinal(punto_cardinal, &entrada_1, &torre_1, tamanio_tablero);
		obtener_camino(camino_1, &tope_camino_1, entrada_1, torre_1);
	}
	cargar_camino(&(nivel->tope_camino_1), tope_camino_1, nivel->camino_1, camino_1);
}

/*Pre-condiciones: 
    "nivel" debe ser 1, 2, 3 o 4;
Post-condiciones: 
    la función retorna un valor que puede ser ENEMIGOS_NIVEL_1, ENEMIGOS_NIVEL_2, ENEMIGOS_NIVEL_3 o ENEMIGOS_NIVEL_4.*/
int enemigos_nivel_actual(int nivel){
	int enemigos;

	switch (nivel){
		case 1:
			enemigos = ENEMIGOS_NIVEL_1;
			break;		
		case 2:
			enemigos = ENEMIGOS_NIVEL_2;
			break;
		case 3:
			enemigos = ENEMIGOS_NIVEL_3;
			break;
		case 4:
			enemigos = ENEMIGOS_NIVEL_4;
			break;
	}

	return enemigos;
}

/*Pre-condiciones: 
    "nivel" debe ser 1, 2, 3 o 4.
Post-condiciones: 
    la función retorna un valor que puede ser ENANOS_NIVEL_1, ENANOS_NIVEL_2, ENANOS_NIVEL_3 o ENANOS_NIVEL_4.*/
int cant_enanos(int nivel){
	int valor;
	
	switch(nivel){
		case 1:
			valor = ENANOS_NIVEL_1;
			break;
		case 2:
			valor = ENANOS_NIVEL_2;
			break;
		case 3:
			valor = ENANOS_NIVEL_3;
			break;
		case 4:
			valor = ENANOS_NIVEL_4;
			break;
	}

	return valor;
}

/*Pre-condiciones: 
    "nivel" debe ser 1, 2, 3 o 4.
Post-condiciones: 
    la función retorna un valor que puede ser ELFOS_NIVEL_1, ELFOS_NIVEL_2, ELFOS_NIVEL_3 o ELFOS_NIVEL_4.*/
int cant_elfos(int nivel){
	int valor;

	switch(nivel){
		case 1:
			valor = ELFOS_NIVEL_1;
			break;
		case 2:
			valor = ELFOS_NIVEL_2;
			break;
		case 3:
			valor = ELFOS_NIVEL_3;
			break;
		case 4:
			valor = ELFOS_NIVEL_4;
			break;
	}

	return valor;
}

/*Pre-condiciones: 
    -
Post-condiciones: 
    la función modifica los valores de las posiciones recibidas según ingreso.*/
void pedir_coordenadas(coordenada_t *posicion){

	printf("Indique numero fila:\n");
	scanf("%i", &(posicion->fil));
	printf("Indique numero columna:\n");
	scanf("%i", &(posicion->col));
}

/*Pre-condiciones: 
    "tope" debe ser positivo;
Post-condiciones: 
    la función retorna VERDADERO si "fila" no está entre 0 y "tope".*/
bool fila_invalida(int fila, int tope){

	return ( (fila < 0) || (fila > tope) );
}

/*Pre-condiciones: 
    "tope" debe ser positivo;
Post-condiciones: 
    la función retorna VERDADERO si "columna" no está entre 0 y "tope".*/
bool columna_invalida(int columna, int tope){

	return ( (columna < 0) || (columna > tope) );
}

/*Pre-condiciones: 
    "tope" debe ser positivo;
Post-condiciones: 
    la función retorna VERDADERO si "posicion.col" o "posicion.fil" no estás entre 0 y "tope".*/
bool rango_invalido(int tope, coordenada_t posicion){
	
	return(fila_invalida(posicion.fil, tope) || columna_invalida (posicion.col, tope));
}

/*Pre-condiciones: 
    "tipo_defensor" debe ser ELFO o ENANO;
    "cantidad_defensores" debe ser mayor a 0;
    "tamanio_tablero" debe ser mayor a 0.
Post-condiciones: 
    la función modificara los valores de las posiciones recibidas.*/
void ingresar_defensores(juego_t *juego, char tipo_defensor, char* nombre_tipo_defensor, int cantidad_defensores, int tamanio_tablero, char *color){
	int cont = 0;
	coordenada_t posicion_a_cambiar;

	for (int i = 0; i < cantidad_defensores; i++){
	
		do{

			if(cont > 0)
				printf(ROJO "Ingreso incorrecto, vuelva a intentarlo siguiendo las instrucciones anteriormente mencionadas\n" FIN);
			printf("Defensor %s%s "FIN"(%i/%i): \n", color, nombre_tipo_defensor, i+1, cantidad_defensores);
			pedir_coordenadas(&posicion_a_cambiar);
			cont++;
		} while( ( rango_invalido(tamanio_tablero, posicion_a_cambiar) )
			|| ( agregar_defensor(&(juego->nivel), posicion_a_cambiar, tipo_defensor) == NO_POSIBLE ) );
		system("clear");
		mostrar_juego(*juego);
		cont = 0;
	}	
}

/*Pre-condiciones: 
    "juego.nivel_actual" debe ser 1, 2, 3 o 4.
    "tamanio_tablero" positivo.
Post-condiciones: 
    la función modificara los valores de las posiciones recibidas.*/
void cargar_defensores(juego_t *juego, int tamanio_tablero){
	int cantidad_elfos, cantidad_enanos;

	cantidad_elfos = cant_elfos(juego->nivel_actual);
	cantidad_enanos = cant_enanos(juego->nivel_actual);

	mostrar_juego(*juego);

	printf("\n Ahora vamos a colocar los defensores del nivel. Le recuerdo que los valores deben ser válidos y no pueden haber defensores donde hay camino u otros defensores. \n");
	ingresar_defensores(juego, ELFO, "ELFO", cantidad_elfos, tamanio_tablero, AZUL);
	ingresar_defensores(juego, ENANO, "ENANO", cantidad_enanos, tamanio_tablero, MAGENTA);

	printf(VERDE"Ingreso finalizado exitosamente\n"FIN);
	detener_el_tiempo(2);
	system("clear");
}

/*Pre-condiciones: 
    -
Post-condiciones: 
    la función modifica todos los valores de las posiciones recibidas por VACIO*/
void limpiar_camino(coordenada_t vector[MAX_LONGITUD_CAMINO]){
	
	for (int i = 0; i < MAX_LONGITUD_CAMINO; ++i){
	
		vector[i].fil = VACIO;
		vector[i].col = VACIO;
	}
}

/*Pre-condiciones: 
    "nivel" debe ser 1, 2, 3 o 4.
Post-condiciones: 
    la función retorna un ascii art segun "nivel".*/
void mostrar_mensaje_nivel_actual (int nivel){
	
	system("clear");
	printf(VERDE " \n \n \n \n");
	switch(nivel){
		case 1:
			printf(ESPACIO"						 ███╗   ██╗██╗██╗   ██╗███████╗██╗          ██╗\n");
			printf(ESPACIO"						 ████╗  ██║██║██║   ██║██╔════╝██║         ███║\n");
			printf(ESPACIO"						 ██╔██╗ ██║██║██║   ██║█████╗  ██║         ╚██║\n");
			printf(ESPACIO"						 ██║╚██╗██║██║╚██╗ ██╔╝██╔══╝  ██║          ██║\n");
			printf(ESPACIO"						 ██║ ╚████║██║ ╚████╔╝ ███████╗███████╗     ██║\n");
			printf(ESPACIO"						 ╚═╝  ╚═══╝╚═╝  ╚═══╝  ╚══════╝╚══════╝     ╚═╝\n");
			break;
		case 2:
			printf(ESPACIO"						 ███╗   ██╗██╗██╗   ██╗███████╗██╗        ,██████╗\n");
			printf(ESPACIO"						 ████╗  ██║██║██║   ██║██╔════╝██║        ╚════██╗\n");
			printf(ESPACIO"						 ██╔██╗ ██║██║██║   ██║█████╗  ██║         █████╔╝\n");
			printf(ESPACIO"						 ██║╚██╗██║██║╚██╗ ██╔╝██╔══╝  ██║        ██╔═══╝\n");
			printf(ESPACIO"						 ██║ ╚████║██║ ╚████╔╝ ███████╗███████╗   ███████╗\n");
			printf(ESPACIO"						 ╚═╝  ╚═══╝╚═╝  ╚═══╝  ╚══════╝╚══════╝   ╚══════╝\n");
			break;
		case 3:
			printf(ESPACIO"						 ███╗   ██╗██╗██╗   ██╗███████╗██╗         ██████╗ \n");
			printf(ESPACIO"						 ████╗  ██║██║██║   ██║██╔════╝██║         ╚═══██║\n");
			printf(ESPACIO"						 ██╔██╗ ██║██║██║   ██║█████╗  ██║          █████║\n");
			printf(ESPACIO"						 ██║╚██╗██║██║╚██╗ ██╔╝██╔══╝  ██║          ╚══██║\n");
			printf(ESPACIO"						 ██║ ╚████║██║ ╚████╔╝ ███████╗███████╗   ███████║\n");
			printf(ESPACIO"						 ╚═╝  ╚═══╝╚═╝  ╚═══╝  ╚══════╝╚══════╝   ╚══════╝ \n");
			break;
		case 4:
			printf(ESPACIO"						 ███╗   ██╗██╗██╗   ██╗███████╗██╗       ██╗  ██╗\n");
			printf(ESPACIO"						 ████╗  ██║██║██║   ██║██╔════╝██║       ██║  ██║\n");
			printf(ESPACIO"						 ██╔██╗ ██║██║██║   ██║█████╗  ██║       ███████║\n");
			printf(ESPACIO"						 ██║╚██╗██║██║╚██╗ ██╔╝██╔══╝  ██║       ╚════██║\n");
			printf(ESPACIO"						 ██║ ╚████║██║ ╚████╔╝ ███████╗███████╗       ██║\n");
			printf(ESPACIO"						 ╚═╝  ╚═══╝╚═╝  ╚═══╝  ╚══════╝╚══════╝       ╚═╝\n");
			break;
	}
	printf(FIN);
	detener_el_tiempo(2);
    fflush(stdout);
    system("clear");
}

/*Pre-condiciones: 
    -
Post-condiciones: 
    la función modificará los valores de las posiciones recibidas con VACIO y con los valores ingresados por el usuario.*/
void inicializar_nivel(juego_t *juego){
	int tamanio_tablero;

	tamanio_tablero = tamanio_tablero_segun_nivel(juego->nivel_actual);
	
	juego->nivel.tope_camino_1 = VACIO;
	juego->nivel.tope_camino_2 = VACIO;
	juego->nivel.tope_defensores = VACIO;
	juego->nivel.tope_enemigos = VACIO;
	limpiar_camino(juego->nivel.camino_1);
	limpiar_camino(juego->nivel.camino_2);
	juego->nivel.max_enemigos_nivel = enemigos_nivel_actual(juego->nivel_actual);
    	
	obtener_camino_segun_nivel(&(juego->nivel), juego->nivel_actual, tamanio_tablero);
	cargar_defensores(juego, tamanio_tablero);
}

/*Pre-condiciones: -.
Post-condiciones:
	la función convierte en mayúscula "letra" si el mismo no lo estaba; si "letra" ya estaba en mayúscula, se devolverá el mismo valor*/
void mayusculizar_letra(char *letra){

    *letra = (char) toupper (*letra);
}

/*Pre-condiciones: 
    "nivel" debe ser 1, 2, 3 o 4.
Post-condiciones: 
    la función retornará ENEMIGOS_DEFENSOR_EXTRA_NIVEL_1, ENEMIGOS_DEFENSOR_EXTRA_NIVEL_2, ENEMIGOS_DEFENSOR_EXTRA_NIVEL_3 o ENEMIGOS_DEFENSOR_EXTRA_NIVEL_4.*/
int enemigos_para_defensor_extra(int nivel){
	int enemigos_necesarios;
	
	switch (nivel){
		case 1:
			enemigos_necesarios = ENEMIGOS_DEFENSOR_EXTRA_NIVEL_1; 
			break;
		case 2:
			enemigos_necesarios = ENEMIGOS_DEFENSOR_EXTRA_NIVEL_2;
			break;
		case 3:
			enemigos_necesarios = ENEMIGOS_DEFENSOR_EXTRA_NIVEL_3;
			break;
		case 4:
			enemigos_necesarios = ENEMIGOS_DEFENSOR_EXTRA_NIVEL_4;
			break;
	}

	return enemigos_necesarios;
}

/*Pre-condiciones: 
    "nivel" debe ser 1, 2, 3 o 4.
Post-condiciones: 
    la función retornará ENEMIGOS_NIVEL_1/ENEMIGOS_DEFENSOR_EXTRA_NIVEL_1, ENEMIGOS_NIVEL_2/ENEMIGOS_DEFENSOR_EXTRA_NIVEL_2, ENEMIGOS_NIVEL_3/ENEMIGOS_DEFENSOR_EXTRA_NIVEL_3
    o ENEMIGOS_NIVEL_4/ENEMIGOS_DEFENSOR_EXTRA_NIVEL_4.*/
int cant_def_extra_segun_nivel(int nivel){
	int cantidad;

	switch(nivel){
		case 1:
			cantidad = ENEMIGOS_NIVEL_1/ENEMIGOS_DEFENSOR_EXTRA_NIVEL_1;
			break;
		case 2:
			cantidad = ENEMIGOS_NIVEL_2/ENEMIGOS_DEFENSOR_EXTRA_NIVEL_2;
			break;
		case 3:
			cantidad = ENEMIGOS_NIVEL_3/ENEMIGOS_DEFENSOR_EXTRA_NIVEL_3;
			break;
		case 4:
			cantidad = ENEMIGOS_NIVEL_4/ENEMIGOS_DEFENSOR_EXTRA_NIVEL_4;
			break;
	}

	return cantidad;
}

/*Pre-condiciones: 
    -
Post-condiciones: 
    la función devolverá VERDADERO si la respuesta del usuario es SI*/
bool querer_agregar_def_extra(){
	bool querer;
	char respuesta;

	printf("Tiene la posibilidad de agregar defensores extra a cambio de puntos de vida de su torre. ¿Desea hacerlo? ('"SI"' para sí, '"NO"' para no) \n");
	scanf(" %c", &respuesta);
	
	mayusculizar_letra(&respuesta);

	if (respuesta == 'S')
		querer = true;
	else
		querer = false;

	return querer;
}

/*Pre-condiciones:
	-
Post-condiciones:
	la función retornará VERDADERO si el primer parámetro ("letra") no es igual a ninguno de los dos valores a comparar.*/
bool ingreso_letra_no_es_correcto_2(char letra, char comparacion_1,  char comparacion_2){

    return ( (letra != comparacion_1) && (letra != comparacion_2) );
}

/*Pre-condiciones: 
    "nivel_actual" debe ser 1, 2, 3 o 4.
Post-condiciones: 
    la función retornará VERDADERO o no según nivel_actual*/
bool se_puede_agregar_elfo_segun_nivel(int nivel_actual){
	bool se_puede;
	
	switch (nivel_actual){
		case 1:
			se_puede =  PODER_AGREGAR_ELFO_NIVEL_1;
			break;
		case 2:
			se_puede =  PODER_AGREGAR_ELFO_NIVEL_2;
			break;
		case 3:
			se_puede =  PODER_AGREGAR_ELFO_NIVEL_3;
			break;
		case 4:
			se_puede =  PODER_AGREGAR_ELFO_NIVEL_4;
			break;
	}

	return se_puede;
}

/*Pre-condiciones: 
    "nivel_actual" debe ser 1, 2, 3 o 4.
Post-condiciones: 
    la función retornará VERDADERO o no según nivel_actual*/
bool se_puede_agregar_enano_segun_nivel(int nivel_actual){
	bool se_puede;
	
	switch (nivel_actual){
		case 1:
			se_puede =  PODER_AGREGAR_ENANO_NIVEL_1;
			break;
		case 2:
			se_puede =  PODER_AGREGAR_ENANO_NIVEL_2;
			break;
		case 3:
			se_puede =  PODER_AGREGAR_ENANO_NIVEL_3;
			break;
		case 4:
			se_puede =  PODER_AGREGAR_ENANO_NIVEL_4;
			break;
	}

	return se_puede;
}

/*Pre-condiciones: 
    "torres->resistencia_torre_1" y "torres->resistencia_torre_2" mayores a 0;
    "defensor" es ENANO o ELFO.
Post-condiciones: 
    la función modificará el valor en la posición torres->resistencia_torre_1 o torres->resistencia_torre_2 según el defensor.*/
void restar_vida_torre_del_defensor(torres_t *torres, char defensor){
	
	if (defensor == ENANO)
		torres->resistencia_torre_1 -= COSTO_DEF_EXTRA_TORRE_1;
	else
		torres->resistencia_torre_2 -= COSTO_DEF_EXTRA_TORRE_2;
}

/*Pre-condiciones: 
    "torres->enanos_extra" y "torres->elfos_extra" mayores o iguales a 0;
    "defensor" es ENANO o ELFO.
Post-condiciones: 
    la función modificará torres->enanos_extra o torres->elfos_extra según el defensor.*/
void restar_defensor_de_def_extras(torres_t *torres, char defensor){

	if (defensor == ENANO)
		(torres->enanos_extra)--;
	else
		(torres->elfos_extra)--;
}

/*Pre-condiciones:
	"nivel_actual" debe ser 1, 2, 3 o 4;
	"enanos_extra" y "elfos_extra" >= 0 y menores a ELFOS_EXTRA_INICIAL y ENANOS_EXTRA_INCIAL;
	"vida_torre_1" y "vida_torre_2" > 0 y menores a VIDA_TORRE_1_INICIAL y VIDA_TORRE_2_INICIAL respectivamente.
Post-condiciones:
	la función modificará el valor de la posición recibida por ELFO o ENANO.*/
void determinar_tipo_de_def_a_agregar(char *tipo, int nivel_actual, int elfos_extra, int enanos_extra, int vida_torre_1, int vida_torre_2){
	char defensor;
	int cont = 0;
	
	if( se_puede_agregar_elfo_segun_nivel(nivel_actual) && (elfos_extra > 0) && (vida_torre_1 > COSTO_DEF_EXTRA_TORRE_1)
		&& se_puede_agregar_enano_segun_nivel(nivel_actual) && (enanos_extra > 0) && (vida_torre_2 > COSTO_DEF_EXTRA_TORRE_2) ){

		do{
			if (cont > 0)
				printf(ROJO"Ingreso inválido\n" FIN);
			printf("¿Desea agregar un " AZUL"ELFO"FIN" (%c) o un " MAGENTA"ENANO "FIN"(%c)?\n" FIN, ELFO, ENANO);
			scanf(" %c", &defensor);
			mayusculizar_letra(&defensor);
        	cont++;
		} while (ingreso_letra_no_es_correcto_2(defensor, ENANO, ELFO));
		*tipo = defensor;
	} else if( se_puede_agregar_elfo_segun_nivel(nivel_actual) && (elfos_extra > 0) && (vida_torre_2 > COSTO_DEF_EXTRA_TORRE_2) ){
		
		printf("Defensor" AZUL " ELFO \n" FIN);
		*tipo = ELFO;
	} else{
		
		printf("Defensor" MAGENTA " ENANO \n" FIN);
		*tipo = ENANO;
	}
}

/*Pre-condiciones: 
	"juego" estructura con valores válidos;
		"juego->nivel_actual" debe ser 1, 2, 3 o 4;
		"juego->torres.enanos_extra" y "juego->torres.elfos_extra" >= 0 y menores a ELFOS_EXTRA_INICIAL y ENANOS_EXTRA_INCIAL;
		"juego->torres.resistencia_torre_1" y "juego->torres.resistencia_torre_2" > 0 y menores a VIDA_TORRE_1_INICIAL y VIDA_TORRE_2_INICIAL respectivamente.
Post-condiciones:
	la función modificará los valores en "juego" según los valores ingresados por el usuario.*/
void agregar_def_extra(juego_t *juego){
	char tipo;
	coordenada_t posicion_a_cambiar;
	int cont = 0;
	int tamanio_tablero = tamanio_tablero_segun_nivel(juego->nivel_actual);

	determinar_tipo_de_def_a_agregar(&tipo, juego->nivel_actual, juego->torres.elfos_extra, juego->torres.enanos_extra,
		juego->torres.resistencia_torre_1, juego->torres.resistencia_torre_2);
	
	do{
		if(cont > 0)
			printf(ROJO "Ingreso incorrecto, vuelva a intentarlo siguiendo las instrucciones anteriormente mencionadas\n" FIN);
		pedir_coordenadas(&posicion_a_cambiar);
		cont++;
	} while (rango_invalido(tamanio_tablero, posicion_a_cambiar) || agregar_defensor(&(juego->nivel), posicion_a_cambiar, tipo) == NO_POSIBLE);
	restar_vida_torre_del_defensor(&(juego->torres), tipo);
	restar_defensor_de_def_extras(&(juego->torres), tipo);
}

/*Pre-condiciones: 
	"nivel" es 1, 2, 3 o 4.
	"elfos_extra" y "enanos_extra" >= 0, y menores a ELFOS_EXTRA_INICIAL y ENANOS_EXTRA_INICIAL respectivamente.
Post-condiciones:
	la función devolverá VERDADERO si hay defensores según el nivel*/
bool hay_defensores_disponibles_seg_nivel(int nivel, int elfos_extra, int enanos_extra){
	bool hay;

	switch (nivel){
		case 1:
			hay = enanos_extra > 0;
			break;
		case 2:
			hay = elfos_extra > 0;
			break;
		case 3:
			hay = enanos_extra > 0 || elfos_extra > 0;
			break;
		case 4:
			hay = enanos_extra > 0 || elfos_extra > 0;
			break;
	}

	return hay;
}

/*Pre-condiciones: 
	"nivel" es 1, 2, 3 o 4.
	"resistencia_torre_1" y "resistencia_torre_2" >= 0, y menores a VIDA_INICIAL_TORRE_1 y VIDA_INICIAL_TORRE_2 respectivamente.
Post-condiciones:
	la función devolverá VERDADERO según los parámetros recibidos*/
bool torres_con_suficiente_vida_seg_nivel(int nivel, int resistencia_torre_1, int resistencia_torre_2){
	bool hay;

	switch (nivel){
		case 1:
			hay = resistencia_torre_1 > COSTO_DEF_EXTRA_TORRE_1;
			break;
		case 2:
			hay = resistencia_torre_2 > COSTO_DEF_EXTRA_TORRE_2;
			break;
		case 3:
			hay = resistencia_torre_2 > COSTO_DEF_EXTRA_TORRE_2 || resistencia_torre_1 > COSTO_DEF_EXTRA_TORRE_1;
			break;
		case 4:
			hay = resistencia_torre_2 > COSTO_DEF_EXTRA_TORRE_2 || resistencia_torre_1 > COSTO_DEF_EXTRA_TORRE_1;
			break;
	}

	return hay;
}

/*Pre-condiciones: 
	-	
Post-condiciones:
	la función mostrará un ascii art*/
void mostrar_mensaje_felicitaciones (){
	system("clear");

	printf(VERDE ESPACIO"\n \n\n \n");
    printf(VERDE ESPACIO"          ███████╗ ███████╗ ██╗      ██╗  ██████╗ ██╗ ████████╗  █████╗   ██████╗ ██╗  ██████╗  ███╗   ██╗ ███████╗ ███████╗ \n");
	printf(      ESPACIO"          ██╔════╝ ██╔════╝ ██║      ██║ ██╔════╝ ██║ ╚══██╔══╝ ██╔══██╗ ██╔════╝ ██║ ██╔═══██╗ ████╗  ██║ ██╔════╝ ██╔════╝ \n");
	printf(      ESPACIO"          █████╗   █████╗   ██║      ██║ ██║      ██║    ██║    ███████║ ██║      ██║ ██║   ██║ ██╔██╗ ██║ █████╗   ███████╗ \n");
	printf(      ESPACIO"          ██╔══╝   ██╔══╝   ██║      ██║ ██║      ██║    ██║    ██╔══██║ ██║      ██║ ██║   ██║ ██║╚██╗██║ ██╔══╝   ╚════██║ \n");
	printf(      ESPACIO"          ██║      ███████╗ ███████╗ ██║ ╚██████╗ ██║    ██║    ██║  ██║ ╚██████╗ ██║ ╚██████╔╝ ██║ ╚████║ ███████╗ ███████║ \n");
	printf(      ESPACIO"          ╚═╝      ╚══════╝ ╚══════╝ ╚═╝  ╚═════╝ ╚═╝    ╚═╝    ╚═╝  ╚═╝  ╚═════╝ ╚═╝  ╚═════╝  ╚═╝  ╚═══╝ ╚══════╝ ╚══════╝ \n");
    printf(FIN);
    
    detener_el_tiempo(2);
    fflush(stdout);
    system("clear");
}

/*Pre-condiciones: 
	-	
Post-condiciones:
	la función mostrará un ascii art*/
void mostrar_mensaje_juego_perdido(int nivel){
	system("clear");
    printf(VERDE "\n\n");
    printf(      ESPACIO"                                   ██████╗ ███████╗██████╗ ██████╗ ██╗███████╗████████╗███████╗ \n");
	printf(      ESPACIO"                                   ██╔══██╗██╔════╝██╔══██╗██╔══██╗██║██╔════╝╚══██╔══╝██╔════╝ \n");
	printf(      ESPACIO"                                   ██████╔╝█████╗  ██████╔╝██║  ██║██║███████╗   ██║   █████╗   \n");
	printf(      ESPACIO"                                   ██╔═══╝ ██╔══╝  ██╔══██╗██║  ██║██║╚════██║   ██║   ██╔══╝   \n");
	printf(      ESPACIO"                                   ██║     ███████╗██║  ██║██████╔╝██║███████║   ██║   ███████╗ \n");
	printf(      ESPACIO"                                   ╚═╝     ╚══════╝╚═╝  ╚═╝╚═════╝ ╚═╝╚══════╝   ╚═╝   ╚══════╝ \n");
    
    printf(FIN);
    detener_el_tiempo(2);
    fflush(stdout);

    system("clear");
}

/*Pre-condiciones: 
	-	
Post-condiciones:
	la función mostrará un ascii art*/
void mostrar_mensaje_juego_ganado(){
	system("clear");

	printf(VERDE ESPACIO"\n \n");
	printf(      ESPACIO"                                  ██╗    ██╗ ██████╗ ██╗    ██╗ \n");
	printf(      ESPACIO"                                  ██║    ██║██╔═══██╗██║    ██║ \n");
	printf(      ESPACIO"                                  ██║ █╗ ██║██║   ██║██║ █╗ ██║ \n");
	printf(      ESPACIO"                                  ██║███╗██║██║   ██║██║███╗██║ \n");
	printf(      ESPACIO"                                  ╚███╔███╔╝╚██████╔╝╚███╔███╔╝ \n");
	printf(      ESPACIO"                                  ╚══╝╚══╝  ╚═════╝  ╚══╝╚══╝  \n");
	printf(      ESPACIO"\n");
    printf(      ESPACIO"	███████╗ ███████╗ ██╗      ██╗  ██████╗ ██╗ ████████╗  █████╗   ██████╗ ██╗  ██████╗  ███╗   ██╗ ███████╗ ███████╗ \n");
	printf(      ESPACIO"	██╔════╝ ██╔════╝ ██║      ██║ ██╔════╝ ██║ ╚══██╔══╝ ██╔══██╗ ██╔════╝ ██║ ██╔═══██╗ ████╗  ██║ ██╔════╝ ██╔════╝ \n");
	printf(      ESPACIO"	█████╗   █████╗   ██║      ██║ ██║      ██║    ██║    ███████║ ██║      ██║ ██║   ██║ ██╔██╗ ██║ █████╗   ███████╗ \n");
	printf(      ESPACIO"	██╔══╝   ██╔══╝   ██║      ██║ ██║      ██║    ██║    ██╔══██║ ██║      ██║ ██║   ██║ ██║╚██╗██║ ██╔══╝   ╚════██║ \n");
	printf(      ESPACIO"	██║      ███████╗ ███████╗ ██║ ╚██████╗ ██║    ██║    ██║  ██║ ╚██████╗ ██║ ╚██████╔╝ ██║ ╚████║ ███████╗ ███████║ \n");
	printf(      ESPACIO"	╚═╝      ╚══════╝ ╚══════╝ ╚═╝  ╚═════╝ ╚═╝    ╚═╝    ╚═╝  ╚═╝  ╚═════╝ ╚═╝  ╚═════╝  ╚═╝  ╚═══╝ ╚══════╝ ╚══════╝ \n");
	printf(      ESPACIO"\n");
	printf(      ESPACIO"                    ██████╗  █████╗ ███╗   ██╗ █████╗ ███████╗████████╗███████╗ \n");
	printf(      ESPACIO"                   ██╔════╝ ██╔══██╗████╗  ██║██╔══██╗██╔════╝╚══██╔══╝██╔════╝ \n");
	printf(      ESPACIO"                   ██║  ███╗███████║██╔██╗ ██║███████║███████╗   ██║   █████╗   \n");
	printf(      ESPACIO"                   ██║   ██║██╔══██║██║╚██╗██║██╔══██║╚════██║   ██║   ██╔══╝   \n");
	printf(      ESPACIO"                   ╚██████╔╝██║  ██║██║ ╚████║██║  ██║███████║   ██║   ███████╗ \n");
	printf(      ESPACIO"                    ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝   ╚═╝   ╚══════╝ \n");
	printf(FIN);
    
    detener_el_tiempo(2);
    fflush(stdout);
    system("clear");
}

/*Pre-condiciones:
	"cant_enemigos_def_extra" debe ser != 0.
Post-condiciones:
	la función modificará el valor de "juego" si es posible y el usuario así quiere.*/
void agregar_defensores_si_es_posible(juego_t *juego, int *cant_defensores_agregar_total, int cant_enemigos_def_extra){
	
	if ( ((juego->nivel.tope_enemigos)%(cant_enemigos_def_extra) == 0) && (*cant_defensores_agregar_total != 0) ) {
			
	    (*cant_defensores_agregar_total)--;
	    if (hay_defensores_disponibles_seg_nivel(juego->nivel_actual, juego->torres.elfos_extra, juego ->torres.enanos_extra)
	    && torres_con_suficiente_vida_seg_nivel(juego->nivel_actual, juego->torres.resistencia_torre_1, juego->torres.resistencia_torre_2)
	    && querer_agregar_def_extra())
	    	agregar_def_extra(juego);
	}
}	    

/*Pre-condiciones: 
	"juego" debe ser una estructura con valores válidos:
		"juego.nivel_actual" debe ser 1, 2, 3 o 4.
		"juego.max_enemigos_nivel" positivo y < MAX_ENEMIGOS.
		"juego->fallo_gimli", "juego->critico_gimli", "juego->fallo_legolas", "juego->critico_legolas" deben ser >= 0 y <= 100;
		"juego->torres" tiene que tener la resistencia de ambas torres > 0.
Post-condiciones:
	la función modificará "juego" según las jugadas realizadas (depende del azar mayormente) y mostrará información de cada turno jugado*/
void jugar_nivel(juego_t *juego){
	int cant_enemigos_def_extra, cant_defensores_agregar_total;

	cant_enemigos_def_extra	= enemigos_para_defensor_extra(juego->nivel_actual);
    cant_defensores_agregar_total = cant_def_extra_segun_nivel(juego->nivel_actual);
		
	while( ( estado_nivel(juego->nivel) == JUGANDO ) && ( estado_juego(*juego) == JUGANDO ) ){
    		
    		system("clear");
    		jugar_turno(juego);
    		mostrar_juego(*juego);
	    	detener_el_tiempo(TIEMPO_ENTRE_TURNOS);
			agregar_defensores_si_es_posible(juego, &cant_defensores_agregar_total, cant_enemigos_def_extra);
    	}

    detener_el_tiempo(2);
}

/*Pre-condiciones: 
	-.
Post-condiciones:
	la función mostrará un punto tras otro, formando una barra de progreso.*/
void mostrar_barra_progreso(){

	for (int i = 0; i < 15; i++){
        
        detener_el_tiempo(0.50);
        fflush(stdout);
        printf("█");
    }
}

/*Pre-condiciones: 
	-.
Post-condiciones:
	la función mostrará la carga del TP2.*/
void mostrar_carga_inicio_TP2(){
	
	system("clear");
	printf("Cargando juego a partir de los datos obtenidos...\n" VERDE);
	mostrar_barra_progreso();
	printf(FIN "\nFinalizado" LINEA);
    detener_el_tiempo(1);
}

/*Pre-condiciones: 
	-.
Post-condiciones:
	la función mostrará las instrucciones del juego.*/
void cargar_instrucciones(){

	system("clear");
	printf("\n \n");
	printf(VERDE ESPACIO"                ___\n");
	printf(ESPACIO "               / | \\  		                                                ______________________       \n");
	printf(ESPACIO "              /  |  \\ 		    ___________________-------------------------                      `\\     \n");
	printf(ESPACIO "             |   |   |		   /:====                                                               \\      \n");
	printf(ESPACIO "             |   |   |		   ||< > |                                         _____________________/     \n");
	printf(ESPACIO "             |   |   |		   | \\__/_________________-------------------------                   |	\n");
	printf(ESPACIO "             |   |   |		   |                                                                  |	\n");
	printf(ESPACIO "             |   |   |	  	   |                           "BLANCO"INSTRUCCIONES"VERDE"                          |	\n");
	printf(ESPACIO "             |   |   |	           |                                                                  | \n");
	printf(ESPACIO "             |   |   |		   |                                                                  | \n");
	printf(ESPACIO"             |   |   |		   |      "BLANCO"El juego consta de 4 niveles. En cada nivel tendrá que"VERDE"      | \n");
	printf(ESPACIO"             |   |   |		   \\	   "BLANCO"vencer a los orcos, mas no es tan facil como suena."VERDE"        \\   \n");
	printf(ESPACIO"             |   |   |		    |                                                                  | \n"); 
	printf(ESPACIO"             |   |   |		    |      "BLANCO"Podrá defenderse con enanos (liderados por Gimli), los"VERDE"      | \n");
	printf(ESPACIO"             |   |   |  	    \\       "BLANCO"cuales atacan a un orco por turno, siempre y cuando"VERDE"        \\  \n");
	printf(ESPACIO"             |  _|_  |		     |        "BLANCO"se encuentre a un casillero de distancia. También"VERDE"         |  \n");
	printf(ESPACIO"             | /   \\ |               |          "BLANCO"contará con elfos (liderados por Legolas), los cuales"VERDE"   | \n");
	printf(ESPACIO"/\\           |/     \\|          /\\    \\       "BLANCO"atacan a todos los orcos a distancia 3 manhattan."VERDE"         \\ \n");
	printf(ESPACIO"\\ \\__________/       \\_________/ /     |                                                                 | \n");
	printf(ESPACIO" \\___________\\       /__________/      |      "BLANCO"Recuerde que dispondrá de defensores extras, que podrá "VERDE"    | \n");
	printf(ESPACIO"              \\     /                  \\       "BLANCO"colocar a cambio de vida de sus torres."VERDE"                    \\ \n");
	printf(ESPACIO"              |\\   /|                   |       "BLANCO"Costo "DEF_EXTRA_TORRE_1": %i ptos de la TORRE 1" VERDE"                         | \n", COSTO_DEF_EXTRA_TORRE_1);
	printf(ESPACIO"              |/\\ //|                   |       "BLANCO"Costo "DEF_EXTRA_TORRE_2": %i ptos de la TORRE 2" VERDE "                          | \n", COSTO_DEF_EXTRA_TORRE_2);
	printf(ESPACIO"              |/////|                   |                                                                  | \n");
	printf(ESPACIO"              |/////|                   |		         "BLANCO"Mucha suerte y que el anillo lo acompañe."VERDE" |\n");
	printf(ESPACIO"              |/////|                   |                                               ___________________|__  \n");
	printf(ESPACIO"              |/////|                   |  ___________________--------------------------                     `\\ \n");
	printf(ESPACIO"              |/////|                   |/`--_                                                                 | \n");
	printf(ESPACIO"              |_///_|                   ||[ ]||                                            ___________________/   \n");
	printf(ESPACIO"               \\___/                     \\===/___________________-------------------------- \n" FIN);
	printf(BLANCO"\n Presione enter para continuar\n " FIN);
	char bla;
	scanf("%c", &bla);
}

/*Pre-condiciones: 
	-.
Post-condiciones:
	la función mostrará varios dibujos en secuencia, formando una animación.*/
void animacion(){
	
	for (int i = 0; i < 3; i++){
		
		system("clear");
		printf(VERDE);
		printf("\n\n                                                               . .:.:.:.:. .:\\     /:. .:.:.:.:. ,                   \n");
		printf("                                                          .-._  `..:.:. . .:.:`- -':.:. . .:.:.,'  _.-.                  \n");
		printf("                                                         .:.:.`-._`-._..-''_...---..._``-.._.-'_.-'.:.:.                 \n");
		printf("                                                      .:.:. . .:_.`' _..-''._________,``-.._ `.._:. . .:.:.              \n");
		printf("                                                   .:.:. . . ,-'_.-''      ||_-(O)-_||      ``-._`-. . . .:.:.           \n");
		printf("                                                  .:. . . .,'_.'           '---------'           `._`.. . . .:.          \n");
		printf("                                                :.:. . . ,','               _________               `.`. . . .:.:        \n");
		printf("                                               `.:.:. .,','            _.-''_________``-._            `._.     _.'       \n");
		printf("                                             -._  `._./ /            ,'_.-'' ,       ``-._`.          ,' '`:..'  _.-     \n");
		printf("                                            .:.:`-.._' /           ,','                   `.`.       /'  '  \\.-':.:.    \n");
		printf("                                            :.:. . ./ /          ,','    DEFENDIENDO LAS    `.`.    / '  '  '\\. .:.:    \n");
		printf("                                           :.:. . ./ /          / /    ,                      \\ \\  :  '  '  ' \\. .:.:   \n");
		printf(""BLANCO"..................."FIN VERDE"                        .:. . ./ /          / /           TORRES      ,     \\ \\ :  '  '  ' '::. .:.    "BLANCO"     .................\n");
		printf("  ................."FIN VERDE"                        :. . .: :    o     / /                               \\ ;'  '  '  ' ':: . .:     "BLANCO"     ...............\n");
		printf("    ..............."FIN VERDE"                        .:. . | |   /_\\   : :     ,                      ,    : '  '  '  ' ' :: .:.    "BLANCO"      ...............\n");
		printf("       ............"FIN VERDE"                        :. . .| |  ((<))  | |,          ,       ,   "BLANCO"By"FIN VERDE"        |\\'__',-._.' ' ||. .:      "BLANCO"      ...........\n");
		printf("         .........."FIN VERDE"                        .:.:. | |   `-'   | |---....____             "BLANCO"Ana G."FIN VERDE"   | ,---\\/--/  ' ||:.:.   "BLANCO"          .........\n");
		printf("           ........"FIN VERDE"                        ------| |         : :    ,.     ```--..._   , "BLANCO"Gutson"FIN VERDE"  |''  '  '  ' ' ||----      "BLANCO"       .........\n");
		printf("            ......."FIN VERDE"                        _...--. |  ,       \\ \\             ,.    `-._     ,  /: '  '  '  ' ' ;;..._              "BLANCO".......\n");
		printf("             ......"FIN VERDE"                        :.:. .| | -O-       \\ \\    ,.                `._    / /:'  '  '  ' ':: .:.:               "BLANCO"..... \n");
		printf("              ....."FIN VERDE"                        .:. . | |_(`__       \\ \\                        `. / / :'  '  '  ' ';;. .:.               "BLANCO"..... \n");
		printf("               ...."FIN VERDE"                        :. . .<' (_)  `>      `.`.          ,.    ,.     ,','   \\  '  '  ' ;;. . .:                "BLANCO"... \n");
		printf("                ..."FIN VERDE"                        .:. . |):-.--'(         `.`-._  ,.           _,-','      \\ '  '  '//| . .:.                "BLANCO".. \n");
		printf("                 .."FIN VERDE"                       :. . .;)()(__)(___________`-._`-.._______..-'_.-'___________\'  '  //_:. . .:                "BLANCO".."FIN VERDE" \n");
		printf("                _::_                    .:.:,' \\/\\/--\\/--------------------------------------------`._',;'`. `.:.:.                  _::_\n");
		printf("               :    :                   :.,' ,' ,'  ,'  /   /   /   ,-------------------.   \\   \\   \\  `. `.`. `..:                 :    :\n");
		printf("______________:      :______________,' ,'  '   /   /   /   /   //                   \\   \\   \\   \\   \\  ` `.A.GUTSON________________:      :________________ \n");
		detener_el_tiempo(0.125);
		fflush(stdout);

		system("clear");
		printf("\n\n                                                               . .:.:.:.:. .:\\     /:. .:.:.:.:. ,                   \n");
		printf("                                                          .-._  `..:.:. . .:.:`- -':.:. . .:.:.,'  _.-.                  \n");
		printf("                                                         .:.:.`-._`-._..-''_...---..._``-.._.-'_.-'.:.:.                 \n");
		printf("                                                      .:.:. . .:_.`' _..-''._________,``-.._ `.._:. . .:.:.              \n");
		printf("                                                   .:.:. . . ,-'_.-''      ||_-(O)-_||      ``-._`-. . . .:.:.           \n");
		printf("                                                  .:. . . .,'_.'           '---------'           `._`.. . . .:.          \n");
		printf("                                                :.:. . . ,','               _________               `.`. . . .:.:        \n");
		printf("                                               `.:.:. .,','            _.-''_________``-._            `._.     _.'       \n");
		printf("                                             -._  `._./ /            ,'_.-'' ,       ``-._`.          ,' '`:..'  _.-     \n");
		printf("                                            .:.:`-.._' /           ,','                   `.`.       /'  '  \\.-':.:.    \n");
		printf("                                            :.:. . ./ /          ,','    DEFENDIENDO LAS    `.`.    / '  '  '\\. .:.:    \n");
		printf("                                           :.:. . ./ /          / /    ,                      \\ \\  :  '  '  ' \\. .:.:   \n");
		printf("          "BLANCO"................."FIN VERDE"                .:. . ./ /          / /           TORRES      ,     \\ \\ :  '  '  ' '::. .:.               "BLANCO"...........\n");
		printf("           ..............."FIN VERDE"                 :. . .: :    o     / /                               \\ ;'  '  '  ' ':: . .:               "BLANCO"...........\n");
		printf("           ..............."FIN VERDE"                 .:. . | |   /_\\   : :     ,                      ,    : '  '  '  ' ' :: .:.               "BLANCO"...........\n");
		printf("             ..........."FIN VERDE"                   :. . .| |  ((<))  | |,          ,       ,   "BLANCO"By"FIN VERDE"        |\\'__',-._.' ' ||. .:     "BLANCO"          ...........\n");
		printf("              ........."FIN VERDE"                    .:.:. | |   `-'   | |---....____             "BLANCO"Ana G."FIN VERDE"   | ,---\\/--/  ' ||:.:.       "BLANCO"        ...........\n");
		printf("              ........."FIN VERDE"                    ------| |         : :    ,.     ```--..._   , "BLANCO"Gutson"FIN VERDE"  |''  '  '  ' ' ||----         "BLANCO"      ..........\n");
		printf("               ......."FIN VERDE"                     _...--. |  ,       \\ \\             ,.    `-._     ,  /: '  '  '  ' ' ;;..._     "BLANCO"          .........\n");
		printf("                ....."FIN VERDE"                      :.:. .| | -O-       \\ \\    ,.                `._    / /:'  '  '  ' ':: .:.:      "BLANCO"         ....... \n");
		printf("                ....."FIN VERDE"                      .:. . | |_(`__       \\ \\                        `. / / :'  '  '  ' ';;. .:.       "BLANCO"         ..... \n");
		printf("                 ..."FIN VERDE"                       :. . .<' (_)  `>      `.`.          ,.    ,.     ,','   \\  '  '  ' ;;. . .:         "BLANCO"        ... \n");
		printf("                 .."FIN VERDE"                        .:. . |):-.--'(         `.`-._  ,.           _,-','      \\ '  '  '//| . .:.           "BLANCO"      .. \n");
		printf("                 .."FIN VERDE"                       :. . .;)()(__)(___________`-._`-.._______..-'_.-'___________\'  '  //_:. . .:          "BLANCO"      .. "FIN VERDE"\n");
		printf("                _::_                    .:.:,' \\/\\/--\\/--------------------------------------------`._',;'`. `.:.:.                  _::_\n");
		printf("               :    :                   :.,' ,' ,'  ,'  /   /   /   ,-------------------.   \\   \\   \\  `. `.`. `..:                 :    :\n");
		printf("______________:      :______________,' ,'  '   /   /   /   /   //                   \\   \\   \\   \\   \\  ` `.A.GUTSON________________:      :________________ \n");
		detener_el_tiempo(0.125);
		fflush(stdout);

		system("clear");
		printf("\n\n                                                               . .:.:.:.:. .:\\     /:. .:.:.:.:. ,                   \n");
		printf("                                                          .-._  `..:.:. . .:.:`- -':.:. . .:.:.,'  _.-.                  \n");
		printf("                                                         .:.:.`-._`-._..-''_...---..._``-.._.-'_.-'.:.:.                 \n");
		printf("                                                      .:.:. . .:_.`' _..-''._________,``-.._ `.._:. . .:.:.              \n");
		printf("                                                   .:.:. . . ,-'_.-''      ||_-(O)-_||      ``-._`-. . . .:.:.           \n");
		printf("                                                  .:. . . .,'_.'           '---------'           `._`.. . . .:.          \n");
		printf("                                                :.:. . . ,','               _________               `.`. . . .:.:        \n");
		printf("                                               `.:.:. .,','            _.-''_________``-._            `._.     _.'       \n");
		printf("                                             -._  `._./ /            ,'_.-'' ,       ``-._`.          ,' '`:..'  _.-     \n");
		printf("                                            .:.:`-.._' /           ,','                   `.`.       /'  '  \\.-':.:.    \n");
		printf("                                            :.:. . ./ /          ,','    DEFENDIENDO LAS    `.`.    / '  '  '\\. .:.:    \n");
		printf("                                           :.:. . ./ /          / /    ,                      \\ \\  :  '  '  ' \\. .:.:   \n");
		printf("                 "BLANCO"..................."FIN VERDE"       .:. . ./ /          / /           TORRES      ,     \\ \\ :  '  '  ' '::. .:.     "BLANCO"              ...........\n");
		printf("                 ................."FIN VERDE"         :. . .: :    o     / /                               \\ ;'  '  '  ' ':: . .:                "BLANCO"   ...........\n");
		printf("                 ..............."FIN VERDE"           .:. . | |   /_\\   : :     ,                      ,    : '  '  '  ' ' :: .:.           "BLANCO"       ...........\n");
		printf("                 ............"FIN VERDE"              :. . .| |  ((<))  | |,          ,       ,   "BLANCO"By"FIN VERDE"        |\\'__',-._.' ' ||. .:  "BLANCO"                ...........\n");
		printf("                 .........."FIN VERDE"                .:.:. | |   `-'   | |---....____             "BLANCO"Ana G."FIN VERDE"   | ,---\\/--/  ' ||:.:.     "BLANCO"            ...........\n");
		printf("                 ........"FIN VERDE"                  ------| |         : :    ,.     ```--..._   , "BLANCO"Gutson"FIN VERDE"  |''  '  '  ' ' ||----  "BLANCO"              ..........\n");
		printf("                 ......."FIN VERDE"                   _...--. |  ,       \\ \\             ,.    `-._     ,  /: '  '  '  ' ' ;;..._       "BLANCO"         .........\n");
		printf("                 ......"FIN VERDE"                    :.:. .| | -O-       \\ \\    ,.                `._    / /:'  '  '  ' ':: .:.:               "BLANCO"  ....... \n");
		printf("                 ....."FIN VERDE"                     .:. . | |_(`__       \\ \\                        `. / / :'  '  '  ' ';;. .:.           "BLANCO"       .....\n");
		printf("                 ...."FIN VERDE"                      :. . .<' (_)  `>      `.`.          ,.    ,.     ,','   \\  '  '  ' ;;. . .:            "BLANCO"     ...\n");
		printf("                 ..."FIN VERDE"                       .:. . |):-.--'(         `.`-._  ,.           _,-','      \\ '  '  '//| . .:.          "BLANCO"       .. \n");
		printf("                 .."FIN VERDE"                       :. . .;)()(__)(___________`-._`-.._______..-'_.-'___________\'  '  //_:. . .:          "BLANCO"      .. "FIN VERDE"\n");
		printf("                _::_                    .:.:,' \\/\\/--\\/--------------------------------------------`._',;'`. `.:.:.                  _::_\n");
		printf("               :    :                   :.,' ,' ,'  ,'  /   /   /   ,-------------------.   \\   \\   \\  `. `.`. `..:                 :    :\n");
		printf("______________:      :______________,' ,'  '   /   /   /   /   //                   \\   \\   \\   \\   \\  ` `.A.GUTSON________________:      :________________\n");
		detener_el_tiempo(0.125);
		fflush(stdout);

		system("clear");
		printf("\n\n                                                               . .:.:.:.:. .:\\     /:. .:.:.:.:. ,                   \n");
		printf("                                                          .-._  `..:.:. . .:.:`- -':.:. . .:.:.,'  _.-.                  \n");
		printf("                                                         .:.:.`-._`-._..-''_...---..._``-.._.-'_.-'.:.:.                 \n");
		printf("                                                      .:.:. . .:_.`' _..-''._________,``-.._ `.._:. . .:.:.              \n");
		printf("                                                   .:.:. . . ,-'_.-''      ||_-(O)-_||      ``-._`-. . . .:.:.           \n");
		printf("                                                  .:. . . .,'_.'           '---------'           `._`.. . . .:.          \n");
		printf("                                                :.:. . . ,','               _________               `.`. . . .:.:        \n");
		printf("                                               `.:.:. .,','            _.-''_________``-._            `._.     _.'       \n");
		printf("                                             -._  `._./ /            ,'_.-'' ,       ``-._`.          ,' '`:..'  _.-     \n");
		printf("                                            .:.:`-.._' /           ,','                   `.`.       /'  '  \\.-':.:.    \n");
		printf("                                            :.:. . ./ /          ,','    DEFENDIENDO LAS    `.`.    / '  '  '\\. .:.:    \n");
		printf("                                           :.:. . ./ /          / /    ,                      \\ \\  :  '  '  ' \\. .:.:   \n");
		printf("                    "BLANCO"..................."FIN VERDE"    .:. . ./ /          / /           TORRES      ,     \\ \\ :  '  '  ' '::. .:. "BLANCO"             ...........\n");
		printf("                    ................."FIN VERDE"      :. . .: :    o     / /                               \\ ;'  '  '  ' ':: . .:       "BLANCO"       ...........\n");
		printf("                   ..............."FIN VERDE"         .:. . | |   /_\\   : :     ,                      ,    : '  '  '  ' ' :: .:.        "BLANCO"      ...........\n");
		printf("                   ............"FIN VERDE"            :. . .| |  ((<))  | |,          ,       ,   "BLANCO"By"FIN VERDE"        |\\'__',-._.' ' ||. .: "BLANCO"             ...........\n");
		printf("                   .........."FIN VERDE"              .:.:. | |   `-'   | |---....____             "BLANCO"Ana G."FIN VERDE"   | ,---\\/--/  ' ||:.:.  "BLANCO"            ...........\n");
		printf("                  ........"FIN VERDE"                 ------| |         : :    ,.     ```--..._   , "BLANCO"Gutson"FIN VERDE"  |''  '  '  ' ' ||----       "BLANCO"       ..........\n");
		printf("                  ......."FIN VERDE"                  _...--. |  ,       \\ \\             ,.    `-._     ,  /: '  '  '  ' ' ;;..._      "BLANCO"        .........\n");
		printf("                  ......"FIN VERDE"                   :.:. .| | -O-       \\ \\    ,.                `._    / /:'  '  '  ' ':: .:.:"BLANCO"               .......\n");
		printf("                 ....."FIN VERDE"                     .:. . | |_(`__       \\ \\                        `. / / :'  '  '  ' ';;. .:.        "BLANCO"        .....\n");
		printf("                 ....."FIN VERDE"                     :. . .<' (_)  `>      `.`.          ,.    ,.     ,','   \\  '  '  ' ;;. . .:                "BLANCO" ...   \n");
		printf("                 ...."FIN VERDE"                      .:. . |):-.--'(         `.`-._  ,.           _,-','      \\ '  '  '//| . .:.           "BLANCO"      ..   \n");
		printf("                 .."FIN VERDE"                       :. . .;)()(__)(___________`-._`-.._______..-'_.-'___________\'  '  //_:. . .:      "BLANCO"          .."FIN VERDE"\n");
		printf("                _::_                    .:.:,' \\/\\/--\\/--------------------------------------------`._',;'`. `.:.:.                  _::_ \n");
		printf("               :    :                   :.,' ,' ,'  ,'  /   /   /   ,-------------------.   \\   \\   \\  `. `.`. `..:                 :    :\n");
		printf("______________:      :______________,' ,'  '   /   /   /   /   //                   \\   \\   \\   \\   \\  ` `.A.GUTSON________________:      :________________\n");
		detener_el_tiempo(0.125);
		fflush(stdout);

		system("clear");
		printf("\n\n                                                               . .:.:.:.:. .:\\     /:. .:.:.:.:. ,                   \n");
		printf("                                                          .-._  `..:.:. . .:.:`- -':.:. . .:.:.,'  _.-.                  \n");
		printf("                                                         .:.:.`-._`-._..-''_...---..._``-.._.-'_.-'.:.:.                 \n");
		printf("                                                      .:.:. . .:_.`' _..-''._________,``-.._ `.._:. . .:.:.              \n");
		printf("                                                   .:.:. . . ,-'_.-''      ||_-(O)-_||      ``-._`-. . . .:.:.           \n");
		printf("                                                  .:. . . .,'_.'           '---------'           `._`.. . . .:.          \n");
		printf("                                                :.:. . . ,','               _________               `.`. . . .:.:        \n");
		printf("                                               `.:.:. .,','            _.-''_________``-._            `._.     _.'       \n");
		printf("                                             -._  `._./ /            ,'_.-'' ,       ``-._`.          ,' '`:..'  _.-     \n");
		printf("                                            .:.:`-.._' /           ,','                   `.`.       /'  '  \\.-':.:.    \n");
		printf("                                            :.:. . ./ /          ,','    DEFENDIENDO LAS    `.`.    / '  '  '\\. .:.:    \n");
		printf("                                           :.:. . ./ /          / /    ,                      \\ \\  :  '  '  ' \\. .:.:   \n");
		printf("               "BLANCO"...................   "FIN VERDE"      .:. . ./ /          / /           TORRES      ,     \\ \\ :  '  '  ' '::. .:.   "BLANCO"        .............\n");
		printf("               .................    "FIN VERDE"       :. . .: :    o     / /                               \\ ;'  '  '  ' ':: . .:      "BLANCO"     .............\n");
		printf("               ...............     "FIN VERDE"        .:. . | |   /_\\   : :     ,                      ,    : '  '  '  ' ' :: .:.      "BLANCO"     .............\n");
		printf("               ............   "FIN VERDE"             :. . .| |  ((<))  | |,          ,       ,   "BLANCO"By"FIN VERDE"        |\\'__',-._.' ' ||. .:  "BLANCO"         .............\n");
		printf("               ..........   "FIN VERDE"               .:.:. | |   `-'   | |---....____             "BLANCO"Ana G."FIN VERDE"   | ,---\\/--/  ' ||:.:.  "BLANCO"         .............\n");
		printf("                ........    "FIN VERDE"               ------| |         : :    ,.     ```--..._   , "BLANCO"Gutson"FIN VERDE"  |''  '  '  ' ' ||----   "BLANCO"         ...........\n");
		printf("                .......   "FIN VERDE"                 _...--. |  ,       \\ \\             ,.    `-._     ,  /: '  '  '  ' ' ;;..._     "BLANCO"        .........\n");
		printf("                ......   "FIN VERDE"                  :.:. .| | -O-       \\ \\    ,.                `._    / /:'  '  '  ' ':: .:.: "BLANCO"             .......\n");
		printf("                 .....          "FIN VERDE"           .:. . | |_(`__       \\ \\                        `. / / :'  '  '  ' ';;. .:.    "BLANCO"           .....\n");
		printf("                 .....  "FIN VERDE"                   :. . .<' (_)  `>      `.`.          ,.    ,.     ,','   \\  '  '  ' ;;. . .:      "BLANCO"          ...\n");
		printf("                 ...."FIN VERDE"                      .:. . |):-.--'(         `.`-._  ,.           _,-','      \\ '  '  '//| . .:.        "BLANCO"        ..\n");
		printf("                 .."FIN VERDE"                       :. . .;)()(__)(___________`-._`-.._______..-'_.-'___________\'  '  //_:. . .:       "BLANCO"         .."FIN VERDE"\n");
		printf("                _::_                    .:.:,' \\/\\/--\\/--------------------------------------------`._',;'`. `.:.:.                  _::_\n");
		printf("               :    :                   :.,' ,' ,'  ,'  /   /   /   ,-------------------.   \\   \\   \\  `. `.`. `..:                 :    :\n");
		printf("______________:      :______________,' ,'  '   /   /   /   /   //                   \\   \\   \\   \\   \\  ` `.A.GUTSON________________:      :________________\n");
		detener_el_tiempo(0.125);
		fflush(stdout);

		system("clear");
		printf("\n\n                                                               . .:.:.:.:. .:\\     /:. .:.:.:.:. ,                   \n");
		printf("                                                          .-._  `..:.:. . .:.:`- -':.:. . .:.:.,'  _.-.                  \n");
		printf("                                                         .:.:.`-._`-._..-''_...---..._``-.._.-'_.-'.:.:.                 \n");
		printf("                                                      .:.:. . .:_.`' _..-''._________,``-.._ `.._:. . .:.:.              \n");
		printf("                                                   .:.:. . . ,-'_.-''      ||_-(O)-_||      ``-._`-. . . .:.:.           \n");
		printf("                                                  .:. . . .,'_.'           '---------'           `._`.. . . .:.          \n");
		printf("                                                :.:. . . ,','               _________               `.`. . . .:.:        \n");
		printf("                                               `.:.:. .,','            _.-''_________``-._            `._.     _.'       \n");
		printf("                                             -._  `._./ /            ,'_.-'' ,       ``-._`.          ,' '`:..'  _.-     \n");
		printf("                                            .:.:`-.._' /           ,','                   `.`.       /'  '  \\.-':.:.    \n");
		printf("                                            :.:. . ./ /          ,','    DEFENDIENDO LAS    `.`.    / '  '  '\\. .:.:    \n");
		printf("                                           :.:. . ./ /          / /    ,                      \\ \\  :  '  '  ' \\. .:.:   \n");
		printf("           "BLANCO"................ "FIN VERDE"               .:. . ./ /          / /           TORRES      ,     \\ \\ :  '  '  ' '::. .: "BLANCO"..................\n");
		printf("           ..............  "FIN VERDE"                :. . .: :    o     / /                               \\ ;'  '  '  ' ':: . .: "BLANCO".................\n");
		printf("           .............  "FIN VERDE"                 .:. . | |   /_\\   : :     ,                      ,    : '  '  '  ' ' :: .:.   "BLANCO"...............\n");
		printf("           ............  "FIN VERDE"                  :. . .| |  ((<))  | |,          ,       ,   "BLANCO"By"FIN VERDE"        |\\'__',-._.' ' ||. .:  "BLANCO"    ............\n");
		printf("           ..........   "FIN VERDE"                   .:.:. | |   `-'   | |---....____             "BLANCO"Ana G."FIN VERDE"   | ,---\\/--/  ' ||:.:.   "BLANCO"     ..........\n");
		printf("            ........  "FIN VERDE"                     ------| |         : :    ,.     ```--..._   , "BLANCO"Gutson"FIN VERDE"  |''  '  '  ' ' ||----     "BLANCO"     ........\n");
		printf("             ....... "FIN VERDE"                      _...--. |  ,       \\ \\             ,.    `-._     ,  /: '  '  '  ' ' ;;..._       "BLANCO"    .......\n");
		printf("              ......   "FIN VERDE"                    :.:. .| | -O-       \\ \\    ,.                `._    / /:'  '  '  ' ':: .:.:      "BLANCO"      ......\n");
		printf("               .....           "FIN VERDE"            .:. . | |_(`__       \\ \\                        `. / / :'  '  '  ' ';;. .:.        "BLANCO"     .....\n");
		printf("               .....         "FIN VERDE"              :. . .<' (_)  `>      `.`.          ,.    ,.     ,','   \\  '  '  ' ;;. . .:     "BLANCO"         ....\n");
		printf("                 ..    "FIN VERDE"                    .:. . |):-.--'(         `.`-._  ,.           _,-','      \\ '  '  '//| . .:.     "BLANCO"          ...\n");
		printf("                 .. "FIN VERDE"                      :. . .;)()(__)(___________`-._`-.._______..-'_.-'___________\'  '  //_:. . .:      "BLANCO"          .."FIN VERDE"\n");
		printf("                _::_                    .:.:,' \\/\\/--\\/--------------------------------------------`._',;'`. `.:.:.                  _::_\n");
		printf("               :    :                   :.,' ,' ,'  ,'  /   /   /   ,-------------------.   \\   \\   \\  `. `.`. `..:                 :    :\n");
		printf("______________:      :______________,' ,'  '   /   /   /   /   //                   \\   \\   \\   \\   \\  ` `.A.GUTSON________________:      :________________\n");
		detener_el_tiempo(0.125);
		fflush(stdout);

		system("clear");
		printf("\n\n                                                               . .:.:.:.:. .:\\     /:. .:.:.:.:. ,                   \n");
		printf("                                                          .-._  `..:.:. . .:.:`- -':.:. . .:.:.,'  _.-.                  \n");
		printf("                                                         .:.:.`-._`-._..-''_...---..._``-.._.-'_.-'.:.:.                 \n");
		printf("                                                      .:.:. . .:_.`' _..-''._________,``-.._ `.._:. . .:.:.              \n");
		printf("                                                   .:.:. . . ,-'_.-''      ||_-(O)-_||      ``-._`-. . . .:.:.           \n");
		printf("                                                  .:. . . .,'_.'           '---------'           `._`.. . . .:.          \n");
		printf("                                                :.:. . . ,','               _________               `.`. . . .:.:        \n");
		printf("                                               `.:.:. .,','            _.-''_________``-._            `._.     _.'       \n");
		printf("                                             -._  `._./ /            ,'_.-'' ,       ``-._`.          ,' '`:..'  _.-     \n");
		printf("                                            .:.:`-.._' /           ,','                   `.`.       /'  '  \\.-':.:.    \n");
		printf("                                            :.:. . ./ /          ,','    DEFENDIENDO LAS    `.`.    / '  '  '\\. .:.:    \n");
		printf("                                           :.:. . ./ /          / /    ,                      \\ \\  :  '  '  ' \\. .:.:   \n");
		printf("    "BLANCO".............        "FIN VERDE"                  .:. . ./ /          / /           TORRES      ,     \\ \\ :  '  '  ' '::. .:.   "BLANCO"      .................\n");
		printf("    ............             "FIN VERDE"              :. . .: :    o     / /                               \\ ;'  '  '  ' ':: . .:        "BLANCO"  ...............\n");
		printf("     .........          "FIN VERDE"                   .:. . | |   /_\\   : :     ,                      ,    : '  '  '  ' ' :: .:.    "BLANCO"      ...............\n");
		printf("      .........        "FIN VERDE"                    :. . .| |  ((<))  | |,          ,       ,   "BLANCO"By"FIN VERDE"        |\\'__',-._.' ' ||. .: "BLANCO"           ...........\n");
		printf("       ..........    "FIN VERDE"                      .:.:. | |   `-'   | |---....____             "BLANCO"Ana G."FIN VERDE"   | ,---\\/--/  ' ||:.:. "BLANCO"            .........\n");
		printf("         ........      "FIN VERDE"                    ------| |         : :    ,.     ```--..._   , "BLANCO"Gutson"FIN VERDE"  |''  '  '  ' ' ||----   "BLANCO"          .........\n");
		printf("          .......  "FIN VERDE"                        _...--. |  ,       \\ \\             ,.    `-._     ,  /: '  '  '  ' ' ;;..._      "BLANCO"        .......\n");
		printf("           ......       "FIN VERDE"                   :.:. .| | -O-       \\ \\    ,.                `._    / /:'  '  '  ' ':: .:.:       "BLANCO"        .....\n");
		printf("            .....    "FIN VERDE"                      .:. . | |_(`__       \\ \\                        `. / / :'  '  '  ' ';;. .:.        "BLANCO"       .....\n");
		printf("            .....  "FIN VERDE"                        :. . .<' (_)  `>      `.`.          ,.    ,.     ,','   \\  '  '  ' ;;. . .:          "BLANCO"      ...\n");
		printf("                ... "FIN VERDE"                       .:. . |):-.--'(         `.`-._  ,.           _,-','      \\ '  '  '//| . .:.           "BLANCO"     ..\n");
		printf("                 .."FIN VERDE"                       :. . .;)()(__)(___________`-._`-.._______..-'_.-'___________\'  '  //_:. . .:            "BLANCO"    .."FIN VERDE"\n");
		printf("                _::_                    .:.:,' \\/\\/--\\/--------------------------------------------`._',;'`. `.:.:.                  _::_\n");
		printf("               :    :                   :.,' ,' ,'  ,'  /   /   /   ,-------------------.   \\   \\   \\  `. `.`. `..:                 :    :\n");
		printf("______________:      :______________,' ,'  '   /   /   /   /   //                   \\   \\   \\   \\   \\  ` `.A.GUTSON________________:      :________________\n");
		detener_el_tiempo(0.125);
		fflush(stdout);

		system("clear");
		printf("\n\n                                                               . .:.:.:.:. .:\\     /:. .:.:.:.:. ,                   \n");
		printf("                                                          .-._  `..:.:. . .:.:`- -':.:. . .:.:.,'  _.-.                  \n");
		printf("                                                         .:.:.`-._`-._..-''_...---..._``-.._.-'_.-'.:.:.                 \n");
		printf("                                                      .:.:. . .:_.`' _..-''._________,``-.._ `.._:. . .:.:.              \n");
		printf("                                                   .:.:. . . ,-'_.-''      ||_-(O)-_||      ``-._`-. . . .:.:.           \n");
		printf("                                                  .:. . . .,'_.'           '---------'           `._`.. . . .:.          \n");
		printf("                                                :.:. . . ,','               _________               `.`. . . .:.:        \n");
		printf("                                               `.:.:. .,','            _.-''_________``-._            `._.     _.'       \n");
		printf("                                             -._  `._./ /            ,'_.-'' ,       ``-._`.          ,' '`:..'  _.-     \n");
		printf("                                            .:.:`-.._' /           ,','                   `.`.       /'  '  \\.-':.:.    \n");
		printf("                                            :.:. . ./ /          ,','    DEFENDIENDO LAS    `.`.    / '  '  '\\. .:.:    \n");
		printf("                                           :.:. . ./ /          / /    ,                      \\ \\  :  '  '  ' \\. .:.:   \n");
		printf("      "BLANCO"............."FIN VERDE"                        .:. . ./ /          / /           TORRES      ,     \\ \\ :  '  '  ' '::. .:.       "BLANCO"         ...................\n");
		printf("      ............    "FIN VERDE"                     :. . .: :    o     / /                               \\ ;'  '  '  ' ':: . .:    "BLANCO"            .................\n");
		printf("       .........   "FIN VERDE"                        .:. . | |   /_\\   : :     ,                      ,    : '  '  '  ' ' :: .:.    "BLANCO"            ...............\n");
		printf("        .........  "FIN VERDE"                        :. . .| |  ((<))  | |,          ,       ,   "BLANCO"By"FIN VERDE"        |\\'__',-._.' ' ||. .: "BLANCO"               ............\n");
		printf("         ..........  "FIN VERDE"                      .:.:. | |   `-'   | |---....____             "BLANCO"Ana G."FIN VERDE"   | ,---\\/--/  ' ||:.:. "BLANCO"               ..........\n");
		printf("           ........ "FIN VERDE"                       ------| |         : :    ,.     ```--..._   , "BLANCO"Gutson"FIN VERDE"  |''  '  '  ' ' ||----"BLANCO"                ........\n");
		printf("            .......  "FIN VERDE"                      _...--. |  ,       \\ \\             ,.    `-._     ,  /: '  '  '  ' ' ;;..._    "BLANCO"            .......\n");
		printf("             ...... "FIN VERDE"                       :.:. .| | -O-       \\ \\    ,.                `._    / /:'  '  '  ' ':: .:.: "BLANCO"               ......\n");
		printf("              .....  "FIN VERDE"                      .:. . | |_(`__       \\ \\                        `. / / :'  '  '  ' ';;. .:.        "BLANCO"        .....\n");
		printf("              .....   "FIN VERDE"                     :. . .<' (_)  `>      `.`.          ,.    ,.     ,','   \\  '  '  ' ;;. . .:       "BLANCO"         ....\n");
		printf("                 ..    "FIN VERDE"                    .:. . |):-.--'(         `.`-._  ,.           _,-','      \\ '  '  '//| . .:.       "BLANCO"         ...\n");
		printf("                 ..     "FIN VERDE"                  :. . .;)()(__)(___________`-._`-.._______..-'_.-'___________\'  '  //_:. . .:      "BLANCO"          .."FIN VERDE"\n");
		printf("                _::_                    .:.:,' \\/\\/--\\/--------------------------------------------`._',;'`. `.:.:.                  _::_ \n");
		printf("               :    :                   :.,' ,' ,'  ,'  /   /   /   ,-------------------.   \\   \\   \\  `. `.`. `..:                 :    :\n");
		printf("______________:      :______________,' ,'  '   /   /   /   /   //                   \\   \\   \\   \\   \\  ` `.A.GUTSON________________:      :________________\n");
		detener_el_tiempo(0.125);
		fflush(stdout);

		system("clear");
		printf("\n\n                                                               . .:.:.:.:. .:\\     /:. .:.:.:.:. ,                   \n");
		printf("                                                          .-._  `..:.:. . .:.:`- -':.:. . .:.:.,'  _.-.                  \n");
		printf("                                                         .:.:.`-._`-._..-''_...---..._``-.._.-'_.-'.:.:.                 \n");
		printf("                                                      .:.:. . .:_.`' _..-''._________,``-.._ `.._:. . .:.:.              \n");
		printf("                                                   .:.:. . . ,-'_.-''      ||_-(O)-_||      ``-._`-. . . .:.:.           \n");
		printf("                                                  .:. . . .,'_.'           '---------'           `._`.. . . .:.          \n");
		printf("                                                :.:. . . ,','               _________               `.`. . . .:.:        \n");
		printf("                                               `.:.:. .,','            _.-''_________``-._            `._.     _.'       \n");
		printf("                                             -._  `._./ /            ,'_.-'' ,       ``-._`.          ,' '`:..'  _.-     \n");
		printf("                                            .:.:`-.._' /           ,','                   `.`.       /'  '  \\.-':.:.    \n");
		printf("                                            :.:. . ./ /          ,','    DEFENDIENDO LAS    `.`.    / '  '  '\\. .:.:    \n");
		printf("                                           :.:. . ./ /          / /    ,                      \\ \\  :  '  '  ' \\. .:.:   \n");
		printf("        "BLANCO"............."FIN VERDE"                      .:. . ./ /          / /           TORRES      ,     \\ \\ :  '  '  ' '::. .:.   "BLANCO"                ...................\n");
		printf("         ............"FIN VERDE"                      :. . .: :    o     / /                               \\ ;'  '  '  ' ':: . .:         "BLANCO"          .................\n");
		printf("          ........."FIN VERDE"                        .:. . | |   /_\\   : :     ,                      ,    : '  '  '  ' ' :: .:.                "BLANCO"  ...............\n");
		printf("           ........."FIN VERDE"                       :. . .| |  ((<))  | |,          ,       ,   "BLANCO"By"FIN VERDE"        |\\'__',-._.' ' ||. .:     "BLANCO"             ............\n");
		printf("            .........."FIN VERDE"                     .:.:. | |   `-'   | |---....____             "BLANCO"Ana G."FIN VERDE"   | ,---\\/--/  ' ||:.:.      "BLANCO"            ..........\n");
		printf("             ........"FIN VERDE"                      ------| |         : :    ,.     ```--..._   , "BLANCO"Gutson"FIN VERDE"  |''  '  '  ' ' ||----        "BLANCO"         ........\n");
		printf("              ......."FIN VERDE"                      _...--. |  ,       \\ \\             ,.    `-._     ,  /: '  '  '  ' ' ;;..._           "BLANCO"      .......\n");
		printf("               ......"FIN VERDE"                      :.:. .| | -O-       \\ \\    ,.                `._    / /:'  '  '  ' ':: .:.:      "BLANCO"           ......\n");
		printf("                ....."FIN VERDE"                      .:. . | |_(`__       \\ \\                        `. / / :'  '  '  ' ';;. .:.       "BLANCO"         .....\n");
		printf("                ....."FIN VERDE"                      :. . .<' (_)  `>      `.`.          ,.    ,.     ,','   \\  '  '  ' ;;. . .:        "BLANCO"        .....\n");
		printf("                 .."FIN VERDE"                        .:. . |):-.--'(         `.`-._  ,.           _,-','      \\ '  '  '//| . .:.       "BLANCO"         ....\n");
		printf("                 .."FIN VERDE"                       :. . .;)()(__)(___________`-._`-.._______..-'_.-'___________\'  '  //_:. . .:       "BLANCO"         .."FIN VERDE"\n");
		printf("                _::_                    .:.:,' \\/\\/--\\/--------------------------------------------`._',;'`. `.:.:.                  _::_\n");
		printf("               :    :                   :.,' ,' ,'  ,'  /   /   /   ,-------------------.   \\   \\   \\  `. `.`. `..:                 :    :\n");
		printf("______________:      :______________,' ,'  '   /   /   /   /   //                   \\   \\   \\   \\   \\  ` `.A.GUTSON________________:      :________________\n");
		detener_el_tiempo(0.125);
		fflush(stdout);

		system("clear");
		printf("\n\n                                                               . .:.:.:.:. .:\\     /:. .:.:.:.:. ,                   \n");
		printf("                                                          .-._  `..:.:. . .:.:`- -':.:. . .:.:.,'  _.-.                  \n");
		printf("                                                         .:.:.`-._`-._..-''_...---..._``-.._.-'_.-'.:.:.                 \n");
		printf("                                                      .:.:. . .:_.`' _..-''._________,``-.._ `.._:. . .:.:.              \n");
		printf("                                                   .:.:. . . ,-'_.-''      ||_-(O)-_||      ``-._`-. . . .:.:.           \n");
		printf("                                                  .:. . . .,'_.'           '---------'           `._`.. . . .:.          \n");
		printf("                                                :.:. . . ,','               _________               `.`. . . .:.:        \n");
		printf("                                               `.:.:. .,','            _.-''_________``-._            `._.     _.'       \n");
		printf("                                             -._  `._./ /            ,'_.-'' ,       ``-._`.          ,' '`:..'  _.-     \n");
		printf("                                            .:.:`-.._' /           ,','                   `.`.       /'  '  \\.-':.:.    \n");
		printf("                                            :.:. . ./ /          ,','    DEFENDIENDO LAS    `.`.    / '  '  '\\. .:.:    \n");
		printf("                                           :.:. . ./ /          / /    ,                      \\ \\  :  '  '  ' \\. .:.:   \n");
		printf("            "BLANCO"............."FIN VERDE"                  .:. . ./ /          / /           TORRES      ,     \\ \\ :  '  '  ' '::. .:.     "BLANCO"         ...................\n");
		printf("            ............."FIN VERDE"                  :. . .: :    o     / /                               \\ ;'  '  '  ' ':: . .:        "BLANCO"      .................\n");
		printf("            ............."FIN VERDE"                  .:. . | |   /_\\   : :     ,                      ,    : '  '  '  ' ' :: .:.        "BLANCO"      ...............\n");
		printf("            ............."FIN VERDE"                  :. . .| |  ((<))  | |,          ,       ,   "BLANCO"By"FIN VERDE"        |\\'__',-._.' ' ||. .:   "BLANCO"           ............\n");
		printf("            ............."FIN VERDE"                  .:.:. | |   `-'   | |---....____             "BLANCO"Ana G."FIN VERDE"   | ,---\\/--/  ' ||:.:.   "BLANCO"           ..........\n");
		printf("             ..........."FIN VERDE"                   ------| |         : :    ,.     ```--..._   , "BLANCO"Gutson"FIN VERDE"  |''  '  '  ' ' ||----     "BLANCO"          ........\n");
		printf("              ........."FIN VERDE"                    _...--. |  ,       \\ \\             ,.    `-._     ,  /: '  '  '  ' ' ;;..._        "BLANCO"       .......\n");
		printf("               ......."FIN VERDE"                     :.:. .| | -O-       \\ \\    ,.                `._    / /:'  '  '  ' ':: .:.:      "BLANCO"         ......\n");
		printf("                ....."FIN VERDE"                      .:. . | |_(`__       \\ \\                        `. / / :'  '  '  ' ';;. .:.      "BLANCO"          .....\n");
		printf("                 ..."FIN VERDE"                       :. . .<' (_)  `>      `.`.          ,.    ,.     ,','   \\  '  '  ' ;;. . .:       "BLANCO"         .....\n");
		printf("                 .."FIN VERDE"                        .:. . |):-.--'(         `.`-._  ,.           _,-','      \\ '  '  '//| . .:.    "BLANCO"            ....\n");
		printf("                 .."FIN VERDE"                       :. . .;)()(__)(___________`-._`-.._______..-'_.-'___________\'  '  //_:. . .:        "BLANCO"        .."FIN VERDE"\n");
		printf("                _::_                    .:.:,' \\/\\/--\\/--------------------------------------------`._',;'`. `.:.:.                  _::_\n");
		printf("               :    :                   :.,' ,' ,'  ,'  /   /   /   ,-------------------.   \\   \\   \\  `. `.`. `..:                 :    :\n");
		printf("______________:      :______________,' ,'  '   /   /   /   /   //                   \\   \\   \\   \\   \\  ` `.A.GUTSON________________:      :________________\n");
		detener_el_tiempo(0.125);
		fflush(stdout);

		system("clear");
		printf("\n\n                                                               . .:.:.:.:. .:\\     /:. .:.:.:.:. ,                   \n");
		printf("                                                          .-._  `..:.:. . .:.:`- -':.:. . .:.:.,'  _.-.                  \n");
		printf("                                                         .:.:.`-._`-._..-''_...---..._``-.._.-'_.-'.:.:.                 \n");
		printf("                                                      .:.:. . .:_.`' _..-''._________,``-.._ `.._:. . .:.:.              \n");
		printf("                                                   .:.:. . . ,-'_.-''      ||_-(O)-_||      ``-._`-. . . .:.:.           \n");
		printf("                                                  .:. . . .,'_.'           '---------'           `._`.. . . .:.          \n");
		printf("                                                :.:. . . ,','               _________               `.`. . . .:.:        \n");
		printf("                                               `.:.:. .,','            _.-''_________``-._            `._.     _.'       \n");
		printf("                                             -._  `._./ /            ,'_.-'' ,       ``-._`.          ,' '`:..'  _.-     \n");
		printf("                                            .:.:`-.._' /           ,','                   `.`.       /'  '  \\.-':.:.    \n");
		printf("                                            :.:. . ./ /          ,','    DEFENDIENDO LAS    `.`.    / '  '  '\\. .:.:    \n");
		printf("                                           :.:. . ./ /          / /    ,                      \\ \\  :  '  '  ' \\. .:.:   \n");
		printf("               "BLANCO"..........."FIN VERDE"                 .:. . ./ /          / /           TORRES      ,     \\ \\ :  '  '  ' '::. .:.      "BLANCO"    ................\n");
		printf("               ..........."FIN VERDE"                 :. . .: :    o     / /                               \\ ;'  '  '  ' ':: . .:    "BLANCO"      ..............\n");
		printf("               ..........."FIN VERDE"                 .:. . | |   /_\\   : :     ,                      ,    : '  '  '  ' ' :: .:.    "BLANCO"      .............\n");
		printf("               ..........."FIN VERDE"                 :. . .| |  ((<))  | |,          ,       ,   "BLANCO"By"FIN VERDE"        |\\'__',-._.' ' ||. .: "BLANCO"         ............\n");
		printf("               ..........."FIN VERDE"                 .:.:. | |   `-'   | |---....____             "BLANCO"Ana G."FIN VERDE"   | ,---\\/--/  ' ||:.:. "BLANCO"         ..........\n");
		printf("               .........."FIN VERDE"                  ------| |         : :    ,.     ```--..._   , "BLANCO"Gutson"FIN VERDE"  |''  '  '  ' ' ||---- "BLANCO"          ........\n");
		printf("               ........."FIN VERDE"                   _...--. |  ,       \\ \\             ,.    `-._     ,  /: '  '  '  ' ' ;;..._       "BLANCO"     .......\n");
		printf("                ......."FIN VERDE"                    :.:. .| | -O-       \\ \\    ,.                `._    / /:'  '  '  ' ':: .:.:    "BLANCO"         ......\n");
		printf("                 ....."FIN VERDE"                     .:. . | |_(`__       \\ \\                        `. / / :'  '  '  ' ';;. .:.    "BLANCO"          .....\n");
		printf("                  ..."FIN VERDE"                      :. . .<' (_)  `>      `.`.          ,.    ,.     ,','   \\  '  '  ' ;;. . .:     "BLANCO"         .....\n");
		printf("                  .."FIN VERDE"                       .:. . |):-.--'(         `.`-._  ,.           _,-','      \\ '  '  '//| . .:.    "BLANCO"            ..\n");
		printf("                 .."FIN VERDE"                       :. . .;)()(__)(___________`-._`-.._______..-'_.-'___________\'  '  //_:. . .:        "BLANCO"        .."FIN VERDE"\n");
		printf("                _::_                    .:.:,' \\/\\/--\\/--------------------------------------------`._',;'`. `.:.:.                  _::_\n");
		printf("               :    :                   :.,' ,' ,'  ,'  /   /   /   ,-------------------.   \\   \\   \\  `. `.`. `..:                 :    :\n");
		printf("______________:      :______________,' ,'  '   /   /   /   /   //                   \\   \\   \\   \\   \\  ` `.A.GUTSON________________:      :________________\n");
		detener_el_tiempo(0.125);
		fflush(stdout);
		
		system("clear");
		printf("\n\n                                                               . .:.:.:.:. .:\\     /:. .:.:.:.:. ,                   \n");
		printf("                                                          .-._  `..:.:. . .:.:`- -':.:. . .:.:.,'  _.-.                  \n");
		printf("                                                         .:.:.`-._`-._..-''_...---..._``-.._.-'_.-'.:.:.                 \n");
		printf("                                                      .:.:. . .:_.`' _..-''._________,``-.._ `.._:. . .:.:.              \n");
		printf("                                                   .:.:. . . ,-'_.-''      ||_-(O)-_||      ``-._`-. . . .:.:.           \n");
		printf("                                                  .:. . . .,'_.'           '---------'           `._`.. . . .:.          \n");
		printf("                                                :.:. . . ,','               _________               `.`. . . .:.:        \n");
		printf("                                               `.:.:. .,','            _.-''_________``-._            `._.     _.'       \n");
		printf("                                             -._  `._./ /            ,'_.-'' ,       ``-._`.          ,' '`:..'  _.-     \n");
		printf("                                            .:.:`-.._' /           ,','                   `.`.       /'  '  \\.-':.:.    \n");
		printf("                                            :.:. . ./ /          ,','    DEFENDIENDO LAS    `.`.    / '  '  '\\. .:.:    \n");
		printf("                                           :.:. . ./ /          / /    ,                      \\ \\  :  '  '  ' \\. .:.:   \n");
		printf("                    "BLANCO"..........."FIN VERDE"            .:. . ./ /          / /           TORRES      ,     \\ \\ :  '  '  ' '::. .:. "BLANCO"  .............\n");
		printf("                    ..........."FIN VERDE"            :. . .: :    o     / /                               \\ ;'  '  '  ' ':: . .:  "BLANCO" ............\n");
		printf("                   ..........."FIN VERDE"             .:. . | |   /_\\   : :     ,                      ,    : '  '  '  ' ' :: .:. "BLANCO"   .........\n");
		printf("                   ..........."FIN VERDE"             :. . .| |  ((<))  | |,          ,       ,   "BLANCO"By"FIN VERDE"        |\\'__',-._.' ' ||. .: "BLANCO"    .........\n");
		printf("                  ..........."FIN VERDE"              .:.:. | |   `-'   | |---....____             "BLANCO"Ana G."FIN VERDE"   | ,---\\/--/  ' ||:.:.  "BLANCO"    ..........\n");
		printf("                 .........."FIN VERDE"                ------| |         : :    ,.     ```--..._   , "BLANCO"Gutson"FIN VERDE"  |''  '  '  ' ' ||----  "BLANCO"      ........\n");
		printf("                 ........."FIN VERDE"                 _...--. |  ,       \\ \\             ,.    `-._     ,  /: '  '  '  ' ' ;;..._   "BLANCO"      .......\n");
		printf("                  ......."FIN VERDE"                  :.:. .| | -O-       \\ \\    ,.                `._    / /:'  '  '  ' ':: .:.: "BLANCO"         ......\n");
		printf("                   ....."FIN VERDE"                   .:. . | |_(`__       \\ \\                        `. / / :'  '  '  ' ';;. .:. "BLANCO"          .....\n");
		printf("                  ..."FIN VERDE"                      :. . .<' (_)  `>      `.`.          ,.    ,.     ,','   \\  '  '  ' ;;. . .:  "BLANCO"         .....\n");
		printf("                  .."FIN VERDE"                       .:. . |):-.--'(         `.`-._  ,.           _,-','      \\ '  '  '//| . .:.   "BLANCO"            ...\n");
		printf("                 .."FIN VERDE"                       :. . .;)()(__)(___________`-._`-.._______..-'_.-'___________\'  '  //_:. . .:   "BLANCO"             .."FIN VERDE"\n");
		printf("                _::_                    .:.:,' \\/\\/--\\/--------------------------------------------`._',;'`. `.:.:.                  _::_\n");
		printf("               :    :                   :.,' ,' ,'  ,'  /   /   /   ,-------------------.   \\   \\   \\  `. `.`. `..:                 :    :\n");
		printf("______________:      :______________,' ,'  '   /   /   /   /   //                   \\   \\   \\   \\   \\  ` `.A.GUTSON________________:      :________________\n");
		detener_el_tiempo(0.125);
		fflush(stdout);
		
		system("clear"); 
		printf("\n\n                                                               . .:.:.:.:. .:\\     /:. .:.:.:.:. ,                   \n");
		printf("                                                          .-._  `..:.:. . .:.:`- -':.:. . .:.:.,'  _.-.                  \n");
		printf("                                                         .:.:.`-._`-._..-''_...---..._``-.._.-'_.-'.:.:.                 \n");
		printf("                                                      .:.:. . .:_.`' _..-''._________,``-.._ `.._:. . .:.:.              \n");
		printf("                                                   .:.:. . . ,-'_.-''      ||_-(O)-_||      ``-._`-. . . .:.:.           \n");
		printf("                                                  .:. . . .,'_.'           '---------'           `._`.. . . .:.          \n");
		printf("                                                :.:. . . ,','               _________               `.`. . . .:.:        \n");
		printf("                                               `.:.:. .,','            _.-''_________``-._            `._.     _.'       \n");
		printf("                                             -._  `._./ /            ,'_.-'' ,       ``-._`.          ,' '`:..'  _.-     \n");
		printf("                                            .:.:`-.._' /           ,','                   `.`.       /'  '  \\.-':.:.    \n");
		printf("                                            :.:. . ./ /          ,','    DEFENDIENDO LAS    `.`.    / '  '  '\\. .:.:    \n");
		printf("                                           :.:. . ./ /          / /    ,                      \\ \\  :  '  '  ' \\. .:.:   \n");
		printf("               "BLANCO"..........."FIN VERDE"                 .:. . ./ /          / /           TORRES      ,     \\ \\ :  '  '  ' '::. .:.  "BLANCO"   .............\n");
		printf("               ..........."FIN VERDE"                 :. . .: :    o     / /                               \\ ;'  '  '  ' ':: . .:  "BLANCO"   ............\n");
		printf("               ..........."FIN VERDE"                 .:. . | |   /_\\   : :     ,                      ,    : '  '  '  ' ' :: .:. "BLANCO"     .........\n");
		printf("               ..........."FIN VERDE"                 :. . .| |  ((<))  | |,          ,       ,   "BLANCO"By"FIN VERDE"        |\\'__',-._.' ' ||. .:  "BLANCO"     .........\n");
		printf("               ..........."FIN VERDE"                 .:.:. | |   `-'   | |---....____             "BLANCO"Ana G."FIN VERDE"   | ,---\\/--/  ' ||:.:.   "BLANCO"     ..........\n");
		printf("               .........."FIN VERDE"                  ------| |         : :    ,.     ```--..._   , "BLANCO"Gutson"FIN VERDE"  |''  '  '  ' ' ||----   "BLANCO"       ........\n");
		printf("               ........."FIN VERDE"                   _...--. |  ,       \\ \\             ,.    `-._     ,  /: '  '  '  ' ' ;;..._"BLANCO"           .......\n");
		printf("                ......."FIN VERDE"                    :.:. .| | -O-       \\ \\    ,.                `._    / /:'  '  '  ' ':: .:.:        "BLANCO"    ......\n");
		printf("                 ....."FIN VERDE"                     .:. . | |_(`__       \\ \\                        `. / / :'  '  '  ' ';;. .:.      "BLANCO"       .....\n");
		printf("                  ..."FIN VERDE"                      :. . .<' (_)  `>      `.`.          ,.    ,.     ,','   \\  '  '  ' ;;. . .:     "BLANCO"        .....\n");
		printf("                  .."FIN VERDE"                       .:. . |):-.--'(         `.`-._  ,.           _,-','      \\ '  '  '//| . .:.     "BLANCO"           ..\n");
		printf("                 .."FIN VERDE"                       :. . .;)()(__)(___________`-._`-.._______..-'_.-'___________\'  '  //_:. . .:     "BLANCO"           .."FIN VERDE"\n");
		printf("                _::_                    .:.:,' \\/\\/--\\/--------------------------------------------`._',;'`. `.:.:.                  _::_\n");
		printf("               :    :                   :.,' ,' ,'  ,'  /   /   /   ,-------------------.   \\   \\   \\  `. `.`. `..:                 :    :\n");
		printf("______________:      :______________,' ,'  '   /   /   /   /   //                   \\   \\   \\   \\   \\  ` `.A.GUTSON________________:      :________________\n");
		detener_el_tiempo(0.125);
		fflush(stdout);

		system("clear");
		printf("\n\n                                                               . .:.:.:.:. .:\\     /:. .:.:.:.:. ,                   \n");
		printf("                                                          .-._  `..:.:. . .:.:`- -':.:. . .:.:.,'  _.-.                  \n");
		printf("                                                         .:.:.`-._`-._..-''_...---..._``-.._.-'_.-'.:.:.                 \n");
		printf("                                                      .:.:. . .:_.`' _..-''._________,``-.._ `.._:. . .:.:.              \n");
		printf("                                                   .:.:. . . ,-'_.-''      ||_-(O)-_||      ``-._`-. . . .:.:.           \n");
		printf("                                                  .:. . . .,'_.'           '---------'           `._`.. . . .:.          \n");
		printf("                                                :.:. . . ,','               _________               `.`. . . .:.:        \n");
		printf("                                               `.:.:. .,','            _.-''_________``-._            `._.     _.'       \n");
		printf("                                             -._  `._./ /            ,'_.-'' ,       ``-._`.          ,' '`:..'  _.-     \n");
		printf("                                            .:.:`-.._' /           ,','                   `.`.       /'  '  \\.-':.:.    \n");
		printf("                                            :.:. . ./ /          ,','    DEFENDIENDO LAS    `.`.    / '  '  '\\. .:.:    \n");
		printf("                                           :.:. . ./ /          / /    ,                      \\ \\  :  '  '  ' \\. .:.:   \n");
		printf("            "BLANCO"............."FIN VERDE"                  .:. . ./ /          / /           TORRES      ,     \\ \\ :  '  '  ' '::. .:.   "BLANCO"    .............\n");
		printf("            ............."FIN VERDE"                  :. . .: :    o     / /                               \\ ;'  '  '  ' ':: . .:       "BLANCO" ............\n");
		printf("            ............."FIN VERDE"                  .:. . | |   /_\\   : :     ,                      ,    : '  '  '  ' ' :: .:.    "BLANCO"     .........\n");
		printf("            ............."FIN VERDE"                  :. . .| |  ((<))  | |,          ,       ,   "BLANCO"By"FIN VERDE"        |\\'__',-._.' ' ||. .:"BLANCO"          .........\n");
		printf("            ............."FIN VERDE"                  .:.:. | |   `-'   | |---....____             "BLANCO"Ana G."FIN VERDE"   | ,---\\/--/  ' ||:.:.        "BLANCO"   ..........\n");
		printf("             ..........."FIN VERDE"                   ------| |         : :    ,.     ```--..._   , "BLANCO"Gutson"FIN VERDE"  |''  '  '  ' ' ||----    "BLANCO"        ........\n");
		printf("              ........."FIN VERDE"                    _...--. |  ,       \\ \\             ,.    `-._     ,  /: '  '  '  ' ' ;;..._    "BLANCO"         .......\n");
		printf("               ......."FIN VERDE"                     :.:. .| | -O-       \\ \\    ,.                `._    / /:'  '  '  ' ':: .:.:    "BLANCO"          ......\n");
		printf("                ....."FIN VERDE"                      .:. . | |_(`__       \\ \\                        `. / / :'  '  '  ' ';;. .:.    "BLANCO"           .....\n");
		printf("                 ..."FIN VERDE"                       :. . .<' (_)  `>      `.`.          ,.    ,.     ,','   \\  '  '  ' ;;. . .:     "BLANCO"          .....\n");
		printf("                 .."FIN VERDE"                        .:. . |):-.--'(         `.`-._  ,.           _,-','      \\ '  '  '//| . .:.       "BLANCO"         ..\n");
		printf("                 .."FIN VERDE"                       :. . .;)()(__)(___________`-._`-.._______..-'_.-'___________\'  '  //_:. . .:      "BLANCO"          .."FIN VERDE"\n");
		printf("                _::_                    .:.:,' \\/\\/--\\/--------------------------------------------`._',;'`. `.:.:.                  _::_\n");
		printf("               :    :                   :.,' ,' ,'  ,'  /   /   /   ,-------------------.   \\   \\   \\  `. `.`. `..:                 :    :\n");
		printf("______________:      :______________,' ,'  '   /   /   /   /   //                   \\   \\   \\   \\   \\  ` `.A.GUTSON________________:      :________________\n");
		printf(FIN);
		detener_el_tiempo(0.125);
		fflush(stdout);
	}
}

/*Pre-condiciones: 
	-.
Post-condiciones:
	la función mostrará la carga del juego.*/
void mostrar_carga_juego(){

    system("clear");
	detener_el_tiempo(0.125);
	
	animacion();
    system("clear");
	detener_el_tiempo(0.125);

    printf(BLANCO "Iniciando...\n" VERDE);
    mostrar_barra_progreso();
    printf(FIN "\nFinalizado" LINEA);
    detener_el_tiempo(1);
    system("clear");
}

/*Pre-condiciones: 
	-.
Post-condiciones:
	la función representará la épica batalla entre los orcos y el equipo de Gimli y Legolas.*/
int main (){

	int viento = 0, humedad = 0;
	char animo_legolas = CARACTER_DEFAULT, animo_gimli = CARACTER_DEFAULT;
	srand ((unsigned)time(NULL));
	juego_t juego;

	mostrar_carga_juego();
	animos(&viento, &humedad, &animo_legolas, &animo_gimli);
	mostrar_carga_inicio_TP2();
	cargar_instrucciones();
	inicializar_juego(&juego, viento, humedad, animo_legolas, animo_gimli);
    do{
    	
    	juego.nivel_actual++;
    	mostrar_mensaje_nivel_actual(juego.nivel_actual);
    	inicializar_nivel(&juego);
		jugar_nivel(&juego);
    	
    	if (estado_nivel(juego.nivel) == GANADO && estado_juego(juego) == JUGANDO)
    		mostrar_mensaje_felicitaciones();   
    }while (estado_juego(juego) == JUGANDO);

    if (estado_juego(juego) == PERDIDO)
    	mostrar_mensaje_juego_perdido(juego.nivel_actual);
    else
    	mostrar_mensaje_juego_ganado();
}