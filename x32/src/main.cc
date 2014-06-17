/** \mainpage Missile Command

 \section presgioco Il gioco

Questo software è una semplice riproduzione del classico videogame "Missile Command" per 
Atari 2600. Il giocatore controlla tre basi sul fondo del display, ognuna costituita da 8 torri, e con 	esse deve difendere le città circostanti dai missili nemici provenienti dalla parte superiore del display.

Cliccando sullo schermo il giocatore può lanciare un missile dalla base più vicina, "consumando" una torre. Una volta raggiunto l'obiettivo il missile causerà una piccola esplosione per un breve periodo. Se durante questo tempo almeno un missile nemico entra in contatto con essa, il missile viene eliminato, vengono assegnati dei punti al giocatore, e viene assegnata una torre bonus.

Nel caso in cui un missile nemico riesca a raggiungere una città, quest'ultima viene distrutta. Perdere tutte e 6 le città porta, ovviamente, al game over. I missili nemici possono inoltre colpire le basi del giocatore, che in tal caso perdono 2 torri. 

\section prescodice Il codice

Questo software si appoggia sulla libreria Allegro (versione 5.0) e sugli addon font, ttf, image e primitives. Il cursore, i missili e le esplosioni sono disegnate interamente dalle funzioni "primitives", mentre lo sfondo, le torri e le città sono file .bmp caricati durante l'inizializzazione. Gli addon font e ttf sono necessari per caricare la riproduzione del font Atari originale per il menu iniziale, per la schermata di game over, e per il contatore del punteggio.

Il codice è suddiviso in 4 file sorgente con 2 header.

main.cc : File principale, inizializza le varie funzionalità di allegro, gestisce le fasi speciali di intro / game over, e contiene la base del loop principale, che a sua volta andrà a chiamare funzioni in altri file. Per il tracing in questo file è stata usata la macro DEBUG_MAIN.

main_header.h : Header principale incluso in tutti e 4 i file sorgente. Contiene le definizioni delle variabili comuni a tutti i file, degli struct e la dichiarazione della maggior parte delle funzioni.

missiles_logic.cc : File contenente tutte le funzioni che riguardano i missili, la loro inizializzazione, le funzioni che ne regolano la traiettoria e le esplosioni. Per il tracing in questo file è stata usata la macro DEBUG_MIS.

drawing.cc : Contiene le funzioni che ad ogni ciclo del programma disegnano sul display città e torri rimanenti, ed il cursore. Per il tracing in questo file è stata usata la macro DEBUG_DRAW.

drawing.h : Header contenente alcune dichiarazioni, incluso solo da drawing.cc.

cleanup.cc : File contenente una singola funzione di "pulizia", chiamata nel caso in cui il giocatore decida di ricominciare la partita dopo un eventuale game over senza riavviare il software.

*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "main_header.h"
#include "drawing.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <allegro5/allegro_primitives.h>

using namespace std;

#define DEBUG_MAIN(a) {}

/** Numero di frame che verranno processati al secondo */
extern const float FPS;

/** Larghezza del display (in pixel) */
extern const int SCREEN_W;

/** Altezza del display (in pixel) */
extern const int SCREEN_H;

/** Numero massimo consentito di missili nemici in un qualsiasi momento durante la partita */
extern const int MAX_ENEMY_MISSILES;

/** Valori utilizzati in seguito per determinare le coordinate del puntatore durante il movimento e dopo un clic*/
float lineX;
/** Valori utilizzati in seguito per determinare le coordinate del puntatore durante il movimento e dopo un clic*/
float lineY;

/** Punteggio totalizzato dal giocatore*/
unsigned int score = 0;

/** Array di 3 elementi rappresentante il numero di torri disponibili in ognuna delle tre basi durante il gioco. */
float turret[3] = {8, 8, 8};

/** Array di bool rappresentante lo stato di ognuna delle 6 città durante il gioco. */
bool city_status[6] = {true, true, true, true, true, true};

/** Variabile di tipo missiles (struct definito nel main_header) che gestisce i dati riguardanti i missili lanciati dal giocatore. */
missiles playerm;

/** Array di missiles (struct definito nel main_header) che gestisce i dati riguardanti i missili lanciati automaticamente dal nemico. Ogni elemento dell'array gestisce un missile individuale. */
missiles enemym[MAX_ENEMY_MISSILES];

