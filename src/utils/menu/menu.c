#include <stdlib.h>
#include "menu.h"
#include "display.h"

Menu* create_menu() {
    Menu *menu = (Menu*) malloc(sizeof(Menu));
    if (menu) {
        strcpy(menu->header, "Menu Cronometro");
        strcpy(menu->options[0].optionName, "Iniciar");
        strcpy(menu->options[1].optionName, "Editar tempo");
    }
    return menu;
}

void draw_menu(Menu *menu, int current_option) {
    display_clear();

    display_write_text_no_clear(menu->header, 20, 0, 1, 0);
    
    for (int i = 0; i < MAX_NUMBER_OPTIONS; i++) {
        int _x = 7;
        int _y = 25 + i * 12;
        if (i == current_option) {
            display_write_text_no_clear(">", _x, _y, 1, 0);
            display_write_text_no_clear(menu->options[i].optionName, _x+9, _y, 1, 0);
        } else {
            display_write_text_no_clear(menu->options[i].optionName, _x, _y, 1, 0);
        }
    }

    display_show();
}