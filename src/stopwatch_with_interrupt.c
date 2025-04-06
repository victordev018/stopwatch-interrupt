// Biblioteca padrões do C e biblioteca para trabalhar com strings
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Bibliotecas do sdk SDK Raspberry Pi Pico W
#include "pico/stdlib.h"                // biblioteca padrão do sdk
#include "hardware/timer.h"             // biblioteca para trabalhar com timer

// Bibliotecas auxiliares
#include "display.h"                    // funções para manipulação do display ssd1306
#include "menu.h"                       // funções para manipulação do menu
#include "joystick.h"                   // funções para manupulação do joystick


// variável de controle para identificar a atual opção do menu selecionada
int current_option = 0;

// Estrutura de timers
struct repeating_timer timer_joystick;

// Menu principal da aplicação
Menu *main_menu = NULL;

/*
* Callback para navegar no menu
*/
bool navigate_menu() {
    JoystickState state = joystick_get_state();
    if (state == JOY_UP && current_option > 0){
        --current_option;
        draw_menu(main_menu, current_option);
    }

    if (state == JOY_DOWN && current_option < MAX_NUMBER_OPTIONS-1) {
        ++current_option;
        draw_menu(main_menu, current_option);
    }
    
    return true;
}

/*
* Função que inicializa os dispositivos
*/
void setup() {

    // inicializa a comunicação serial
    stdio_init_all();
    // inicializa o display
    display_init();
    // inicializa o joystick e configura interrupção para ler o joystick e atualizar o diplay a cada 100ms
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
    draw_menu(main_menu, current_option);

    while (true) {
        printf("current option: %d\n", current_option);
        sleep_ms(300);
    }
}
