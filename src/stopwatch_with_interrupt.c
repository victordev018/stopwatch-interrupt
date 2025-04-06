#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "display.h"
#include "joystick.h"

// Definições relacionadas ao MENU
#define MAX_NUMBER_OPTIONS 2
int current_option = 0;

// Estrutura de timers
struct repeating_timer timer_joystick;

// Estrutura que representa uma opção no menu
typedef struct {
    char optionName[40];
} Option;

// Estrutura que representa o Menu
typedef struct {
    char header[40];
    Option options[MAX_NUMBER_OPTIONS];
} Menu;

Menu *main_menu = NULL;

// Instancia o menu
Menu* create_menu() {
    Menu *menu = (Menu*) malloc(sizeof(Menu));
    if (menu) {
        strcpy(menu->header, "Menu Cronometro");
        strcpy(menu->options[0].optionName, "Iniciar");
        strcpy(menu->options[1].optionName, "Editar tempo");
    }
    return menu;
}

void draw_menu() {
    display_clear();

    display_write_text_no_clear(main_menu->header, 20, 0, 1, 0);
    
    for (int i = 0; i < MAX_NUMBER_OPTIONS; i++) {
        int _x = 7;
        int _y = 25 + i * 12;
        if (i == current_option) {
            display_write_text_no_clear(">", _x, _y, 1, 0);
            display_write_text_no_clear(main_menu->options[i].optionName, _x+9, _y, 1, 0);
        } else {
            display_write_text_no_clear(main_menu->options[i].optionName, _x, _y, 1, 0);
        }
    }

    display_show();
}

// JOYSTICK
bool navigate_menu() {
    JoystickState state = joystick_get_state();
    if (state == JOY_UP && current_option > 0){
        --current_option;
        draw_menu();
    }

    if (state == JOY_DOWN && current_option < MAX_NUMBER_OPTIONS-1) {
        ++current_option;
        draw_menu();
    }
    
    return true;

}

void setup() {

    stdio_init_all();

    display_init();

    joystick_init();
    add_repeating_timer_ms(-100, navigate_menu, NULL, &timer_joystick);
}

int main()
{
    
    setup();

    main_menu = create_menu();

    // boas vindas e teste do display
    display_write_text("Bem vindo", 10, 10, 1, 2000);

    // exibindo os dados
    draw_menu();
    // printf("%s\n%s\n%s\n", main_menu->header, main_menu->options[0].optionName, main_menu->options[1].optionName);

    while (true) {
        
    }
}
