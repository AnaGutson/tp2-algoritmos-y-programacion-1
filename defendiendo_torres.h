#ifndef __DEFENDIENDO_TORRES_H__
#define __DEFENDIENDO_TORRES_H__

#define MAX_FILAS 30
#define MAX_COLUMNAS 30
#define MAX_LONGITUD_CAMINO 200
#define MAX_ENEMIGOS 500
#define MAX_DEFENSORES 50

typedef struct coordenada {
	int fil;
	int col;
} coordenada_t;

typedef struct defensor {
	char tipo;
	int fuerza_ataque;
	coordenada_t posicion;
} defensor_t;

typedef struct torres {
	int resistencia_torre_1;
	int resistencia_torre_2;
	int enanos_extra;
	int elfos_extra;
} torres_t;

typedef struct enemigo {
	int camino;
	int pos_en_camino;
	int vida;
} enemigo_t;

typedef struct nivel {
	coordenada_t camino_1[MAX_LONGITUD_CAMINO];
	int tope_camino_1;
	
	coordenada_t camino_2[MAX_LONGITUD_CAMINO];
	int tope_camino_2;

	defensor_t defensores[MAX_DEFENSORES];
	int tope_defensores;

	enemigo_t enemigos[MAX_ENEMIGOS];
	int tope_enemigos;

	int max_enemigos_nivel;
} nivel_t;

typedef struct juego {
	nivel_t nivel;
	torres_t torres;
	int nivel_actual;
	int critico_legolas;
	int critico_gimli;
	int fallo_legolas;
	int fallo_gimli;
} juego_t;


/*
 * Recibe un juego con todas sus estructuras válidas. 
 * El juego se dará por ganado si el juego está en el ultimo nivel y éste ha sido terminado.
 * El juego se dará por perdido, si alguna de las torres llega a 0 en su resistencia.
 * Devolverá:
 * >  0 si el estado es jugando.
 * > -1 si el estado es perdido.
 * >  1 si el estado es ganado.
 */
/*Pre-condiciones: 
	"juego.nivel.max_enemigos_nivel" debe ser válido según juego.nivel_actual;
Post-condiciones:
	la función devolverá JUGANDO, GANADO o PERDIDO según el parámetro recibido.*/
int estado_juego(juego_t juego);

/*
 * Recibe un nivel con todas sus estructuras válidas. 
 * El nivel se dará por ganado cuando estén TODOS los orcos de ese 
 * nivel muertos (esto es, con vida menor o igual a 0).
 * Devolverá:
 * >  0 si el estado es jugando.
 * >  1 si el estado es ganado.
 */
/*Pre-condiciones: 
	"nivel.max_enemigos_nivel" debe ser válido según nivel_actual.
Post-condiciones:
	la función devolverá JUGANDO o GANADO según el parametro recibido.*/
int estado_nivel(nivel_t nivel);

/*
 * Jugará un turno y dejará el juego en el estado correspondiente.
 * Harán su jugada enanos, elfos y orcos en ese orden.
 */
/*Pre-condiciones: 
	"juego" debe ser una estructura con valores válidos:
		"juego.nivel_actual" debe ser 1, 2, 3 o 4.
		"juego.max_enemigos_nivel" positivo y < MAX_ENEMIGOS.
		"juego->fallo_gimli", "juego->critico_gimli", "juego->fallo_legolas", "juego->critico_legolas" deben ser >= 0 y <= 100;
		"juego->torres" tiene que tener la resistencia de ambas torres > 0.
Post-condiciones:
	la función modificará "juego" según la jugada realizada (depende del azar mayormente) y mostrará información del turno jugado*/
void jugar_turno(juego_t* juego);

/*
 * Inicializará el juego, cargando la informacion de las torres y
 * los ataques críticos y fallo de Legolas y Gimli.
 * NO inicializará el primer nivel.
 */
/*Pre-condiciones: 
	"viento" debe ser MUCHO_VIENTO, VIENTO_REGULAR o POCO_VIENTO;
    "humedad" debe ser MUCHA_HUMEDAD, HUMEDAD_REGULAR o POCA_HUMEDAD;
    "animo_legolas" y "animo_gimli" deben ser BUENO, MALO o REGULAR.
Post-condiciones:
	la función modificara los valores de las posiciones recibidas a partir de los parámetros "viento", "humedad", "animo_gimli" y "animo_legolas".*/
void inicializar_juego(juego_t* juego, int viento, int humedad, char animo_legolas, char animo_gimli);

/* 
 * Agregará un defensor en el nivel recibido como parametro.
 * Devolverá:
 * >  0 si pudo agregar el defensor correctamente.
 * > -1 si no pudo (la coordenada es parte del camino de ese nivel, 
 * existe otro defensor, etc.)
 */
/*Pre-condiciones: 
	"posicion" debe ser válida;
    "tipo" debe ser ENANO o ELFO;
Post-condiciones:
	la función modificara los valores de las posiciones recibidas.*/
int agregar_defensor(nivel_t* nivel, coordenada_t posicion, char tipo);

/* 
 * Mostrará el mapa dependiendo del nivel en que se encuentra el jugador.
 */
/*Pre-condiciones: 
	"juego.torres.resistencia_torre_1" y "juego.torres.resistencia_torre_2" son mayores o iguales a 0;
	"juego.nivel_actual" es 1, 2, 3 o 4;
	"juego.nivel.tope_enemigos" es menor igual a "juego.nivel.max_enemigos_nivel";
	"juego.nivel.tope_camino_1" es mayor a 0.
Post-condiciones:
	la función mostrará el juego actual a partir de "juego".*/
void mostrar_juego(juego_t juego);

#endif /* __DEFENDIENDO_TORRES_H__ */
