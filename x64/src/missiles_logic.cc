#include <math.h>
#include "main_header.h"
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

using namespace std;

#define DEBUG_MIS(a) {}

/** Funzione che prende in ingresso una variabile di tipo missiles generica (del giocatore o del nemico), e che tramite l'algoritmo della linea di Bresenham calcola la traiettoria che il missile dovrà compiere pixel per pixel. La prima parte della funzione viene chiamata solamente una volta per lancio, mentre la seconda parte verrà chiamata per il missile in questione ad ogni ciclo fino a quando non avrà raggiunto il suo obiettivo. */

missiles advance_by_one(missiles &missile){

    if (missile.bullet_init == false) {
        missile.dx = abs(missile.x1-missile.x0);
        missile.dy = abs(missile.y1-missile.y0);

    if (missile.x0 < missile.x1)
        missile.sx = 1;
    else
        missile.sx = -1;

    if (missile.y0 < missile.y1)
        missile.sy = 1 ;
    else
        missile.sy = -1;

    missile.err = missile.dx-missile.dy;

    missile.bullet_init = true;

    }

    missile.e2 = 2*missile.err;

    if (missile.e2 > -missile.dy) {
        missile.err = missile.err - missile.dy;
        missile.x0 = missile.x0 + missile.sx;
     }

    if (missile.e2 < missile.dx) {
        missile.err = missile.err +missile.dx;
        missile.y0 = missile.y0 + missile.sy ;
     }

     return missile;
}

/** Funzione che si occupa dell'inizializzazione delle coordinate dei missili del giocatore. imposta il punto di partenza e di fine del missile lanciato dal giocatore. Essa imposta come punto d'arrivo del missile la posizione del mouse all'atto del clic, salvata in lineX e lineY, mentre come punto di partenza la base più vicina al clic che dispone almeno di una torre */

missiles player_missile_init(missiles &pl_missile){

    playerm.bullet_init = false;
    playerm.y0 = pl_missile.spawn_y = 412;
    playerm.x1 = lineX;
    playerm.y1 = lineY;

    if ((lineX <= 170 && turret[0] >= 1) || ((turret[1] <= 0 && turret[0] >= 1) && (lineX < 254))
                                         || ((turret[1] <= 0 && turret[0] >= 1) && (turret[2] <= 0))){

		DEBUG_MIS(cout<<"Turret 1"<<endl) ;
        pl_missile.x0 = pl_missile.spawn_x = 41;
        turret[0]--;
        pl_missile.launched = true; }

    if (((lineX < 340 && lineX > 170) && (turret[1] >= 1))|| ((turret[2] <= 0 && turret[1] >= 1) && (lineX > 170))
                                                         || ((turret[0] <= 0 && turret[1] >= 1) && (lineX < 340))
                                                         || ((turret[0] <= 0 && turret[1] >= 1) && (turret[2] <= 0))) {

        if (pl_missile.launched == false) {

			DEBUG_MIS(cout<<"Turret 2"<<endl);
            pl_missile.x0 = pl_missile.spawn_x = 248;
            turret[1]--;
            pl_missile.launched = true; } }

    if ((lineX >= 340 && turret[2] >= 1) || ((turret[1] <= 0 && turret[2] >= 1) && (lineX >= 255))
                                             || ((turret[1] <= 0 && turret[2] >= 1) && (turret[0] <= 0))) {

        if (pl_missile.launched == false) {

			DEBUG_MIS(cout<<"Turret 3"<<endl);
			pl_missile.x0 = pl_missile.spawn_x = 483;
			turret[2]--;
			pl_missile.launched = true; } }

    pl_missile = advance_by_one(pl_missile);

    return pl_missile;
}

/** Funzione che si occupa dell'inizializzazione delle coordinate dei missili nemici. Mentre i valori y dei punti di partenza e di arrivo sono statici, le x vengono calcolate casualmente. Nel caso del punto di partenza, la funzione rand() sceglie un pixel da 1 a 510 (larghezza del display), mentre per quanto riguarda il punto di arrivo viene scelto un obiettivo da 1 a 9. I valori da 1 a 6 rappresentano le città, mentre quelli da 7 a 9 rappresentano le tre basi. */

