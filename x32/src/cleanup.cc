#include "main_header.h"

//** Questa funzione si occupa di riportare le variabili principali ai valori di default nel caso in cui il giocatore decida di ricominciare la partita dopo un game over senza riavviare il programma */
void cleanup() {

    for (int i = 0; i < 6; i++) {
        city_status[i] = true;
    }

    for (int i = 0; i < MAX_ENEMY_MISSILES; i++) {
        enemym[i].launched = false;
        enemym[i].prep = false;
        enemym[i].bounty = false;
        enemym[i].explosion.duration = 60;
    }

    playerm.bullet_init = false;
    playerm.launched = false;

    playerm.explosion.duration = 0;

    turret[0] = 8;
    turret[1] = 8;
    turret[2] = 8;

    score = 0;

}
