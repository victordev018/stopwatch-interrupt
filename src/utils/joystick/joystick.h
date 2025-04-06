#ifndef JOYSTICK_H
#define JOYSTICK_H

// inclusões de bibliotecas
#include <stdio.h>
#include "hardware/adc.h"

// Pinos do Joystick
#define EIXO_Y 26

// Definições para obter o estado do joystick
#define JOY_DEADZONE 400
#define JOY_CENTER 1915
#define JOY_UP_THRESHOLD    (JOY_CENTER + JOY_DEADZONE)
#define JOY_DOWN_THRESHOLD  (JOY_CENTER - JOY_DEADZONE)

typedef enum {
    JOY_IDLE,       // parado
    JOY_UP,         // moveu para cima
    JOY_DOWN        // moveu para baixo
} JoystickState;

/*
* Função reponsável por inicializar o joystick com o eixo Y
*/
void joystick_init();

/*
* Função reponsável por fazer a leitura do eixo Y
*/
uint16_t joystick_read_current_value();

/*
* Função reponsável por onbter o estado (ou posição) do joystick
*/
JoystickState joystick_get_state();

#endif