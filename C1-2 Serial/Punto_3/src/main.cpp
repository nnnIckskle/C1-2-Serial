/*
Enunciado: 
 1. Independencia del orden: El programa debe localizar las claves sin importar la
    posición en la que aparezcan dentro del objeto json.
2. Validación de integridad:
   1. Si el mensaje contiene todas las claves requeridas, se deben extrar los valores
   e imprimirlos por el serial.
   2. Si falta al menos una clave obligatoria, el sistema debe imprimir un mensaje de
   error específico por serial.
3. Validación de tipo:
   1. Se deber verificar que cada valor corresponda al tipo de dato especificado 
   (string para ID, numérico para los demás).
   2. Si se detecta un tipo de dato incorrecto, el mensaje debe ser ignorado por 
   completo.
4. Manejo de claves excedentes:
   1. Si el objeto contiene claves adicionales a las solicitadas, estas deben ser
   ignoradas.
   2. El programa debe procesar y mostrar únicamente las cuatro claves obligatorias.

Compilador usado: Visual Studio Code. Platformio.ini

Lila Ospino - Moises Del Toro - Nicol Morales - Jesus Navarro
*/


#include <Arduino.h>
#include <ArduinoJson.h>

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("-------------------------------------------------");
  Serial.println("Esperando datos de mensaje JSON...");

  Serial.println("Debe enviar un JSON con las siguientes claves:");
  Serial.println("ID -> String (texto entre comillas)");
  Serial.println("Temperatura -> Número (entero o decimal)");
  Serial.println("Humedad -> Número (entero o decimal)");
  Serial.println("Distancia -> Número (entero o decimal)");
  Serial.println();

  Serial.println("-------------------------------------------------");
  Serial.println("Formato esperado:");
  Serial.println("{\"ID\":\"Sensor_1\",\"Temperatura\":25.5,\"Humedad\":60,\"Distancia\":100}");
  Serial.println("-------------------------------------------------");
  Serial.println();
}

void loop() {

  if (Serial.available()) {

    String mensaje = Serial.readStringUntil('\n');
    mensaje.trim();
  if (mensaje.length() == 0) {
    Serial.println("-------------------------------------------------"); 
    Serial.println("No se recibió ningún mensaje JSON. Asegúrese de enviar un mensaje válido.");
    Serial.println("-------------------------------------------------");
    Serial.println();
    return;
  }

    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, mensaje);

    if (error){
      Serial.println("Error al parsear el mensaje JSON. Ignorando mensaje.");
      Serial.println("-------------------------------------------------");
      Serial.println();
      return;
    }

    if (!doc.containsKey("ID") || !doc["ID"].is<String>()){
      Serial.println("Error: La clave 'ID' es obligatoria y debe ser un string.");
      Serial.println("-------------------------------------------------");
      Serial.println();
      return;
    }

    if (!doc.containsKey("Temperatura") || !doc["Temperatura"].is<float>()){
      Serial.println("Error: La clave 'Temperatura' es obligatoria y debe ser un numero.");
      Serial.println("-------------------------------------------------");
      Serial.println();
      return;
    }

    if (!doc.containsKey("Humedad") || !doc["Humedad"].is<float>()){
      Serial.println("Error: La clave 'Humedad' es obligatoria y debe ser un numero.");
      Serial.println("-------------------------------------------------");
      Serial.println();
      return;
    }

    if (!doc.containsKey("Distancia") || !doc["Distancia"].is<float>()){
      Serial.println("Error: La clave 'Distancia' es obligatoria y debe ser un numero.");
      Serial.println("-------------------------------------------------");
      Serial.println();
      return;
    }

    String ID = doc["ID"];
    float temperatura = doc["Temperatura"];
    float humedad = doc["Humedad"];
    float distancia = doc["Distancia"];

    Serial.println("Datos recibidos:");
    Serial.println("ID: " + ID);
    Serial.println("Temperatura: " + String(temperatura));
    Serial.println("Humedad: " + String(humedad));
    Serial.println("Distancia: " + String(distancia));
    
    Serial.println("-------------------------------------------------");
    Serial.println();
  }
}