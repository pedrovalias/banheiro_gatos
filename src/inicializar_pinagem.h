#include <Arduino.h>

void inicializaPinagem(){
  pinMode(PINO_VALVULA_1, OUTPUT);
  digitalWrite(PINO_VALVULA_1, HIGH);

  pinMode(PINO_VALVULA_2, OUTPUT);
  digitalWrite(PINO_VALVULA_2, HIGH);

  pinMode(LED_VERDE, OUTPUT);
  digitalWrite(LED_VERDE, LOW);

  pinMode(LED_VERMELHO, OUTPUT);
  digitalWrite(LED_VERMELHO, LOW);

  pinMode(LED_AZUL, OUTPUT);
  digitalWrite(LED_AZUL, LOW);
}