missiles enemy_missile_init(missiles &en_missile){

    int city;
    en_missile.bullet_init = false;
    en_missile.y0 = en_missile.spawn_y = 20;
    en_missile.x0 = en_missile.spawn_x = rand() % 510 + 1;
    en_missile.y1 = 428;
    city = rand() % 9 + 1;

    en_missile.objective = city;

    switch (city) {

        case 1:
            en_missile.x1 = 102;
            break;

        case 2:
            en_missile.x1 = 147;
            break;

        case 3:
            en_missile.x1 = 192;
            break;

        case 4:
            en_missile.x1 = 310;
            break;

        case 5:
            en_missile.x1 = 364;
            break;

        case 6:
            en_missile.x1 = 415;
            break;

        case 7: {
            en_missile.x1 = 41;
            en_missile.y1 = 413;
            break; }

        case 8: {
            en_missile.x1 = 248;
            en_missile.y1 = 413;
            break; }

        case 9: {
            en_missile.x1 = 483;
            en_missile.y1 = 413;
            break; }

        default:
            break;

        }

    return en_missile;

    }

/** Funzione che iniializza e gestisce le esplosioni causate dai missili dopo aver raggiunto il loro obiettivo. Come prima cosa controlla se vi è un esplosione provocata nel ciclo precedente che è entrata in collisione con un missile nemico (e in tal caso azzera i valori del missile, assegnando poi i punti ed una torre extra al giocatore), e poi disegna le eventuali esplosioni che non hanno ancora esaurito la loro durata. */

missiles explosion_logic (missiles &missile) {

    for (int i = 0; i < MAX_ENEMY_MISSILES; i++) {

        if (((enemym[i].x0<= playerm.explosion.x0+15) &&
             (enemym[i].y0<= playerm.explosion.y0+15)) && ((enemym[i].x0 > playerm.explosion.x0-15)&&
             (enemym[i].y0 > playerm.explosion.y0-15))) {

        if (enemym[i].bounty == false && enemym[i].launched) {
            score+= 100;
            enemym[i].bounty = true;

            if (turret[0] < 8 && playerm.spawn_x == 41)
            turret[0]++;

            if (turret[1] < 8 && playerm.spawn_x == 248)
            turret[1]++;

            if (turret[2] < 8 && playerm.spawn_x == 483)
            turret[2]++;
        }

   if (enemym[i].launched)
	enemym[i].prep = false;

        enemym[i].launched = false;
        enemym[i].bullet_init = false;

        }
        }

    if (missile.explosion.flash_init == false) {
        missile.explosion.x0 = missile.x1;
        missile.explosion.y0 = missile.y1;
        missile.explosion.radius = 15;
        missile.explosion.internal_radius = 0;
        missile.explosion.flash_init = true;
    }

    if (missile.explosion.duration % 2 == 0) {
        al_draw_filled_circle(missile.explosion.x0, missile.explosion.y0,
                              missile.explosion.radius, al_map_rgb(0, 255, 255));
        }

    if (missile.explosion.duration %2 != 0) {
        al_draw_filled_circle(missile.explosion.x0, missile.explosion.y0,
                              missile.explosion.radius, al_map_rgb(210, 210, 180));

        }

    al_draw_filled_circle(missile.explosion.x0, missile.explosion.y0,
                          missile.explosion.internal_radius, al_map_rgb(0, 0, 0));

    if (missile.explosion.internal_radius < 15 && missile.explosion.duration < 60)
                                              missile.explosion.internal_radius++;

    missile.explosion.duration++;

    if (missile.explosion.duration >= 30) {
        missile.explosion.flash = false;
    }

        return missile;
    }

/** Questa funzione viene chiamata ogni volta che un missile nemico raggiunge il suo obiettivo. Prima controlla l'obiettivo del nemico, e nel caso di una delle 6 città la setta a "false", mentre nel caso di una base, diminuisce le torri disponibili in quella base di 2 */

