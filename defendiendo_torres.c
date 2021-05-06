#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "defendiendo_torres.h"

#define MAX_FILAS					30
#define MAX_COLUMNAS 				30
#define MAX_LONGITUD_CAMINO 		200
#define MAX_ENEMIGOS 				500
#define MAX_DEFENSORES 				50
#define MAX_NOMBRE_PUNTO_CARDINAL	20

#define ULTIMO_NIVEL 	4

#define REGULAR 'R'
#define MALO 	'M'
#define BUENO 	'B'

#define PORCENTAJE_CRITICO_ANIMO_MALO 		0
#define PORCENTAJE_CRITICO_ANIMO_REGULAR 	10
#define PORCENTAJE_CRITICO_ANIMO_BUENO 		25

#define VIDA_INICIAL_TORRE_1	600
#define VIDA_INICIAL_TORRE_2	600
#define VIDA_INICIAL_ORCO		200
#define VIDA_RANDOM_ORCO_MAX	100
#define ENANOS_EXTRAS_INICIAL	10
#define ELFOS_EXTRAS_INICIAL	10

#define DANIO_FALLIDO_ENANO 	0
#define DANIO_NORMAL_ENANO  	60
#define DANIO_CRITICO_ENANO 	40
#define DISTANCIA_ATAQUE_ENANO	1

#define DANIO_FALLIDO_ELFO 		0
#define DANIO_NORMAL_ELFO   	30
#define DANIO_CRITICO_ELFO  	40
#define DISTANCIA_ATAQUE_ELFO	3

#define ESTE 	'E'
#define OESTE 	'O'
#define NORTE 	'N'
#define SUR 	'S'

#define CAMINO_1 	'U'
#define CAMINO_2 	'D'
#define DOS_CAMINOS 'C'
#define NADA 		'N'
#define ENTRADA 	'E'
#define ENTRADA_1 	'1'
#define ENTRADA_2 	'2'
#define TORRE_1 	'3'
#define TORRE_2 	'4'

#define TAMANIO_NIVEL_1 		15
#define CARDINAL_NIVEL_1 		ESTE
#define TORRE_NIVEL_1			TORRE_1
#define CANT_CAMINOS_NIVEL_1    1

#define TAMANIO_NIVEL_2 		15
#define CARDINAL_NIVEL_2 		OESTE
#define TORRE_NIVEL_2			TORRE_2
#define CANT_CAMINOS_NIVEL_2    1

#define TAMANIO_NIVEL_3 		20
#define CARDINAL_NIVEL_3		NORTE
#define CANT_CAMINOS_NIVEL_3    2

#define TAMANIO_NIVEL_4 		20
#define CARDINAL_NIVEL_4 		SUR
#define CANT_CAMINOS_NIVEL_4    2

#define MIN_MUCHA_VIDA 200
#define MIN_MASO_VIDA  100


#define VERDE_CLARO "\033[0m\033[32m"
#define VERDE_OSCURO "\033[1m\033[32m"
#define FIN  		"\033[0m"
#define LINEA 		"\n-------------------------------------------------\n"
#define AMARILLO 	"\033[1m\033[33m"
#define BLANCO 		"\033[1m\033[37m"
#define SILVER 		"\033[0m\033[37m"
#define ROJO_OSCURO "\033[0m\033[31m"
#define ROJO_CLARO 	"\033[1m\033[31m"
#define AZUL 		"\033[1m\033[34m"
#define MARRON 		"\033[0m\033[33m"
#define MAGENTA 	"\033[1m\033[35m"
#define GRIS	 	"\033[1m\033[30m"

#define MARGEN 		"         "

#define ELFO 		 'L'
#define ENANO 		 'G'

#define ENEMIGO_1_MUCHA_VIDA 	 			'a'
#define ENEMIGO_1_MASO_VIDA 	 			'b'
#define ENEMIGO_1_POCA_VIDA 	 			'c'
#define ENEMIGO_2_MUCHA_VIDA 				'd'
#define ENEMIGO_2_MASO_VIDA 	 			'e'
#define ENEMIGO_2_POCA_VIDA 	 			'f'
#define DOS_ENEMIGOS_MUCHA_VIDA_MUCHA_VIDA 	'g'
#define DOS_ENEMIGOS_MUCHA_VIDA_MASO_VIDA   'h'
#define DOS_ENEMIGOS_MUCHA_VIDA_POCA_VIDA   'i'
#define DOS_ENEMIGOS_MASO_VIDA_MASO_VIDA    'j'
#define DOS_ENEMIGOS_MASO_VIDA_POCA_VIDA 	'k'
#define DOS_ENEMIGOS_POCA_VIDA_POCA_VIDA 	'l'

#define POSIBLE 0
#define NO_POSIBLE -1

#define GANADO 1
#define JUGANDO 0
#define PERDIDO -1

/*Pre-condiciones: 
	-.
Post-condiciones:
	la función mosdificará los valores de las posiciones recibidas.*/
void inicializo_torres(torres_t* torres){
	
	torres->resistencia_torre_1 = VIDA_INICIAL_TORRE_1;
	torres->resistencia_torre_2 = VIDA_INICIAL_TORRE_2;
	torres->enanos_extra = ENANOS_EXTRAS_INICIAL;
	torres->elfos_extra = ELFOS_EXTRAS_INICIAL;
}

/*Pre-condiciones: 
	"animo" debe ser REGULAR, MALO o BUENO.
Post-condiciones:
	la función devolverá un valor que puede ser PORCENTAJE_CRITICO_ANIMO_MALO, PORCENTAJE_CRITICO_ANIMO_REGULAR o PORCENTAJE_CRITICO_ANIMO_BUENO.*/
int valor_critico_segun_animo(char animo){
	int critico;

	if (animo == MALO)
		critico = PORCENTAJE_CRITICO_ANIMO_MALO;
	else if (animo == REGULAR)
		critico = PORCENTAJE_CRITICO_ANIMO_REGULAR;
	else
		critico = PORCENTAJE_CRITICO_ANIMO_BUENO;

	return critico;
}

/*Pre-condiciones: 
	-
Post-condiciones:
	la función devolverá la mitad del valor recibido.*/
int fallo_segun_condiciones(int condicion){

	return condicion/2;
}

// Pre y post en defendiendo_torres.h
void inicializar_juego(juego_t* juego, int viento, int humedad, char animo_legolas, char animo_gimli){

	inicializo_torres(&(juego->torres));

	juego->nivel_actual = 0;

	juego->critico_legolas = valor_critico_segun_animo(animo_legolas);
	juego->critico_gimli = valor_critico_segun_animo(animo_gimli);

	juego->fallo_legolas = fallo_segun_condiciones(viento);
	juego->fallo_gimli = fallo_segun_condiciones(humedad);
}

/*Pre-condiciones: 
    "nivel" debe ser 1, 2, 3 o 4;
Post-condiciones: 
    la función retorna "tamanio" que es TAMANIO_NIVEL_1, TAMANIO_NIVEL_2, TAMANIO_NIVEL_3 o TAMANIO_NIVEL_4.*/
