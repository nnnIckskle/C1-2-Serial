// Ejercicio 2 Caudalimetro //
// El microcontrolador solo debe procesar aquellos mensajes que tengan el siguiente formato, donde VALOR es un número entero entre 0 y 99. //
// Moises Del Toro - Lilia Ospino - Nicol Morales - Jesus Navarro //
// Compilador usado: Visual Studio Code. Platformio.ini //

#include <stdio.h>
#include <Arduino.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 100

int procesar_trama(char *linea, int *valor_extraido) {
    
    char formato[] = "{'caudal': ";
    int i = 0;
    
    while (formato[i] != '\0') {
        if (linea[i] != formato[i])
            return 0;
        i++;
    }

    if (!isdigit(linea[i]))
        return 0;

    int valor = 0;

    while (isdigit(linea[i])) {
        valor = valor * 10 + (linea[i] - '0');
        i++;
    }

    if (linea[i] != '}')
        return 0;

    i++;

    if (linea[i] != '\0')
        return 0;

    if (valor < 0 || valor > 99)
        return 0;

    *valor_extraido = valor;
    return 1; 
}
    char linea[MAX_LINE];

    int ultimo = 0;
    int mayor = -1;
    int menor = 100;
    int suma = 0;
    int contador = 0;


void setup() {
    Serial.begin(115200);
    Serial.println("Sistema listo. Envie tramas con formato:");
    Serial.println("{'caudal': 25}");
}

void loop() {

    if (Serial.available()) {

        static char linea[MAX_LINE];

        int len = Serial.readBytesUntil('\n', linea, MAX_LINE - 1);
        linea[len] = '\0';

        int valor;

        if (procesar_trama(linea, &valor)) {

            ultimo = valor;

            if (valor > mayor)
                mayor = valor;

            if (valor < menor)
                menor = valor;

            suma += valor;
            contador++;

            double promedio = (double)suma / contador;

            Serial.printf("{'ultimo': %d, 'mayor': %d, 'menor': %d, 'promedio': %.2f}\n",
                          ultimo, mayor, menor, promedio);
        }
    }
}
