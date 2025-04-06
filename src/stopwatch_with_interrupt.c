#include <stdio.h>
#include "pico/stdlib.h"
#include "display.h"

void setup() {

    stdio_init_all();

    display_init();
}

int main()
{
    
    setup();

    // boas vindas e teste do display
    display_write_text("Bem vindo", 10, 10, 1, 0);
    

    while (true) {
        
    }
}
