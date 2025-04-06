#ifndef MENU_H
#define MENU_H

// Definições relacionadas ao MENU
#define MAX_NUMBER_OPTIONS 2

// Estrutura que representa uma opção no menu
typedef struct {
    char optionName[40];
} Option;

// Estrutura que representa o Menu
typedef struct {
    char header[40];
    Option options[MAX_NUMBER_OPTIONS];
} Menu;

/*
* Cria um novo menu pré definido
*/
Menu* create_menu();

/*
* Desenha no display menu
*/
void draw_menu(Menu *menu, int current_option);

#endif