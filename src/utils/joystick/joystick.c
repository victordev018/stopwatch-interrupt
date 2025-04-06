#include "joystick.h"

void joystick_init() {
    
    // configurando o adc e inicializa o eixo Y
    adc_init();
    adc_gpio_init(EIXO_Y);
}

uint16_t joystick_read_current_value() {
    adc_select_input(0);
    return adc_read();
}

JoystickState joystick_get_state() {
    uint16_t value = joystick_read_current_value();
    if (value > JOY_UP_THRESHOLD) return JOY_UP;
    if (value < JOY_DOWN_THRESHOLD) return JOY_DOWN;
    return JOY_IDLE;
}