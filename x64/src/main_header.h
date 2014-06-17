

const float FPS = 60;
const int SCREEN_W = 510;
const int SCREEN_H = 462;
const int MAX_ENEMY_MISSILES = 8;

extern bool city_status[6];
extern float turret[3];
extern float lineX,lineY;
extern unsigned int score;

struct explosions
{
/** Posizione dell'esplosione */
    float x0;
/** Posizione dell'esplosione */
    float y0;
/** Raggio del cerchio esterno */
    float radius;
/** Raggio del cerchio minore interno */
    float internal_radius;
/** Durata dell'esplosione, determinata in explosion_logic */
    int duration;
/** Variabile che controlla l'esistenza o menu di un'esplosione legata al missile */
    bool flash;
/** Variabile settata a false dopo il primo ciclo dell'esplosione */
    bool flash_init;
};

struct missiles
{
/** Parte delle coordinate attuali del missile ad ogni ciclo */
    float x0;
/** Parte delle coordinate dell'obiettivo del missile */
    float x1;
/** Parte delle coordinate attuali del missile ad ogni ciclo */
    float y0;
/** Parte delle coordinate dell'obiettivo del missile */
    float y1;
/** Parte delle coordinate del punto di partenza del missile */
    float spawn_x;
/** Parte delle coordinate del punto di partenza del missile */
    float spawn_y;
/** Variabile necessaria per il calcolo della traiettoria pixel per pixel */
    float dx;
/** Variabile necessaria per il calcolo della traiettoria pixel per pixel */
    float dy;
/** Variabile necessaria per il calcolo della traiettoria pixel per pixel */
    float sx;
/** Variabile necessaria per il calcolo della traiettoria pixel per pixel */
    float sy;
/** Variabile necessaria per il calcolo della traiettoria pixel per pixel */
    float err;
/** Variabile necessaria per il calcolo della traiettoria pixel per pixel */
    float e2;
/** Timer che determina in quale ciclo ogni missile nemico debba essere lanciato */
    int timer;
/** Valore da 1 a 9 dell'obiettivo del missile nemico */
    int objective;
/** Variabile che controlla se il punteggio e la torre extra sono già state assegnate per il missile nemico */
    bool bounty;
/** Variabile che controlla se è già stato assegnato un timer al missile nemico */
    bool prep;
/** Variabile che controlla se il missile è già stato lanciato */
    bool launched;
/** Variabile che controlla se la traiettoria del missile è già stata inizializzata (vedere funzione advance_by_one) */
    bool bullet_init;
/** variabile di tipo explosions assegnata ad ogni missile */
    explosions explosion;
};

extern missiles playerm;
extern missiles enemym[MAX_ENEMY_MISSILES];

missiles player_missile_init(missiles &pl_missile);

missiles enemy_missile_init(missiles &en_missile);

missiles explosion_logic (missiles &missile);

void destroy_city(missiles &en_missile);

void enemy_missile_logic();

void player_missile_logic();


//** Questa funzione si occupa di riportare le variabili principali ai valori di default nel caso in cui il giocatore decida di ricominciare la partita dopo un game over senza riavviare il programma */
void cleanup();


