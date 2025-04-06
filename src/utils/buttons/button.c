#include "button.h"
#include <stdio.h>

void button_init() {
    // inicializa o botão B
    gpio_init(PIN_BTN_B);
    gpio_set_dir(PIN_BTN_B, GPIO_IN);
    gpio_pull_up(PIN_BTN_B); // Configura pull-up interno
}


void button_enable_interrupt() {
    gpio_set_irq_enabled(PIN_BTN_B, GPIO_IRQ_EDGE_FALL, true);
}


void button_disable_interrupt() {
    gpio_set_irq_enabled(PIN_BTN_B, GPIO_IRQ_EDGE_FALL, false);
}