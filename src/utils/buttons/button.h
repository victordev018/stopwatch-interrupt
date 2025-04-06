#ifndef BUTTON_H
#define BUTTON_H

#include "pico/stdlib.h"

// definição da pinagem
#define PIN_BTN_B 6            // Pino do botão B

/*
* Função responsável por inicializar o botáo B
*/
void button_init();

/*
* Função para ativar as interrupção do botão B
*/
void button_enable_interrupt();

/*
* Função para desativar as interrupção do botão B
*/
void button_disable_interrupt();

#endif