/**
 * Exemplo criado para a Palestra no Genuino Day 2016
 * realizado no dia 2 de Abril, no ICMC - USP São Carlos
 * 
 * Titulo da Palestra: "IoT com ESP8266"
 * Palestrante: Daniel Junho
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid     = "Sua rede";
const char* password = "Senha da rede";


// Coloque aqui a chave de escrita do seu canal (ThingSpeak)
#define CHAVE_DE_ESCRITA     "SUA CHAVE AQUI!"

void ConectarWiFi(void){
  Serial.println("Conectando-se no WiFi");  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void setup() {

  Serial.begin(115200);
  delay(10);
  
  Serial.println();
  Serial.println();
    
  Serial.print("Conectando-se a rede ");
  Serial.println(ssid);

  ConectarWiFi();

  Serial.println("Conectado no WiFi");  
  Serial.print("IP = "); Serial.println(WiFi.localIP());
  Serial.print("\n\n");

}


void loop() {
  static int contador = 0;
  
  // Espera pela conexão da rede
  if((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    
    // Enviando dados 
    String dados = "http://api.thingspeak.com/update?key=" CHAVE_DE_ESCRITA "&field1=";
    dados += String(contador);
    
    Serial.print("contador = "); Serial.println(contador);
    
    http.begin(dados.c_str()); //HTTP

    Serial.print("[HTTP] GET... Enviando dados ao ThingSpeak\n");
    
    int httpCode = http.GET();

    // Em caso de erro "httpCode" é negativo
    if(httpCode > 0) {
      // Em caso de sucesso incrementa o contador
      contador++;
    
      // Caso queira mostrar a resposta. Ex: "200 OK"
      //Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // Se OK mostra o conteúdo
      if(httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        // O valor no caso do ThingSpeak é o indice que o dado foi escrito no banco
        Serial.print("Valor obtido: "); Serial.println(payload);
      }
    } else {
      // Em caso de erro, mostra a msg
      Serial.printf("[HTTP] GET... falhou, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    
  } else {

    //Caso perca a conexão, tenta se conectar novamente.
    ConectarWiFi();
  }

  // Repete o processo a cada 15 segundos
  delay(15000);
  Serial.print("\n\n");
}