void destroy_city(missiles &en_missile) {

    switch (en_missile.objective) {

        case 1:
        city_status[0] = false;
        break;

        case 2:
        city_status[1] = false;
        break;

        case 3:
        city_status[2] = false;
        break;

        case 4:
        city_status[3] = false;
        break;

        case 5:
        city_status[4] = false;
        break;

        case 6:
        city_status[5] = false;
        break;

        case 7:
        turret[0] -= 2;
        break;

        case 8:
        turret[1] -= 2;
        break;

        case 9:
        turret[2] -= 2;
        break;

        default:
        break;
    }
}

/** Questa funzione viene chiamata ad ogni ciclo del gioco e si occupa prima dell'inizializzazione di ogni missile, del disegno pixel per pixel e infine della chiamata alla funzione destroy_city ed explosion_logic nel caso in cui un missile abbia raggiunto il suo obiettivo. Prima del lancio, ad ogni missile nemico è assegnato casualmente un timer che va da 1 a 250 cicli, che determinerà quando verrà lanciato */

void enemy_missile_logic() {

    for (int i = 0; i < MAX_ENEMY_MISSILES; i++) {

            if (enemym[i].prep == false) {
                enemym[i].timer = rand() % 250 + 1;
				DEBUG_MIS(cout<<"Timer missile "<<i<<"= "<<enemym[i].timer<<endl);
                enemym[i].prep = true;
            }

            if (enemym[i].prep && enemym[i].launched == false) {
                enemym[i].timer--;
            }

            if (enemym[i].timer == 0 && enemym[i].launched == false) {
                enemym[i].launched = true;
                enemym[i].bounty = false;
                enemym[i] = enemy_missile_init(enemym[i]);
            }

            if (enemym[i].launched) {

                enemym[i] = advance_by_one(enemym[i]);
                al_draw_line(enemym[i].spawn_x, enemym[i].spawn_y, enemym[i].x0, enemym[i].y0, al_map_rgb(255,0,0),1);
                al_draw_pixel(enemym[i].x0,enemym[i].y0,al_map_rgb(0,255,255));

                if ((enemym[i].y0 == enemym[i].y1) && enemym[i].launched) {

                    destroy_city(enemym[i]);
                    enemym[i].launched = false;
                    enemym[i].prep = false;
                    enemym[i].explosion.duration = 0;
                    enemym[i].explosion.flash = true;
                    enemym[i].explosion.flash_init = false;
                    enemym[i].bullet_init = false;

                }
            }

            if (enemym[i].explosion.flash)
                enemym[i] = explosion_logic(enemym[i]);

        }

}

/** Questa funzione viene chiamata ad ogni ciclo dopo l'inizializzazione per disegnare la traiettoria dei missili del giocatore. A differenza di quelli nemici, i missili del giocatore vengono disegnati per più pixel ad ogni ciclo (invece che uno), rendendoli effettivamente "più veloci". Questa funzione infine si occupa delle chiamata ad explosion_logic nel momento in cui l'obiettivo è stato raggiunto */

void player_missile_logic() {

int player_pixel_skip = 0;

    if (playerm.launched) {

        while (player_pixel_skip != FPS/10) {

            playerm = advance_by_one(playerm);
            al_draw_line(playerm.spawn_x, playerm.spawn_y, playerm.x0, playerm.y0, al_map_rgb(0,0,255),1);
            al_draw_pixel(playerm.x0, playerm.y0, al_map_rgb(0,255,255));
            player_pixel_skip++;

            if ((playerm.x0 == playerm.x1) && (playerm.y0 == playerm.y1)) {
                playerm.launched = false;
                playerm.explosion.duration = 0;
                playerm.explosion.flash = true;
                playerm.explosion.flash_init = false;
                playerm.bullet_init = false;
                break;
            }
        }

    }

    if (playerm.explosion.flash)
        playerm = explosion_logic(playerm);

}

