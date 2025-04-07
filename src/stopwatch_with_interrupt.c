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
#include "button.h"                     // funções para manipulação dos botões

// Enum para servir como máquina de estados para a aplicação
typedef enum {
    STATE_MENU,         // menu inicial
    STATE_EDIT_TIME,    // modo de edição de tempo
    STATE_RUNNING       // modo de cronômetro rodando
} AppState;

// Variável de controle para o estado da aplucação
AppState current_state;

// variável de controle para identificar a atual opção do menu selecionada
int current_option = 0;
int timer_of_stopwatch = 10;

// Estrutura de timers
struct repeating_timer timer_joystick;
struct repeating_timer timer_debouncing;
struct repeating_timer timer_stopwatch;

// Menu principal da aplicação
Menu *main_menu = NULL;

// Definindo o tipo do callback genérico para Timer: função que retorna true e não recebe argumentos
typedef bool (*CallbackTimer)();

/*
* Função que desativa o joystick para leituras periódicas (100ms)
*/
void disable_joystick_reading() {
    cancel_repeating_timer(&timer_joystick);
}

/*
* Função que ativa o joystick para leitura a cada 100ms e executa um callback passado
*/
void activate_joystick_reading(CallbackTimer callback) {
    // desativo o timer do joystick que estava rodando, caso tenha
    disable_joystick_reading();
    // configuro o novo callback para executar a cada leitura do joystick
    add_repeating_timer_ms(-100, callback, NULL, &timer_joystick);
}

/*
* Callback para poder reativar os botões
*/
bool reeneble_button_callback() {
    button_enable_interrupt();      // ativa novamente o botão
    return false;                   // retorna falso para a função não ser chamada novamente
}

/*
* Função reponsável por tratar o efeito bounce
*/
void debouncing() {
    button_disable_interrupt();     // desativa o botão temporariamente
    add_repeating_timer_ms(200, reeneble_button_callback, NULL, &timer_debouncing);
}

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
* Callback que atualiza o cronômetro no display
*/
bool update_stopwatch_on_display() {
    static int current_second = -1;
    
    // garantindo que o cronômetro sempre use o valor atualizado do delay
    static int last_delay_time = 5;
    if (timer_of_stopwatch != last_delay_time){
        current_second = timer_of_stopwatch;
        last_delay_time = timer_of_stopwatch;
    }

    // no primeiro loop, o segundo atual será o tempo configurado para o cronômetro
    if (current_second == -1) current_second = timer_of_stopwatch;

    // quando o cronômetro zerar
    if (current_second == 0) {
        current_second = timer_of_stopwatch;        // zera o segundo atual para tempo configurado
        current_state = STATE_MENU;                 // altera o estado da aplicação para o estado do MENU
        draw_menu(main_menu, current_option);       // atualiza o display para exibir o menu
        activate_joystick_reading(navigate_menu);   // ativa novamente o joystick para navegar no menu
        return false;                               // retorna falso para o callback não ser mais chamado
    }

    // escreve cronômetro regressivo no display
    display_clear();
    char timer_msg[20];
    snprintf(timer_msg, sizeof(timer_msg), "Timer: %d", current_second);
    display_write_text_no_clear(timer_msg, 10, 20, 2, 0);
    display_show();

    // decrementando o segundo final
    --current_second;
    return true;    // retorna true para o callback continuar sendo executado
}

/*
* Função para a opção de iniciar o cronômetro
*/
void option_init_stopwatch() {
    // altera o estado da aplicação para cronômetro rodando
    current_state = STATE_RUNNING;
    // desativa a leitura do joystick temporariamente
    disable_joystick_reading();
    // Iniciar cronômetro
    add_repeating_timer_ms(-1000, update_stopwatch_on_display, NULL, &timer_stopwatch);
}

/*
* Callback reposnável por fazer a edição do timer
*/
bool edit_timer() {
    // caso tenha confirmado o novo valor do timer
    if (current_state == STATE_MENU) return false;

    // obtendo o estado do joystick
    JoystickState joy_state = joystick_get_state();

    // atualizando o valor do timer entre (1-60) segundos
    if (joy_state == JOY_UP && timer_of_stopwatch < 60) {
        timer_of_stopwatch++;
    } else if (joy_state == JOY_DOWN && timer_of_stopwatch > 1) {
        timer_of_stopwatch--;
    }

    // escrevendo no display valor atual do timer
    display_clear();
    char current_timer_msg[20];
    snprintf(current_timer_msg, sizeof(current_timer_msg), "Atual: %d", timer_of_stopwatch);
    display_write_text_no_clear(current_timer_msg, 10, 15, 2, 0);
    display_write_text_no_clear("Min: 1  Max: 60", 20, 50, 1, 0);
    display_show();
    return true;
}

/*
* Função para aopção de editar o timer do cronômetro
*/
void option_edit_timer() {
    // atualizar o stado da aplicação para STATE_EDIT_TIMER
    current_state = STATE_EDIT_TIME;
    // configurar um timer para ajustar o timer do cronômetro
    activate_joystick_reading(edit_timer);
}

/*
* Função que direciona para a opção selecionada
*/
void run_option(int option) {

    // caso seja a opção de iniciar cronômetro
    if (option == 0) {
        option_init_stopwatch();
        return;
    }

    // caso seja a opção de editar tempo do cronômetro
    if (option == 1) {
        option_edit_timer();
        return;
    }
}

/*
* Callback para gerenciar as interrupções geradas pelo botão
*/
void button_callback(uint pin, uint32_t event) {
    // caso o cronômetro já esteja rodando, aborta a ação
    if (current_state == STATE_RUNNING) return;

    // trata o efeito bounce
    debouncing();

    // caso esteja no menu e selecione uma opção
    if (current_state == STATE_MENU) {
        // executa função da opção selecionada'
        run_option(current_option);
        return;
    }
    
    // caso esteja no modo de edição do timer e confirme o novo valor
    if (current_state == STATE_EDIT_TIME) {
        // atualizar o stado da aplicuação para STATE_MENU
        current_state = STATE_MENU;
        activate_joystick_reading(navigate_menu);
        draw_menu(main_menu, current_option);
        return;
    }
}

/*
* Função que inicializa os dispositivos
*/
void setup() {

    // inicializa a comunicação serial
    stdio_init_all();
    // inicializa o display
    display_init();
    // inicializa o botão B e configura a interrupção
    button_init();
    gpio_set_irq_enabled_with_callback(PIN_BTN_B, GPIO_IRQ_EDGE_FALL, true, button_callback);
    // inicializa o joystick e configura interrupção para ler o joystick e atualizar o diplay a cada 100ms
    joystick_init();
    activate_joystick_reading(navigate_menu);
}

int main()
{
    // incializa a comunicação serial e os dispotivos
    setup();
    // instancia o menu principal da aplicação
    main_menu = create_menu();
    // configura o estado inicial da aplicação para o estado do MENU
    current_state = STATE_MENU;
    // boas vindas no display
    display_write_text("Bem vindo", 35, 25, 1, 1000);
    // desenhando o menu no display
    draw_menu(main_menu, current_option);

    while (true) {
        tight_loop_contents();
    }
}
