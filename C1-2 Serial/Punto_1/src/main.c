/* Ejercicio 1:

Describir una solución para el microcontrolador que calcule el cuadrado de un número recibido por puerto serial
e imprima el resultado por puerto serial.
Lila Ospino - Moises Del Toro - Nicol Morales - Jesus Navarro
Compilador usado: Visual Studio Code. Platformio.ini
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"

#define UART_PORT       UART_NUM_0
#define UART_BAUDRATE   115200
#define RX_BUF_SIZE     256

static uint64_t cuadrado_por_impares(uint32_t N) {
    uint64_t suma = 0;
    for (uint32_t i = 1; i <= N; i++) {
        uint32_t impar = 2 * i - 1;
        suma += impar;
    }
    return suma;
}

static void uart_send_str(const char *s) {
    uart_write_bytes(UART_PORT, s, (int)strlen(s));
}

void app_main(void) {

    uart_config_t cfg = {
        .baud_rate = UART_BAUDRATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
        
    };

    uart_driver_install(UART_PORT, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_PORT, &cfg);
    uart_set_pin(UART_PORT,
                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE,
                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    uint8_t rx[RX_BUF_SIZE];
    char line[128];
    int line_len = 0;

    uart_send_str("Listo. Envie un entero positivo y presione Enter.\r\n");

    while (1) {
        int n = uart_read_bytes(UART_PORT, rx, sizeof(rx), pdMS_TO_TICKS(100));
        if (n <= 0) continue;

        for (int i = 0; i < n; i++) {
            char c = (char)rx[i];

            if (c == '\r') continue; 

            if (c == '\n') {
                line[line_len] = '\0';

                if (line_len > 0) {
                    errno = 0;
                    char *endptr = NULL;
                    long val = strtol(line, &endptr, 10);

                    while (endptr && *endptr && isspace((unsigned char)*endptr)) endptr++;

                    if (errno == 0 && endptr && *endptr == '\0' && val > 0) {
                        uint32_t N = (uint32_t)val;
                        uint64_t res = cuadrado_por_impares(N);

                        char out[128];
                        
                        snprintf(out, sizeof(out),
                                 "[N: %u, Cuadrado: %llu]\r\n",
                                 (unsigned)N, (unsigned long long)res);
                        uart_send_str(out);
                    }
                    
                }

                line_len = 0;
                continue;
            }

            if (line_len < (int)sizeof(line) - 1) {
                line[line_len++] = c;
            } else {
                line_len = 0; 
            }
        }
    }
}