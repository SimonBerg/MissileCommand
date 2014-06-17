#include "main_header.h"
#include "drawing.h"
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

using namespace std;

#define DEBUG_DRAW(a) {}

/** Funzione che viene chiamata ad ogni aggiornamento del display per creare il cursore "mirino" durante il gioco. Crea due linee tramite funzioni di disegno "primitivo" di Allegro basandosi sulla posizione del cursore, salvata nelle variabili lineX e lineY */

void create_crosshair(){

    al_draw_line(lineX-10,lineY,lineX+10,lineY,al_map_rgb(0,0,255),2);
    al_draw_line(lineX,lineY-10,lineX,lineY+10,al_map_rgb(0,0,255),2);

}

/** Funzione che prende in ingresso la posizione (x) ed il numero (n) di una base, e che ad ogni ciclo del programma disegna le torri singolarmente in base al numero di esse rimanente nella base in questione (da 0 a 8). */

void draw_turret(float x, int n, ALLEGRO_BITMAP* turret_b) {

    float y = 413;

    if (turret[n] >= 1)
        al_draw_bitmap(turret_b, x, y, 0);

    if (turret[n] >= 2)
        al_draw_bitmap(turret_b, x-6, y+4, 0);

    if (turret[n] >= 3)
        al_draw_bitmap(turret_b, x+6, y+4, 0);

    if (turret[n] >= 4)
        al_draw_bitmap(turret_b, x-12, y+10, 0);

    if (turret[n] >= 5)
        al_draw_bitmap(turret_b, x, y+10, 0);

    if (turret[n] >= 6)
        al_draw_bitmap(turret_b, x+12, y+10, 0);

    if (turret[n] >= 7)
        al_draw_bitmap(turret_b, x-6, y+16, 0);

    if (turret[n] == 8)
        al_draw_bitmap(turret_b, x+6, y+16, 0);

}

/** Funzione che ad ogni ciclo del programma controlla lo stato di ogni città, disegnando quelle settate su true, e creando uno spazio vuoto al posto di quelle settate su false */
void draw_cities(ALLEGRO_BITMAP* city_b) {

    if (city_status[0])
        al_draw_bitmap(city_b,87, 428, 0);
        else
            al_draw_filled_circle(102, 428, 20, al_map_rgb(0,0,0));

    if (city_status[1])
        al_draw_bitmap(city_b,132, 428, 0);
        else
            al_draw_filled_circle(147, 428, 20, al_map_rgb(0,0,0));

    if (city_status[2])
        al_draw_bitmap(city_b,177, 428, 0);
        else
            al_draw_filled_circle(192, 428, 20, al_map_rgb(0,0,0));

    if (city_status[3])
        al_draw_bitmap(city_b,295, 428, 0);
        else
            al_draw_filled_circle(310, 428, 20, al_map_rgb(0,0,0));

    if (city_status[4])
        al_draw_bitmap(city_b,349, 428, 0);
        else
            al_draw_filled_circle(364, 428, 20, al_map_rgb(0,0,0));

    if (city_status[5])
        al_draw_bitmap(city_b,400, 428, 0);
        else
            al_draw_filled_circle(415, 428, 20, al_map_rgb(0,0,0));
}

