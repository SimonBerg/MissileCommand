#include <allegro5/allegro.h>

/** Variabile collegata all'immagine bitmap delle città */
extern ALLEGRO_BITMAP*      city_b;
/** Variabile collegata all'immagine bitmap delle torri */
extern ALLEGRO_BITMAP*      turret_b;

extern bool city_status[6];

void create_crosshair();

void draw_turret(float x, int n, ALLEGRO_BITMAP* turret_b);

void draw_cities(ALLEGRO_BITMAP* city_b);
