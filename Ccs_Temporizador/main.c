#include <16F84A.h>         // Incluye el header del PIC16F84A
#fuses XT, NOWDT, NOPROTECT // Configuraciones del PIC: Oscilador externo XT, Sin Watchdog Timer, Sin protecci�n de c�digo
#use delay(clock=4000000)   // Define el uso de un cristal de 4 MHz

#include <lcd.c>            // Incluye la librer�a LCD

// Definiciones para los pines
#define BTN_MIN_INC PIN_A0         // Bot�n para incrementar minutos
#define BTN_HOUR_INC PIN_A1        // Bot�n para incrementar horas
#define BTN_START PIN_A2           // Bot�n para iniciar/parar temporizador
#define BTN_MIN_DEC PIN_A3         // Bot�n para decrementar minutos
#define BTN_HOUR_DEC PIN_A4        // Bot�n para decrementar horas
#define OUTPUT_PIN PIN_B3          // Pin de salida para el control de potencia

// Variables globales
unsigned char minutos = 0;
unsigned char horas = 0;
unsigned char estado = 0;       // 0 = parado, 1 = corriendo

// Prototipos de funciones
void display_tiempo();
void incrementar_minuto();
void incrementar_hora();
void decrementar_minuto();
void decrementar_hora();
void iniciar_parar();
void esperar_1_minuto();
void decrementar_tiempo();

void main(void) {
    set_tris_a(0x1F);    // RA0-RA4 como entradas (botones)
    set_tris_b(0x00);    // Todo el puerto B como salida (LCD y control de potencia)
    output_b(0x00);      // Limpiar puerto B

    lcd_init();          // Inicializar LCD
    lcd_putc("\f" ) ; //Borra pantalla.
    lcd_gotoxy(1,1); //Sit�a el cursor en 5 columna, 1 fila.
    lcd_putc("  Hola que tal?");  // Mensaje inicial
    lcd_gotoxy(1,2); //Sit�a el cursor en 5 columna, 1 fila.
    lcd_putc("  Para servirte.");  // Mensaje inicial
    delay_ms(3000);      // Esperar 2 segundos
    lcd_putc("\f" ) ; //Borra pantalla.
    lcd_gotoxy(1,1); //Sit�a el cursor en 5 columna, 1 fila.
    lcd_putc("  Mi nombre es:");  // Mensaje inicial
    lcd_gotoxy(1,2); //Sit�a el cursor en 5 columna, 1 fila.
    lcd_putc("   Tempo-Pic");  // Mensaje inicial
    delay_ms(3000);      // Esperar 2 segundos
    lcd_putc("\f" ) ; //Borra pantalla.

    while (TRUE) {
        display_tiempo();  // Mostrar horas y minutos en LCD
        if (input(BTN_MIN_INC)) {     // Si se presiona el bot�n de incrementar minutos
            delay_ms(200);            // Anti-rebote
            incrementar_minuto();
        }

        if (input(BTN_HOUR_INC)) {    // Si se presiona el bot�n de incrementar horas
            delay_ms(200);            // Anti-rebote
            incrementar_hora();
        }

        if (input(BTN_MIN_DEC)) {     // Si se presiona el bot�n de decrementar minutos
            delay_ms(200);            // Anti-rebote
            decrementar_minuto();
        }

        if (input(BTN_HOUR_DEC)) {    // Si se presiona el bot�n de decrementar horas
            delay_ms(200);            // Anti-rebote
            decrementar_hora();
        }

        if (input(BTN_START)) {       // Si se presiona el bot�n de inicio/parada
            delay_ms(200);            // Anti-rebote
            iniciar_parar();
        }

        if (estado == 1) {      // Si el temporizador est� corriendo
            esperar_1_minuto(); // Esperar 1 minuto
            decrementar_tiempo();  // Disminuir el tiempo

            if (horas == 0 && minutos == 0) {
                output_low(OUTPUT_PIN);  // Desactiva el pin de salida
                estado = 0;        // Parar temporizador
            }
        }
    }
}

// -----------------------
// Funciones del temporizador
// -----------------------
void display_tiempo() {
    lcd_gotoxy(1, 1);
    printf(lcd_putc, "Horas: %02u", horas);
    
    lcd_gotoxy(1, 2);
    printf(lcd_putc, "Min: %02u", minutos);
}

void incrementar_minuto() {
    if (minutos < 59) {
        minutos++;
    } else {
        minutos = 0;
    }
}

void decrementar_minuto() {
    if (minutos > 0) {
        minutos--;
    } else if (horas > 0) {
        minutos = 59;
        decrementar_hora();
    }
}

void incrementar_hora() {
    if (horas < 23) {
        horas++;
    } else {
        horas = 0;
    }
}

void decrementar_hora() {
    if (horas > 0) {
        horas--;
    }
}

void iniciar_parar() {
    estado = !estado;   // Alterna entre iniciar y parar
    if (estado == 1) {
        output_high(OUTPUT_PIN);  // Activar el pin de salida
    }
    else{
      output_low(OUTPUT_PIN);  // Desativa el pin de salida
    }
}

void esperar_1_minuto() {
    for (int i = 0; i < 60; i++) {
        delay_ms(1000);   // Retardo de 1 segundo
    }
}

void decrementar_tiempo() {
    if (minutos > 0) {
        minutos--;
    } else if (horas > 0) {
        horas--;
        minutos = 59;
    }
}
