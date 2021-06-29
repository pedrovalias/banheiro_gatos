#include <Arduino.h>

#include <NewPing.h>

// #define ECHO_PIN_1      5    // Arduino pin tied to echo pin on the ultrasonic sensor.
// #define TRIGGER_PIN_1   6    // Arduino pin tied to trigger pin on the ultrasonic sensor.

//#define ECHO_PIN_2    7       // Arduino pin tied to echo pin on the ultrasonic sensor.
//#define TRIGGER_PIN_2 8       // Arduino pin tied to trigger pin on the ultrasonic sensor.

#define MAX_DISTANCE    55      // Distância máxima que queremos o ping(em centimetros). O máximo possível é 400-500cm.

#define SONAR_NUM 2             // Número de sensores

NewPing sonar[SONAR_NUM] = {    // Sensor object array.
  NewPing(6, 5, MAX_DISTANCE),  // Cada sensor possui um trigger pin, um echo pin e a maxima distancia para o ping  
  NewPing(8, 7, MAX_DISTANCE)
};

#define PINO_VALVULA_1  9       // Válvula Solenoide 1
#define PINO_VALVULA_2  10      // Válvula Solenoide 2

#define LED_VERDE    2 
#define LED_AZUL     3
#define LED_VERMELHO 4

int distancia = 0;
int delayLoop = 3;
const int limite_distancia = 32;
boolean gato_usou_banheiro = false;
int tempoEsperaSegundos = 3;    // Tempo determinado para espera é de 120 segundos   

// int execucoesLimpeza = 0;

void inicializaPinagem();
void verifica_banheiro(int numSensor);
void verificaStatusSistema();
void executaLimpeza(int banheiro);
void apagaLEDS();
void piscaLEDS();

void setup() {

  Serial.begin(115200);

  inicializaPinagem();
}

void loop() {

  verificaStatusSistema();

  digitalWrite(LED_VERDE, HIGH);
  
  // Envia o ping, obtem a distancia em cm e exibe terminal (0 = fora do alcance)
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop por cada sensor e exibição do valor lido
    delay(50);                              // Aguarda 50ms entre pings (aprox 20 pings/segundo). 29ms deve ser o menor delay entre pings.
    Serial.print(i);
    Serial.print("=");
    Serial.print(sonar[i].ping_cm());
    Serial.print("cm ");
  }
  Serial.println();

  verifica_banheiro(0);
  verifica_banheiro(1);

  delay(delayLoop * 1000);
}

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

int validarDistancia(int dv){
  
  int d[5];
  int media = 0;

  Serial.println("Validando distancia do sensor " + String(dv));
  for(uint8_t i = 0; i < 5; i++){
    d[i] = sonar[dv].ping_cm();
    delay(500);
    Serial.println("leitura " + String(i) + " : " + String(d[i]));
//    Serial.print(i);
//    Serial.print(" : ");
//    Serial.println(d[i]);
  }

  media = (d[0] + d[1] + d[2] + d[3] + d[4]) /5;

  Serial.println("Distancia média obtida: " + String(media));
  Serial.println("-------------------------------------------------");

  return media;
}

void executaLimpeza(int banheiro){

  distancia = validarDistancia(banheiro);
  
  if((distancia > limite_distancia || distancia == 0) && gato_usou_banheiro == true){ 
    Serial.print("Limpeza em andamento!");
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(LED_AZUL, HIGH);
    banheiro == 0 ? digitalWrite(PINO_VALVULA_1, LOW) : digitalWrite(PINO_VALVULA_2, LOW);
    Serial.print("Valvula "); Serial.print(banheiro); Serial.println(" Ligada");
    delay(8000);
    banheiro == 0 ? digitalWrite(PINO_VALVULA_1, HIGH) : digitalWrite(PINO_VALVULA_2, HIGH);
    digitalWrite(LED_AZUL, LOW);
    gato_usou_banheiro = false;
    // execucoesLimpeza ++;
  }

}

void verificaStatusSistema(){

  int distancia_validada1 = 0;
  int distancia_validada2 = 0;

  Serial.println("######### Verificando Status do Sistema #########");

  boolean controle = true;

  // Enquanto a leitura do sensor trouxer valor zero os LEDs piscam sequenciamente
  while(controle){

    distancia_validada1 = validarDistancia(0);
    distancia_validada2 = validarDistancia(1);

    if(distancia_validada1 + distancia_validada2 > 0){
      controle = false;
    } else {
      piscaLEDS();
      apagaLEDS();
    } 
  }
}

void apagaLEDS(){
  delay(700);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_VERMELHO, LOW);
}

void piscaLEDS(){
    digitalWrite(LED_VERDE, HIGH);
    delay(700);
    // digitalWrite(LED_AZUL, HIGH);
    // delay(700);
    digitalWrite(LED_VERMELHO, HIGH);
}
