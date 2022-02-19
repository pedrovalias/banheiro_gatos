#include <Arduino.h>

void verifica_banheiro(int numSensor){
   
  distancia = validarDistancia(numSensor);

  boolean gatoUsandoBanheiro = false ;

  if(distancia <= limite_distancia && distancia != 0){
    gatoUsandoBanheiro = true;
    gato_usou_banheiro = true;

    while(gatoUsandoBanheiro) {

      Serial.println("O Gato está no banheiro");
      
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_VERMELHO, HIGH);

      distancia = validarDistancia(numSensor);

      if(distancia > limite_distancia || distancia == 0) {
        Serial.println("Banheiro liberado");
        Serial.print("Tempo de espera antes de ligar a valvula: ");
        for(int i = 0 ; i < tempoEsperaSegundos; i++){
          delay(1000);
          Serial.print(i);
        }
        // delay(5000);
        gatoUsandoBanheiro = false;
        digitalWrite(LED_VERMELHO, LOW);
      }
    }
  }

  Serial.print("Gato usou o banheiro "); 
  Serial.print(numSensor);
  Serial.print(" : ");
  Serial.println(gato_usou_banheiro);

  executaLimpeza(numSensor);
}