int tamano_tablero_segun_nivel(int nivel){
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
    "tamanio_tablero" positivo y menor a MAX_FILAS Y MAX_COLUMNAS;
Post-condiciones: 
    la función modifica las posiciones recibidas por VACIO.*/
void inicializar_matriz(char matriz[MAX_FILAS][MAX_COLUMNAS], int tamanio_tablero, char letra){
	
	for (int fila = 0; fila < tamanio_tablero; fila++){
		
		for (int columna = 0; columna < tamanio_tablero; columna++)		
			matriz[fila][columna] = letra;
	}
}

/*Pre-condiciones: 
    -
Post-condiciones: 
    la función retornará VERDADERO si letra == CAMINO_1.*/
bool hay_camino_en_matriz(char letra){

	return (letra == CAMINO_1);
}

/*Pre-condiciones: 
    "tope" positivo y menor a MAX_FILAS y MAX_COLUMNAS.
    "letra" debe ser CAMINO_1 o CAMINO_2.
Post-condiciones: 
    la función modificará los valores de matriz[][] según vector[] y "es_segundo_camino" por "letra" o DOS_CAMINOS.*/
void cambiar_matriz_segun_camino(char matriz[MAX_FILAS][MAX_COLUMNAS], coordenada_t vector[MAX_LONGITUD_CAMINO], int tope, char letra, bool es_segundo_camino){
	int fila_a_modificar = 0, columna_a_modificar = 0;
	
	if (es_segundo_camino){

		for (int i = 0; i < tope; i++){

			fila_a_modificar = vector[i].fil;
			columna_a_modificar = vector[i].col;
			
			if(hay_camino_en_matriz(matriz[fila_a_modificar][columna_a_modificar]))
				matriz[fila_a_modificar][columna_a_modificar] = DOS_CAMINOS;
			else
				matriz[fila_a_modificar][columna_a_modificar] = letra;
		}
	}
	else {

		for (int i = 0; i < tope; i++){

			fila_a_modificar = vector[i].fil;
			columna_a_modificar = vector[i].col;
			matriz[fila_a_modificar][columna_a_modificar] = letra;
		}
	}
}

/*Pre-condiciones: 
    "nivel.enemigos[]" inicializado y válido.
Post-condiciones: 
    la función modificará los valores de matriz[][] según nivel.enemigos[].*/
void agregar_defensores_a_matriz(char matriz[MAX_FILAS][MAX_COLUMNAS], nivel_t nivel){
	int fila_a_modificar = 0, columna_a_modificar = 0;
	
	for (int i = 0; i < (nivel.tope_defensores); i++){
		
		fila_a_modificar = nivel.defensores[i].posicion.fil;
		columna_a_modificar = nivel.defensores[i].posicion.col;
		matriz[fila_a_modificar][columna_a_modificar] = nivel.defensores[i].tipo;
	}
}

/*Pre-condiciones: 
    "camino" debe ser 1 o 2.
Post-condiciones: 
    la función retornará VERDADERO si ya hay un orco.*/
bool hay_orco(char letra, int camino){
	bool comparacion;

	if (camino == 1)
		comparacion = ((letra == ENEMIGO_2_POCA_VIDA) || (letra == ENEMIGO_2_MASO_VIDA) || (letra == ENEMIGO_2_MUCHA_VIDA));
	else
		comparacion = ((letra == ENEMIGO_1_POCA_VIDA) || (letra == ENEMIGO_1_MASO_VIDA) || (letra == ENEMIGO_1_MUCHA_VIDA));

    return comparacion;
}

/*Pre-condiciones: 
    "posicion_a_modificar" debe ser una posición válida;
    "vida_enemigo_dos" debe ser mayor o igual a 0.
Post-condiciones: 
    la función cambiará el valor de matriz[posicion_a_modificar.fil][posicion_a_modificar.col] según las vidas de los orcos.*/
void determinar_vidas_orcos(char matriz[MAX_FILAS][MAX_COLUMNAS], coordenada_t posicion_a_modificar, int vida_enemigo_dos){
	char enemigo_1;

	enemigo_1 = matriz[posicion_a_modificar.fil][posicion_a_modificar.col];
	
	if ((enemigo_1 == ENEMIGO_1_MUCHA_VIDA || enemigo_1 == ENEMIGO_2_MUCHA_VIDA) && (vida_enemigo_dos >= MIN_MUCHA_VIDA))
		matriz[posicion_a_modificar.fil][posicion_a_modificar.col] = DOS_ENEMIGOS_MUCHA_VIDA_MUCHA_VIDA;
	
	else if ( ((enemigo_1 == ENEMIGO_1_MUCHA_VIDA || enemigo_1 == ENEMIGO_2_MUCHA_VIDA) && vida_enemigo_dos >= MIN_MASO_VIDA)
		|| ((enemigo_1 == ENEMIGO_1_MASO_VIDA || enemigo_1 == ENEMIGO_2_MASO_VIDA) && vida_enemigo_dos >= MIN_MUCHA_VIDA) )
		matriz[posicion_a_modificar.fil][posicion_a_modificar.col] = DOS_ENEMIGOS_MUCHA_VIDA_MASO_VIDA;

	else if ((enemigo_1 == ENEMIGO_1_MUCHA_VIDA || enemigo_1 == ENEMIGO_2_MUCHA_VIDA)
		|| ((enemigo_1 == ENEMIGO_1_POCA_VIDA || enemigo_1 == ENEMIGO_2_POCA_VIDA) && vida_enemigo_dos >= MIN_MUCHA_VIDA))
		matriz[posicion_a_modificar.fil][posicion_a_modificar.col] = DOS_ENEMIGOS_MUCHA_VIDA_POCA_VIDA;
	
	else if ((enemigo_1 == ENEMIGO_1_MASO_VIDA || enemigo_1 == ENEMIGO_2_MASO_VIDA)	&& (vida_enemigo_dos >= MIN_MASO_VIDA))
		matriz[posicion_a_modificar.fil][posicion_a_modificar.col] = DOS_ENEMIGOS_MASO_VIDA_MASO_VIDA;
	
	else if ((enemigo_1 == ENEMIGO_1_MASO_VIDA || enemigo_1 == ENEMIGO_2_MASO_VIDA)
		|| ((enemigo_1 == ENEMIGO_1_POCA_VIDA || enemigo_1 == ENEMIGO_2_POCA_VIDA) && vida_enemigo_dos >= MIN_MASO_VIDA))
		matriz[posicion_a_modificar.fil][posicion_a_modificar.col] = DOS_ENEMIGOS_MASO_VIDA_POCA_VIDA;

	else
		matriz[posicion_a_modificar.fil][posicion_a_modificar.col] = DOS_ENEMIGOS_POCA_VIDA_POCA_VIDA;
}

/*Pre-condiciones: 
    -
Post-condiciones: 
    la función cambiará el valor de matriz[posicion_a_modificar.fil][posicion_a_modificar.col] según nivel.enemigos[].*/
void agregar_enemigos_a_matriz(char matriz[MAX_FILAS][MAX_COLUMNAS], nivel_t nivel){
	coordenada_t posicion_a_modificar;

	for (int i = 0; i < (nivel.tope_enemigos); i++){

		if ( (nivel.enemigos[i].vida > 0) && (nivel.enemigos[i].camino == 1) ){
		
			posicion_a_modificar = nivel.camino_1[nivel.enemigos[i].pos_en_camino];
			if ( hay_orco(matriz[posicion_a_modificar.fil][posicion_a_modificar.col], nivel.enemigos[i].camino) )
				determinar_vidas_orcos(matriz, posicion_a_modificar, nivel.enemigos[i].vida);
			else{
				if (nivel.enemigos[i].vida >= MIN_MUCHA_VIDA)
					matriz[posicion_a_modificar.fil][posicion_a_modificar.col] = ENEMIGO_1_MUCHA_VIDA;
				else if (nivel.enemigos[i].vida >= MIN_MASO_VIDA)
					matriz[posicion_a_modificar.fil][posicion_a_modificar.col] = ENEMIGO_1_MASO_VIDA;
				else
					matriz[posicion_a_modificar.fil][posicion_a_modificar.col] = ENEMIGO_1_POCA_VIDA;
			}
		} else if( (nivel.enemigos[i].vida > 0) && (nivel.enemigos[i].camino == 2) ){

			posicion_a_modificar = nivel.camino_2[nivel.enemigos[i].pos_en_camino];
			if ( hay_orco(matriz[posicion_a_modificar.fil][posicion_a_modificar.col], nivel.enemigos[i].camino) )
				determinar_vidas_orcos(matriz, posicion_a_modificar, nivel.enemigos[i].vida);
			else{
				if (nivel.enemigos[i].vida >= MIN_MUCHA_VIDA)
					matriz[posicion_a_modificar.fil][posicion_a_modificar.col] = ENEMIGO_2_MUCHA_VIDA;
				else if (nivel.enemigos[i].vida >= MIN_MASO_VIDA)
					matriz[posicion_a_modificar.fil][posicion_a_modificar.col] = ENEMIGO_2_MASO_VIDA;
				else
					matriz[posicion_a_modificar.fil][posicion_a_modificar.col] = ENEMIGO_2_POCA_VIDA;
			}
		}
	}
}

/*Pre-condiciones: 
    "tamanio" positivo y menor a MAX_FILAS y MAX_COLUMNAS.
Post-condiciones: 
    la función retornará un valor a partir de "tamanio".*/
int calcular_largo_tablero(int tamanio){
	int suma = 8;

	for (int i = 0; i < tamanio; i++)
		suma += 3; 
	suma += 8;

	return suma;
}

/*Pre-condiciones: 
    "valor" positivo.
Post-condiciones: 
    la función devolverá la cantidad de digitos del valor.*/
int cant_digitos(int valor){
	int digitos = 0;
	
	do{
		
		digitos ++;
		valor/=10;
	}while (valor!=0);
	
	return digitos;
}

/*Pre-condiciones: 
    "largo_total" positivo
Post-condiciones: 
    la función mostrará la linea superior del cuadro.*/
void mostrar_linea_superior_cuadro(int largo_total){
	printf(MARGEN MARGEN MARGEN MARGEN MARGEN"█");
	for(int i = 0; i < largo_total; i++)
		printf("▀");
	printf("█\n" FIN);
}

/*Pre-condiciones: 
    "cant_espacios" positivo
Post-condiciones: 
    la función mostrará "cant_espacios" espacios*/
void mostrar_espacio(int cant_espacios){
	
	for(int i = 0; i < cant_espacios; i++)
		printf(" ");
}

/*Pre-condiciones: 
    "largo_total" positivo;
Post-condiciones: 
    la función mostrará una linea del cuadro con espacios.*/
void mostrar_linea_vacia_cuadro(int largo_total){
	printf(MARGEN MARGEN MARGEN MARGEN MARGEN"█");
	mostrar_espacio(largo_total);
	printf("█\n" FIN);
}

/*Pre-condiciones: 
    "vida_por_cuadrado" mayor a 0;
    "vida_inicial_torre" es VIDA_INICIAL_TORRE_1 o VIDA_INICIAL_TORRE_2;
    "resistencia_torre_actual" mayor o igual a 0 y menor o igual a "vida_inicial_torre".
Post-condiciones: 
    la función mostrará una barra con colores dependiendo de los valores de "resistencia_torre_actual" y "vida_inicial_torre".*/
void mostrar_barra_vida_torre(int resistencia_torre_actual, int vida_inicial_torre, int vida_por_cuadrado){
	int texto_1_a = (resistencia_torre_actual)/vida_por_cuadrado;

	for(int i = 0; i < vida_inicial_torre/vida_por_cuadrado + 1; i++){
		
		if (resistencia_torre_actual == 0)
			printf(ROJO_CLARO"▒"FIN);	
		else if (texto_1_a >= i)
			printf(VERDE_CLARO"▒"FIN );
		else
			printf(ROJO_CLARO"▒"FIN );	
	}

	printf("%i/%i", resistencia_torre_actual, vida_inicial_torre);
}

/*Pre-condiciones: 
    "tamanio_tablero" positivo y menor a MAX_FILAS Y MAX_COLUMNAS.
Post-condiciones: 
    la función mostrará una línea con la enumeración de las columnas del tablero.*/
void mostrar_cant_columnas_cuadro(int tamanio_tablero){
	
	printf(BLANCO);
	for (int i = 0; i < tamanio_tablero; i ++){
	
		if (cant_digitos(i) < 2)
			printf(" %i ", i);
		else
			printf(" %i", i);
	}
}

/*Pre-condiciones: 
    "nivel" debe ser 1, 2, 3 o 4.
Post-condiciones: 
    la función devolverá NORTE, SUR, ESTE u OESTE según "nivel".*/
char pto_cardinal_segun_nivel(int nivel){
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
    "nivel_actual" debe ser 1, 2, 3 o 4.
Post-condiciones: 
    la función modificará el valor de nombre[] según "nivel_actual".*/
void obtener_nombre_punto_cardinal_segun_nivel(int nivel_actual, char nombre[MAX_NOMBRE_PUNTO_CARDINAL]){
	char punto_cardinal;

	punto_cardinal = pto_cardinal_segun_nivel(nivel_actual);
	switch (punto_cardinal){
		case ESTE: 
			strcpy(nombre, "ESTE");
			break;
		case OESTE:
			strcpy(nombre, "OESTE");
			break;
		case NORTE:
			strcpy(nombre, "NORTE");
			break;
		case SUR:
			strcpy(nombre, "SUR");
			break;
	}
}

/*Pre-condiciones: 
    "largo_total" positivo;
    "nivel_actual" debe ser 1, 2, 3 o 4.
Post-condiciones: 
    la función mostrará una linea con el nivel actual.*/
void mostrar_linea_nivel(int largo_total, int nivel_actual){
	int texto_total, espacio, resto;
	char nombre_punto_cardinal[20] ={'\0'}, texto_1[] = "NIVEL ", texto_2[] = " - ";

	obtener_nombre_punto_cardinal_segun_nivel(nivel_actual, nombre_punto_cardinal);
	texto_total = (largo_total- (int) strlen(texto_1) - cant_digitos(nivel_actual) - (int) strlen(texto_2) - (int) strlen(nombre_punto_cardinal));
	espacio = texto_total/2;
	resto = texto_total%2;

	printf(MARGEN MARGEN MARGEN MARGEN MARGEN "█");
	mostrar_espacio(espacio);
	printf(BLANCO "%s%i%s%s" FIN , texto_1, nivel_actual, texto_2, nombre_punto_cardinal);
	mostrar_espacio(espacio + resto);
	printf("█\n" FIN);
}

/*Pre-condiciones: 
    "largo_total" positivo;
Post-condiciones: 
    la función mostrará una linea con texto de las torres.*/
void mostrar_linea_texto_torres(int largo_total){
	int texto_total, espacio, resto;
	char texto_1[]= "VIDA TORRE 1", texto_2[] = "VIDA TORRE 2";
	
	texto_total = (largo_total- (int) strlen(texto_1) - (int) strlen(texto_2));
	espacio = texto_total/3;
	resto = texto_total%3;

	printf(MARGEN MARGEN MARGEN MARGEN MARGEN "█");
	mostrar_espacio(espacio);
	printf(BLANCO "%s", texto_1);
	mostrar_espacio(espacio + resto);
	printf("%s", texto_2);
	mostrar_espacio(espacio);
	printf("█\n" FIN);
}

/*Pre-condiciones: 
    "largo_total" positivo;
    "resistencia_torre_1" y "resistencia_torre_2" mayores o iguales a 0 y menores o iguales a VIDA_INICIAL_TORRE_1 y VIDA_INICIAL_TORRE_2 respectivamente.
Post-condiciones: 
    la función mostrará una linea con información de la vida de las torres.*/
void mostrar_informacion_torres(int largo_total, int resistencia_torre_1, int resistencia_torre_2){
	int vida_por_cuadrado = 50, espacio, resto;
	int texto_1_a, texto_1_b, texto_2_a, texto_2_b;

	printf(MARGEN MARGEN MARGEN MARGEN MARGEN "█");
	texto_1_a = VIDA_INICIAL_TORRE_1/vida_por_cuadrado + 1;
	texto_1_b = cant_digitos(resistencia_torre_1) + 1 + cant_digitos(VIDA_INICIAL_TORRE_1);
	texto_2_a = VIDA_INICIAL_TORRE_2/vida_por_cuadrado + 1;
	texto_2_b = cant_digitos(resistencia_torre_2) + 1 + cant_digitos(VIDA_INICIAL_TORRE_2);
	espacio = (largo_total-(texto_2_a + texto_2_b + texto_1_a + texto_1_b))/3;
	resto = (largo_total-(texto_2_a + texto_2_b + texto_1_a + texto_1_b))%3;
	
	mostrar_espacio(espacio+1);
	mostrar_barra_vida_torre(resistencia_torre_1, VIDA_INICIAL_TORRE_1, vida_por_cuadrado);	
	mostrar_espacio(espacio + resto -2);
	mostrar_barra_vida_torre(resistencia_torre_2, VIDA_INICIAL_TORRE_2, vida_por_cuadrado);
	mostrar_espacio(espacio+1);

	printf("█\n" FIN);
}

/*Pre-condiciones: 
    "largo_total" positivo;
    "resistencia_torre_1" y "resistencia_torre_2" mayores o iguales a 0 y menores o iguales a VIDA_INICIAL_TORRE_1 y VIDA_INICIAL_TORRE_2 respectivamente.
Post-condiciones: 
    la función mostrará dos lineas con información de las torres.*/
void mostrar_lineas_torres(int largo_total, int resistencia_torre_1, int resistencia_torre_2){
	
	mostrar_linea_texto_torres(largo_total);
	mostrar_informacion_torres(largo_total, resistencia_torre_1, resistencia_torre_2);	
}

/*Pre-condiciones: 
    "largo_total" positivo;
    "tope_defensores" menor a MAX_DEFENSORES.
    "elfos_extra" y "enanos_extra" mayores o iguales a 0 y menores a ELFOS_EXTRA_INICIAL y ENANOS_EXTRA_INICIAL respectivamente.
Post-condiciones: 
    la función mostrará una linea con información de los defensores.*/
void mostrar_linea_defensores(int largo_total, int tope_defensores, int elfos_extra, int enanos_extra){
	char texto_1[] = "CANT. DEFENSORES: ", texto_2[] = "DISPONIBLES T1: ", texto_3[] = " - T2: ";
	int espacio, resto, texto_total;
	
	texto_total = cant_digitos(tope_defensores) + cant_digitos(enanos_extra) + cant_digitos(elfos_extra)
	+ (int)strlen(texto_1) + (int)strlen(texto_2) + (int)strlen(texto_3);
	espacio = (largo_total - texto_total)/3;
	resto = (largo_total - texto_total)%3;
	printf(MARGEN MARGEN MARGEN MARGEN MARGEN "█");
	mostrar_espacio(espacio);
	printf(BLANCO"%s%i", texto_1, tope_defensores);
	mostrar_espacio(espacio + resto);
	printf("%s%i%s%i", texto_2, enanos_extra, texto_3, elfos_extra);
	mostrar_espacio(espacio);
	printf("█\n"FIN);
}

/*Pre-condiciones: 
	"tope_enemigos" >= 0 y < MAX_ENEMIGOS;
Post-condiciones:
	la función devolverá un valor según los valores en enemigos[].*/
int cant_enemigos_vivos(int tope_enemigos, enemigo_t enemigos[MAX_ENEMIGOS]){
	int cont = 0;
	
	for (int i = 0; i < tope_enemigos; i++){

		if (enemigos[i].vida > 0)
			cont++;
	}

	return cont;
}

/*Pre-condiciones: 
    "largo_total" positivo;
    "tope_enemigos" menor o igual a max_enemigos_nivel, el cual es menor o igual a MAX_ENEMIGOS.
    "elfos_extra" y "enanos_extra" mayores o iguales a 0 y menores a ELFOS_EXTRA_INICIAL y ENANOS_EXTRA_INICIAL respectivamente.
Post-condiciones: 
    la función mostrará una linea con información de la vida de los enemigos.*/
void mostrar_informacion_enemigos(int largo_total, int tope_enemigos, enemigo_t enemigos[MAX_ENEMIGOS], int max_enemigos_nivel){
	int enemigos_vivos, texto_total, espacio, resto;
	char texto_1[] = "ORCOS TOTALES: ", texto_2[] = "/", texto_3[] = "ORCOS EN PANTALLA: ";

	enemigos_vivos = cant_enemigos_vivos(tope_enemigos, enemigos);
	
	texto_total = largo_total - (int)strlen(texto_1) - (int)strlen(texto_2) - (int)strlen(texto_3);
	texto_total -= (cant_digitos(tope_enemigos) + cant_digitos(max_enemigos_nivel) + cant_digitos(enemigos_vivos));
	espacio = texto_total/3;
	resto = texto_total%3;
	printf(MARGEN MARGEN MARGEN MARGEN MARGEN "█");
	mostrar_espacio(espacio);
	printf(BLANCO"%s%i%s%i"FIN , texto_1, tope_enemigos, texto_2, max_enemigos_nivel);
	mostrar_espacio(espacio + resto);
	printf(BLANCO"%s%i"FIN , texto_3, enemigos_vivos);
	mostrar_espacio(espacio);
	printf("█\n" FIN);
}

/*Pre-condiciones: 
    "largo_total" positivo;
    "tamanio_tablero" positivo y menor a MAX_FILAS Y MAX_COLUMNAS.
Post-condiciones: 
    la función mostrará una linea del cuadro con la enumeración de las columnas del tablero.*/
void mostrar_linea_columnas(int largo_total, int tamanio_tablero){
	int espacio, resto, texto_total;

	texto_total = largo_total - (tamanio_tablero*3);
	espacio = texto_total/2;
	resto = texto_total%2;
	printf(MARGEN MARGEN MARGEN MARGEN MARGEN "█");
	mostrar_espacio(espacio +1);
	mostrar_cant_columnas_cuadro(tamanio_tablero);
	mostrar_espacio(espacio + resto -1);
	printf("█\n"FIN);
}

/*Pre-condiciones: 
    "largo_total" positivo;    
    "tamanio_tablero" positivo y menor a MAX_FILAS Y MAX_COLUMNAS.
Post-condiciones: 
    la función mostrará la linea superior del tablero.*/
void mostrar_linea_superior_tablero(int largo_total, int tamanio_tablero){
	int texto_total, espacio, resto;
	 
	texto_total = largo_total - (tamanio_tablero*3);
	espacio = texto_total/2;
	resto = texto_total%2;
	
	printf(MARGEN MARGEN MARGEN MARGEN MARGEN "█");
	mostrar_espacio(espacio);
	printf("┌");
	for(int i = 0; i < tamanio_tablero*3; i++)
		printf("─");
	printf("┐");
	mostrar_espacio(espacio - 2 + resto);
	printf("█\n" FIN);
}

/*Pre-condiciones: 
    "largo_total" positivo;
    "tamanio_tablero" positivo y menor a MAX_FILAS Y MAX_COLUMNAS.
Post-condiciones: 
    la función mostrará el tablero del juego según los valores de la matriz.*/
void mostrar_filas(int largo_total, int tamanio_tablero, char matriz_camino[MAX_FILAS][MAX_COLUMNAS]){
	int espacio, texto_total, resto;
	char valor;

	for (int i = 0; i < tamanio_tablero; i++){
		
		texto_total = largo_total - (cant_digitos(i) + 2 + tamanio_tablero*3 + 1);
		espacio = texto_total/2;
		resto = texto_total%2;

		printf(MARGEN MARGEN MARGEN MARGEN MARGEN "█");
		mostrar_espacio(espacio);
		printf(BLANCO "%i │" FIN, i);
		
		for(int j = 0; j < tamanio_tablero; j++){

			valor = matriz_camino[i][j];
			switch(valor){
				case ENTRADA_1:
					printf(VERDE_OSCURO " E1"FIN);
					break;
				case ENTRADA_2:
					printf(VERDE_OSCURO " E2"FIN);
					break;
				case ENTRADA:
					printf(VERDE_OSCURO " E "FIN);
					break;	
				case TORRE_1:
					printf(VERDE_OSCURO " T1"FIN);
					break;
				case TORRE_2:
					printf(VERDE_OSCURO " T2"FIN);
					break;
				case CAMINO_1:
					printf(AMARILLO" ■ "FIN);
					break;
				case CAMINO_2:
					printf(SILVER" ■ "FIN);
					break;
				case DOS_CAMINOS:
					printf(GRIS" ■ "FIN);
					break;
				case NADA:
					printf(VERDE_CLARO"',´"FIN);
					break;
				case ELFO:
					printf(AZUL" %c "FIN, ELFO);
					break;
				case ENANO:
					printf(MAGENTA" %c "FIN, ENANO);
					break;
				case ENEMIGO_1_MUCHA_VIDA:
					printf(ROJO_CLARO" O "FIN);
					break;
				case ENEMIGO_1_MASO_VIDA:
					printf(ROJO_CLARO" o "FIN);
					break;
				case ENEMIGO_1_POCA_VIDA:
					printf(ROJO_CLARO" ° "FIN);
					break;
				case ENEMIGO_2_MUCHA_VIDA:
					printf(ROJO_OSCURO" O "FIN);
					break;
				case ENEMIGO_2_MASO_VIDA:
					printf(ROJO_OSCURO" o "FIN);
					break;
				case ENEMIGO_2_POCA_VIDA:
					printf(ROJO_OSCURO" ° "FIN);
					break;
				case DOS_ENEMIGOS_MUCHA_VIDA_MUCHA_VIDA:
					printf(ROJO_CLARO" O"ROJO_OSCURO"O"FIN);
					break;
				case DOS_ENEMIGOS_MUCHA_VIDA_MASO_VIDA:
					printf(ROJO_CLARO" O"ROJO_OSCURO"o"FIN);
					break;
				case DOS_ENEMIGOS_MUCHA_VIDA_POCA_VIDA:
					printf(ROJO_CLARO" O"ROJO_OSCURO"°"FIN);
					break;
				case DOS_ENEMIGOS_MASO_VIDA_MASO_VIDA:
					printf(ROJO_CLARO" o"ROJO_OSCURO"o"FIN);
					break;
				case DOS_ENEMIGOS_MASO_VIDA_POCA_VIDA:
					printf(ROJO_CLARO" o"ROJO_OSCURO"°"FIN);
					break;
				case DOS_ENEMIGOS_POCA_VIDA_POCA_VIDA:
					printf(ROJO_CLARO" °"ROJO_OSCURO"°"FIN);
					break;
			}
		}
		printf("│");
		mostrar_espacio(espacio + resto);
		printf("█\n" FIN);
	}

	printf(FIN);
}

/*Pre-condiciones: 
    "largo_total" positivo;
    "tamanio_tablero" positivo y menor a MAX_FILAS Y MAX_COLUMNAS.
Post-condiciones: 
    la función mostrará la linea inferior del tablero.*/
void mostrar_linea_inferior_tablero(int largo_total, int tamanio_tablero){
 	int texto_total, espacio, resto;

 	texto_total = (tamanio_tablero*3);
	espacio = (largo_total - texto_total)/2;
	resto = (largo_total - texto_total)%2;
	printf(MARGEN MARGEN MARGEN MARGEN MARGEN "█");
	mostrar_espacio(espacio);
	printf("└");
	for(int i = 0; i < tamanio_tablero*3; i++)
		printf("─");
	printf("┘");
	mostrar_espacio(espacio - 2 + resto);
	printf("█\n" FIN);
}

/*Pre-condiciones: 
    "largo_total" positivo.
Post-condiciones: 
    la función mostrará la linea inferior del cuadro.*/
void mostrar_linea_inferior_cuadro (int largo_total){

	printf(MARGEN MARGEN MARGEN MARGEN MARGEN "█");
	
	for(int i = 0; i < largo_total; i++)
		printf("▄");

	printf("█\n" FIN);
}

/*Pre-condiciones: 
	"juego.torres.resistencia_torre_1" y "juego.torres.resistencia_torre_2" son mayores o iguales a 0;
	"juego.nivel_actual" es 1, 2, 3 o 4;
	"juego.nivel.tope_enemigos" es menor igual a "juego.nivel.max_enemigos_nivel";
	"juego.nivel.tope_camino_1" es mayor a 0;
	"tamanio_tablero" positivo y menor a MAX_FILAS Y MAX_COLUMNAS.
Post-condiciones:
	la función mostrará el juego actual a partir de "juego"y "matriz_camino[][]".*/
void mostrar_parte_visual(juego_t juego, char matriz_camino[MAX_FILAS][MAX_COLUMNAS], int tamanio_tablero){
	int largo_total = calcular_largo_tablero(tamanio_tablero);
	
	mostrar_linea_superior_cuadro(largo_total);
	mostrar_linea_nivel(largo_total, juego.nivel_actual);
	mostrar_linea_vacia_cuadro(largo_total);
	mostrar_lineas_torres(largo_total, juego.torres.resistencia_torre_1, juego.torres.resistencia_torre_2);
	mostrar_linea_vacia_cuadro(largo_total);
	mostrar_linea_defensores(largo_total, juego.nivel.tope_defensores, juego.torres.elfos_extra, juego.torres.enanos_extra);
	mostrar_linea_vacia_cuadro(largo_total);
	mostrar_informacion_enemigos(largo_total, juego.nivel.tope_enemigos, juego.nivel.enemigos, juego.nivel.max_enemigos_nivel);
	mostrar_linea_vacia_cuadro(largo_total);
	mostrar_linea_columnas(largo_total, tamanio_tablero);
	mostrar_linea_superior_tablero(largo_total, tamanio_tablero);
	mostrar_filas(largo_total, tamanio_tablero, matriz_camino);
	mostrar_linea_inferior_tablero(largo_total, tamanio_tablero);
	mostrar_linea_inferior_cuadro(largo_total);	
	printf(FIN); 
}

/*Pre-condiciones: 
	"tope" positivo y menor a MAX_FILAS Y MAX_COLUMNAS.
	"entrada" es ENTRADA_1 o ENTRADA_2.
	"torre" es TORRE_1 o TORRE_2.
Post-condiciones:
	la función modificará matriz_camino[][] según camino[] por "entrada" y "torre" .*/
void agregar_entrada_torre_matriz (char matriz_camino[MAX_FILAS][MAX_COLUMNAS], coordenada_t camino [MAX_LONGITUD_CAMINO], int tope, char entrada, char torre){
	int fila_a_modificar, columna_a_modificar;

	fila_a_modificar = camino[0].fil;
	columna_a_modificar = camino[0].col;
	matriz_camino[fila_a_modificar][columna_a_modificar] = entrada;
		
	fila_a_modificar = camino[tope-1].fil;
	columna_a_modificar = camino[tope-1].col;
	matriz_camino[fila_a_modificar][columna_a_modificar] = torre;
}

/*Pre-condiciones: 
	"nivel" debe ser 1 o 2.
Post-condiciones:
	la función devolverá TORRE_NIVEL_1 o TORRE_NIVEL_2 según "nivel".*/
char torre_segun_nivel_un_cam(int nivel){
	char respuesta;

	if (nivel == 1)
		respuesta = TORRE_NIVEL_1;
	else if (nivel == 2)
		respuesta = TORRE_NIVEL_2;

	return respuesta;
}

//Pre y post en defendiendo_torres.h.
void mostrar_juego(juego_t juego){
	int tamanio_tablero;
	char matriz_camino[MAX_FILAS][MAX_COLUMNAS];

	tamanio_tablero = tamano_tablero_segun_nivel(juego.nivel_actual);
	inicializar_matriz(matriz_camino, tamanio_tablero, NADA);

	cambiar_matriz_segun_camino(matriz_camino, juego.nivel.camino_1, juego.nivel.tope_camino_1, CAMINO_1, false);
	
	if (juego.nivel.tope_camino_2 != 0){
	
		cambiar_matriz_segun_camino(matriz_camino, juego.nivel.camino_2, juego.nivel.tope_camino_2, CAMINO_2, true);
		agregar_entrada_torre_matriz(matriz_camino, juego.nivel.camino_1, juego.nivel.tope_camino_1, ENTRADA_1, TORRE_1);
		agregar_entrada_torre_matriz(matriz_camino, juego.nivel.camino_2, juego.nivel.tope_camino_2, ENTRADA_2, TORRE_2);
	}
	else
		agregar_entrada_torre_matriz(matriz_camino, juego.nivel.camino_1, juego.nivel.tope_camino_1, ENTRADA, torre_segun_nivel_un_cam(juego.nivel_actual));
	
	agregar_defensores_a_matriz(matriz_camino, juego.nivel);
	agregar_enemigos_a_matriz(matriz_camino, juego.nivel);
	mostrar_parte_visual(juego, matriz_camino, tamanio_tablero);
}

/*Pre-condiciones: 
	"fila" y "columna" deben ser válidas.
Post-condiciones:
	la función devolverá VERDADERO si hay alguna posicion de camino_1 o camino_2 con esa "fila" y "columna".*/
bool hay_camino(nivel_t nivel, int fila, int columna){
	bool hay = false;

	for (int i = 0; i < nivel.tope_camino_1; i++){
		
		if ( (fila == nivel.camino_1[i].fil) && (columna == nivel.camino_1[i].col) )
			hay = true;
	}

	if ( (!hay) && (nivel.tope_camino_2 != 0) ){
	
		for (int i = 0; i < nivel.tope_camino_2; i++){
			
			if ( (fila == nivel.camino_2[i].fil) && (columna == nivel.camino_2[i].col) )
				hay = true;
		}
	}

	return hay;
}

/*Pre-condiciones: 
	"fila" y "columna" deben ser válidas;
	"nivel.tope_defensores" debe ser positivo.
Post-condiciones:
	la función devolverá VERDADERO si hay alguna posicion de nivel.defensores[] con esa "fila" y "columna".*/
bool ya_hay_defensor(nivel_t nivel, int fila, int columna){
	bool hay = false;

	for (int i = 0; i < nivel.tope_defensores; i++){

		if ( (nivel.defensores[i].posicion.fil == fila) && (nivel.defensores[i].posicion.col == columna) )
			hay = true;
	}

	return hay;
}

/*Pre-condiciones: 
	"fila" y "columna" deben ser válidas;
	"nivel.tope_defensores" debe ser positivo.
Post-condiciones:
	la función devolverá VERDADERO si hay camino o defensor en esa "fila" y "columna".*/
bool defensor_invalido(nivel_t nivel, int fila, int columna){
	bool invalido = false;
	
	if ( hay_camino(nivel, fila, columna) || ya_hay_defensor(nivel, fila, columna) )
		invalido = true;
	else
		invalido = false;

	return invalido;
}

/*Pre-condiciones: 
	"tipo" debe ser ELFO o ENANO;
Post-condiciones:
	la función devolverá DANIO_NORMAL_ELFO o DANIO_NORMAL_ENANO.*/
int fuerza_ataque_segun_tipo(char tipo){
	int fuerza;

	if (tipo == ELFO)
		fuerza = DANIO_NORMAL_ELFO;
	else
		fuerza = DANIO_NORMAL_ENANO;

	return fuerza;
}

//Pre y post en defendiendo_torres.h
int agregar_defensor(nivel_t* nivel, coordenada_t posicion, char tipo){
	int estado = POSIBLE;

	if ( defensor_invalido(*nivel, posicion.fil, posicion.col) )
		estado = NO_POSIBLE;
	else{

		nivel->defensores[nivel->tope_defensores].tipo = tipo;
		nivel->defensores[nivel->tope_defensores].fuerza_ataque = fuerza_ataque_segun_tipo(tipo);
		nivel->defensores[nivel->tope_defensores].posicion = posicion;
		nivel->tope_defensores++;
	}

	return estado;
}

/*Pre-condiciones: 
	"fila_defensor", "fila_enemigo", "columna_defensor" y "columna_enemigo" son válidas;
Post-condiciones:
	la función retornará VERDADERO según los parámetros recibidos.*/
bool posiciones_validas_enemigo_segun_enano(int fila_defensor, int columna_defensor, int fila_enemigo, int columna_enemigo){

	return (abs(fila_defensor - fila_enemigo) <= DISTANCIA_ATAQUE_ENANO && abs(columna_defensor - columna_enemigo) <= DISTANCIA_ATAQUE_ENANO);
}

/*Pre-condiciones: 
	"vida_enemigo" debe ser positiva;
    "porcentaje_fallo", "porcentaje_critico" deben ser >= 0 y < 100.
Post-condiciones:
	la función modificara los valores de las posiciones recibidas a partir de los parámetros recibidos.*/
void efectuar_ataque(int *vida_enemigo, int porcentaje_fallo, int porcentaje_critico, int danio_ataque_normal, int *cont_criticos, int *cont_fallos){
	int valor_random_para_fallo, valor_random_para_critico;

	valor_random_para_fallo = rand()%100;
	valor_random_para_critico = rand()%100;

	if (valor_random_para_fallo < porcentaje_fallo){
		
		*vida_enemigo -= DANIO_FALLIDO_ELFO;
		(*cont_fallos)++;
	} else{
		
		*vida_enemigo -= danio_ataque_normal;
		if(valor_random_para_critico < porcentaje_critico){

			*vida_enemigo -= DANIO_CRITICO_ELFO;
			(*cont_criticos)++;
		}
	}
}

/*Pre-condiciones: 
	"nivel" debe ser una estructura con sus valores válidos;
	"porcentaje_fallo", "porcentaje_critico" deben ser >= 0 y < 100;
	"pos_defensor" >= 0 y < MAX_DEFENSORES.
Post-condiciones:
	la función modificara los valores de las posiciones recibidas a partir de los parámetros recibidos.*/
void atacar_siendo_enano(nivel_t *nivel, int porcentaje_fallo, int porcentaje_critico, int pos_defensor, int *cont_criticos, int *cont_fallos){
	int orcos_posibles_para_atacar[nivel->tope_enemigos];
	coordenada_t posicion_defensor, posicion_enemigo;
	int cont = -1, pos_orco_a_atacar;
	
	posicion_defensor = nivel->defensores[pos_defensor].posicion;

	for (int j = 0; j < (nivel->tope_enemigos); j++){
		
		if (nivel->enemigos[j].camino == 1)
			posicion_enemigo = nivel->camino_1[nivel->enemigos[j].pos_en_camino];
		else
			posicion_enemigo = nivel->camino_2[nivel->enemigos[j].pos_en_camino];
		
		if ( posiciones_validas_enemigo_segun_enano(posicion_defensor.fil, posicion_defensor.col, posicion_enemigo.fil, posicion_enemigo.col)
		 && ( nivel->enemigos[j].vida > 0) ){
			
			cont++;
			orcos_posibles_para_atacar[cont] = j;
		}
	}

	if (cont >= 0) {

		pos_orco_a_atacar = rand()%(cont+1);
		efectuar_ataque(&(nivel->enemigos[orcos_posibles_para_atacar[pos_orco_a_atacar]].vida), porcentaje_fallo, porcentaje_critico,
			nivel->defensores[pos_defensor].fuerza_ataque, cont_criticos, cont_fallos);
	}
}

/*Pre-condiciones: 
	"posicion_defensor" y "posicion_enemigo" debes ser posiciones válidas;
Post-condiciones:
	la función devolverá un valor según los parámetros recibidos.*/
int distancia_manhattan(coordenada_t posicion_defensor, coordenada_t posicion_enemigo){

	return (abs(posicion_defensor.fil - posicion_enemigo.fil) + abs(posicion_defensor.col - posicion_enemigo.col));
}

/*Pre-condiciones: 
	"nivel" debe ser una estructura con sus valores válidos;
	"porcentaje_fallo", "porcentaje_critico" deben ser >= 0 y < 100;
	"pos_defensor" >= 0 y < MAX_DEFENSORES.
Post-condiciones:
	la función modificara los valores de las posiciones recibidas a partir de los parámetros recibidos.*/
void atacar_siendo_elfo(nivel_t *nivel, int porcentaje_fallo, int porcentaje_critico, int pos_defensor, int *cont_criticos, int *cont_fallos){
	coordenada_t posicion_defensor, posicion_enemigo;
	
	posicion_defensor = nivel->defensores[pos_defensor].posicion;
	
	for (int j = 0; j < nivel->tope_enemigos; j++){
		
		if (nivel->enemigos[j].camino == 1)
			posicion_enemigo = nivel->camino_1[nivel->enemigos[j].pos_en_camino];
		else
			posicion_enemigo = nivel->camino_2[nivel->enemigos[j].pos_en_camino];

		if ( ( distancia_manhattan(posicion_defensor, posicion_enemigo) <= DISTANCIA_ATAQUE_ELFO ) && ( nivel->enemigos[j].vida > 0 )){

			efectuar_ataque(&(nivel->enemigos[j].vida), porcentaje_fallo, porcentaje_critico,
				nivel->defensores[pos_defensor].fuerza_ataque, cont_criticos, cont_fallos);
		}
	}
}

/*Pre-condiciones: 
	"vida_enemigo" > 0.
Post-condiciones:
	la función modificara el valor en "vida_torre" por un valor >= 0 segun "vida_enemigo".*/
void restar_vida_a_torre(int *vida_torre, int vida_enemigo){

	*vida_torre -= vida_enemigo;
	if (*vida_torre < 0)
		*vida_torre = 0;
}

/*Pre-condiciones: 
	"*vida_enemigo" > 0.
	"camino" debe ser 1 o 2.
Post-condiciones:
	la función modificara el valor en las posiciones recibidas segun "camino" y cambiará el valor en la posicion "vida_enemigo" a 0.*/
void sacrificar_orco_por_torre(torres_t *torres, int *vida_enemigo, int camino){

	if(camino == 1)
		restar_vida_a_torre(&(torres->resistencia_torre_1), *vida_enemigo);
	else
		restar_vida_a_torre(&(torres->resistencia_torre_2), *vida_enemigo);

	*vida_enemigo = 0;
}

/*Pre-condiciones: 
	"numero_camino" debe ser 1 o 2.
	"*tope" positivo y menor a MAX_ENEMIGOS.
Post-condiciones:
	la función modificará "enemigo" con y a partir de los datos recibidos, además de incrementar el valor de "tope".*/
void agregar_enemigo(enemigo_t *enemigo, int *tope, int numero_camino){
	
	enemigo->pos_en_camino = 0;
	enemigo->camino = numero_camino;
	enemigo->vida = VIDA_INICIAL_ORCO + rand()%VIDA_RANDOM_ORCO_MAX;
	(*tope)++;
}

/*Pre-condiciones: 
	"enemigo->pos_en_camino" debe ser < tope;
	"enemigo->vida" > 0;
	"tope" < MAX_ENEMIGOS.
Post-condiciones:
	la función modificara los valores de las posiciones recibidas a partir de los parámetros recibidos.*/
void determinar_movimiento_enemigo(enemigo_t *enemigo, torres_t *torres, int tope){

	if (enemigo->pos_en_camino + 2 < tope)
		enemigo->pos_en_camino++;
	else
		sacrificar_orco_por_torre(torres, &(enemigo->vida), enemigo->camino);
}

/*Pre-condiciones: 
	"tope_enemigos" < MAX_ENEMIGOS.
	"nivel" y "torres" deben ser una estructura con valores válidos.
Post-condiciones:
	la función modificara los valores de las posiciones recibidas a partir de los parámetros recibidos.*/
void avanzar_enemigos(int tope_enemigos, nivel_t *nivel, torres_t *torres){

	for (int i = 0; i < tope_enemigos; i++){

		if (nivel->enemigos[i].vida > 0){

			if (nivel->enemigos[i].camino == 1)
				determinar_movimiento_enemigo(&(nivel->enemigos[i]), torres, nivel->tope_camino_1);
			else
				determinar_movimiento_enemigo(&(nivel->enemigos[i]), torres, nivel->tope_camino_2);
		}
	}
}

/*Pre-condiciones: 
	"nivel" debe ser una estructura con sus valores válidos;
	"prob_fallo_gimli", "prob_fallo_gimli", "prob_critico_gimli", "prob_critico_legolas" deben ser >= 0 y < 100.
Post-condiciones:
	la función modificara los valores de las posiciones recibidas a partir de los parámetros recibidos y mostrará información sobre el ataque*/
void atacar_con_defensores(nivel_t *nivel, int prob_fallo_gimli, int prob_critico_gimli, int prob_fallo_legolas, int prob_critico_legolas){
	int cont_criticos= 0, cont_fallos= 0;
	
	for(int i = 0; i < nivel->tope_defensores; i++){
		
		if (nivel->defensores[i].tipo == ENANO)
			atacar_siendo_enano(nivel, prob_fallo_gimli, prob_critico_gimli, i, &cont_criticos, &cont_fallos);
		else
			atacar_siendo_elfo(nivel, prob_fallo_legolas, prob_critico_legolas, i, &cont_criticos, &cont_fallos);
	}

	printf(MARGEN MARGEN MARGEN MARGEN MARGEN BLANCO"  GOLPES CRITICOS: %i   ", cont_criticos);
	printf("GOLPES FALLIDOS: %i   "FIN, cont_fallos);
}

/*Pre-condiciones: 
	"nivel" debe ser 1, 2, 3 o 4.
Post-condiciones:
	la función devolverá CANT_CAMINOS_NIVEL_1, CANT_CAMINOS_NIVEL_2, CANT_CAMINOS_NIVEL_3 o CANT_CAMINOS_NIVEL_4, que serán valores enteros entre 1 y 2*/
int cant_caminos_segun_nivel(int nivel){
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

// Pre y post en defendiendo_torres.h.
void jugar_turno(juego_t* juego){
	int vivos_inicial, vivos_final; 
	
	vivos_inicial = cant_enemigos_vivos(juego->nivel.tope_enemigos, juego->nivel.enemigos); 
	atacar_con_defensores(&(juego->nivel), juego->fallo_gimli, juego->critico_gimli, juego->fallo_legolas, juego->critico_legolas);
	vivos_final = cant_enemigos_vivos(juego->nivel.tope_enemigos, juego->nivel.enemigos); 
	printf(BLANCO"GOLPES MORTALES: %i  \n"FIN, (vivos_inicial - vivos_final));

	avanzar_enemigos(juego->nivel.tope_enemigos, &(juego->nivel), &(juego->torres));

	if (juego->nivel.tope_enemigos < juego->nivel.max_enemigos_nivel){

		agregar_enemigo(&(juego->nivel.enemigos[juego->nivel.tope_enemigos]), &(juego->nivel.tope_enemigos), 1);
		if (cant_caminos_segun_nivel(juego->nivel_actual) == 2)
			agregar_enemigo(&(juego->nivel.enemigos[juego->nivel.tope_enemigos]), &(juego->nivel.tope_enemigos), 2);
	}
}

//Pre y post en defendiendo_torres.h
int estado_nivel(nivel_t nivel){
	int i = 0, estado = GANADO;

	if (nivel.tope_enemigos == nivel.max_enemigos_nivel){

		while ((i < nivel.tope_enemigos) && (estado == GANADO)){
	
			if (nivel.enemigos[i].vida > 0)
				estado = JUGANDO;
			i++;
		}
	}
	else 
		estado = JUGANDO;

	return estado;
}

/*Pre-condiciones: 
	.
Post-condiciones:
	la función devolverá VERDADERO si alguna de las torres tiene vida 0.*/
bool torres_derribadas(torres_t torres){

	return (torres.resistencia_torre_1 == 0 || torres.resistencia_torre_2 == 0);
}

//Pre y post en defendiendo_torres.h
int estado_juego(juego_t juego){
	int estado = JUGANDO;

	if ( (juego.nivel_actual == ULTIMO_NIVEL) && ( estado_nivel(juego.nivel) == GANADO ) )
		estado = GANADO;
	else if ( torres_derribadas(juego.torres) )
		estado = PERDIDO;

	return estado;
}