/** Inizializzazioni delle funzioni base di Allegro e degli addon necessari per il funzionamento del programma (font, ttf, image, primitives). Il main inoltre gestisce la schermata iniziale, finale, e il loop principale che ad ogni ciclo andrà a chiamare le funzioni necessarie */
int main(int argc, char **argv) {

	ALLEGRO_DISPLAY 	  *display 			= NULL;
    ALLEGRO_EVENT_QUEUE   *event_queue 		= NULL;
    ALLEGRO_TIMER 	   	  *timer	   	    = NULL;
    ALLEGRO_BITMAP 	      *background_b	    = NULL;
    ALLEGRO_BITMAP        *city_b      	    = NULL;
    ALLEGRO_BITMAP        *turret_b     	= NULL;

    bool refresh = true;
    bool game_over_state = false;
    bool intro = true;
    bool quit = false;
    int y = 462;

    srand(time(NULL));

    if(!al_init()) {
 	   cout<<"Error: failed to initialize Allegro!"<<endl;
       return -1;
    }

    al_init_font_addon();

    if(!al_init_ttf_addon()) {
		al_show_native_message_box(display, "Error", "Error", "Failed to load .ttf addon!",
                                    NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
    }

	if (!al_init_image_addon()) {
		al_show_native_message_box(display, "Error", "Error", "Failed to load image addon!",
                                    NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    if (!al_init_primitives_addon()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load primitives addon!",
                                    NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    if(!al_install_mouse()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load mouse input!",
                                    NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    if(!al_install_keyboard()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load keyboard input!",
                                    NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    timer = al_create_timer(1.0 / FPS);

    if(!timer) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load timer!",
                                    NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);

    if(!display) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load display!",
                                    NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_timer(timer);
        return -1;
    }

    event_queue = al_create_event_queue();

    if(!event_queue) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load event queue!",
                                    NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    ALLEGRO_FONT *menu_font_large = al_load_ttf_font ("./Resources/Font/atarifont.ttf", 30,0);
    ALLEGRO_FONT *menu_font_small = al_load_ttf_font ("./Resources/Font/atarifont.ttf", 20,0);
    ALLEGRO_FONT *score_font = al_load_ttf_font ("./Resources/Font/atarifont.ttf", 14, 0);

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_start_timer(timer);

    al_hide_mouse_cursor(display);

    background_b = al_load_bitmap("./Resources/bmp/image.bmp");

    if(!background_b) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
                                    NULL, ALLEGRO_MESSAGEBOX_ERROR);
    	al_destroy_display(display);
   	    return 0;
    }

    city_b = al_load_bitmap("./Resources/bmp/city.bmp");

    if(!city_b) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load city!",
                                    NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return 0;
    }

    turret_b = al_load_bitmap("./Resources/bmp/turret.bmp");

    if(!turret_b) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load turret bitmap!",
                                    NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return 0;
    }

    al_draw_bitmap(background_b,0,0,0);

    al_flip_display();

    while (intro) {

   		ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

    	al_draw_text(menu_font_large, al_map_rgb(0,255,255), 255, 100, ALLEGRO_ALIGN_CENTRE, "MISSILE");
   	    al_draw_text(menu_font_large, al_map_rgb(0,255,255), 255, 150, ALLEGRO_ALIGN_CENTRE, "COMMAND");
        al_draw_text(menu_font_small, al_map_rgb(0,255,255), 255, 300, ALLEGRO_ALIGN_CENTRE, "Press ENTER to begin");
    	al_flip_display();

   	    if (ev.type == ALLEGRO_EVENT_KEY_UP)
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                intro = false;

   	    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
	        quit = true;
                break;
        }

    }

    while(1) {

        if (quit)
            break;

        al_draw_bitmap(background_b,0,0,0);

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER) {
            refresh = true;
        }

        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
            lineX = ev.mouse.x;
            lineY = ev.mouse.y;
        }

        else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {

            if (playerm.launched == false && ev.mouse.y < 400) {
                playerm = player_missile_init(playerm);
            }
        }

        if(refresh) {

            refresh = false;

            al_draw_bitmap(background_b,0,0,0);

            al_draw_textf(score_font, al_map_rgb(255,0,0), 255, 5, ALLEGRO_ALIGN_CENTRE, "%d", score);

            draw_cities(city_b);

            draw_turret(40, 0, turret_b);
            draw_turret(246, 1, turret_b);
            draw_turret(480, 2, turret_b);

            player_missile_logic();
            enemy_missile_logic();

            create_crosshair();

            if (city_status[0] == false)
                if (city_status[1] == false)
                    if (city_status[2] == false)
                        if (city_status[3] == false)
                            if (city_status[4] == false)
                                if (city_status[5] == false)
                                    game_over_state = true;

            if (game_over_state) {

                while (1) {

                    ALLEGRO_EVENT ev;
                    al_wait_for_event(event_queue, &ev);

                    while (y > 40) {

                        al_draw_text(menu_font_large, al_map_rgb(0,255,255), 255, y+20, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
                        al_draw_textf(score_font, al_map_rgb(255,0,0), 255, 5, ALLEGRO_ALIGN_CENTRE, "%d", score);
                        al_draw_text(menu_font_small, al_map_rgb(0,255,255), 255, y+200, ALLEGRO_ALIGN_CENTRE, "Press R to restart");
                        al_draw_text(menu_font_small, al_map_rgb(0,255,255), 255, y+300, ALLEGRO_ALIGN_CENTRE, "Press ESCAPE to quit");

                        al_flip_display();
                        al_rest(0.001);
                        al_draw_bitmap(background_b,0,0,0);
                        y--;

                    }

                al_draw_text(menu_font_large, al_map_rgb(0,255,255), 255, y+20, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
                al_draw_textf(score_font, al_map_rgb(255,0,0), 255, 5, ALLEGRO_ALIGN_CENTRE, "%d", score);
                al_draw_text(menu_font_small, al_map_rgb(0,255,255), 255, y+200, ALLEGRO_ALIGN_CENTRE, "Press R to restart");
                al_draw_text(menu_font_small, al_map_rgb(0,255,255), 255, y+300, ALLEGRO_ALIGN_CENTRE, "Press ESCAPE to quit");

                al_flip_display();

                if (ev.type == ALLEGRO_EVENT_KEY_UP) {
                    if (ev.keyboard.keycode == ALLEGRO_KEY_R) {
                        game_over_state = false;
                        y = 462;
                        cleanup();
                        break;
                }

                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    quit = true;
                    game_over_state = false;
                    cleanup();
                    break;
                }

				if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				quit = true;
				
          		 break;
       			 }

                }
                }
            }

            al_flip_display();
        }
    }

    al_destroy_timer(timer);
    al_destroy_bitmap(turret_b);
    al_destroy_bitmap(background_b);
    al_destroy_bitmap(city_b);
    al_destroy_font(menu_font_large);
    al_destroy_font(menu_font_small);
    al_destroy_font(score_font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
