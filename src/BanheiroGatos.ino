#include <Arduino.h>

#include <NewPing.h>

#define ECHO_PIN_1      5    // Arduino pin tied to echo pin on the ultrasonic sensor.
#define TRIGGER_PIN_1   6    // Arduino pin tied to trigger pin on the ultrasonic sensor.

//#define ECHO_PIN_2    7    // Arduino pin tied to echo pin on the ultrasonic sensor.
//#define TRIGGER_PIN_2 8    // Arduino pin tied to trigger pin on the ultrasonic sensor.

#define MAX_DISTANCE    55   // Distância máxima que queremos o ping(em centimetros). O máximo possível é 400-500cm.

#define PINO_VALVULA_1  9    // Válvula Solenoide 1
#define PINO_VALVULA_2  10   // Válvula Solenoide 2

#define LED_VERDE    2 
#define LED_AZUL     3
#define LED_VERMELHO 4

int distancia = 0;
int delayLoop = 3;
int execucoesPrograma = 0;
const int limite_distancia = 35;
boolean gato_usou_banheiro = false;

void inicializaPinagem();
void verifica_banheiro();
void verificaStatusSistema();
void apagaLEDS();
void piscaLEDS();

NewPing sonar(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE); // NewPing setup

void setup() {

  Serial.begin(115200);

  inicializaPinagem();
}

void loop() {

  verificaStatusSistema();

  digitalWrite(LED_VERDE, HIGH);
  
  Serial.print("Ping: ");
  Serial.print(sonar.ping_cm());  // Envia o ping, obtem a distancia em cm e exibe terminal (0 = fora do alcance)
  Serial.println("cm");

  verifica_banheiro();

  Serial.print("Gato usou o banheiro: "); Serial.println(gato_usou_banheiro);

  distancia = validarDistancia();
  
  if((distancia > limite_distancia || distancia == 0) && gato_usou_banheiro == true){ 
    Serial.print("Limpeza em andamento!");
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(LED_AZUL, HIGH);
  
    digitalWrite(PINO_VALVULA_1, LOW);
    Serial.println("Valvula Ligada");
    delay(8000);
    digitalWrite(PINO_VALVULA_1, HIGH);
    digitalWrite(LED_AZUL, LOW);
    gato_usou_banheiro = false;
  }

  execucoesPrograma ++;
  Serial.print("Execuções Programa: "); Serial.println(execucoesPrograma);
  
  // Loop executa a cada x segundos (estipulado no setup)
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

void verifica_banheiro(){
   
  distancia = validarDistancia();
  boolean gatoUsandoBanheiro = false ;

  if(distancia <= limite_distancia && distancia != 0){
    gatoUsandoBanheiro = true;
    gato_usou_banheiro = true;

    while(gatoUsandoBanheiro) {

      Serial.println("O Gato está no banheiro");
      
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_VERMELHO, HIGH);

      distancia = validarDistancia();

      if(distancia > limite_distancia || distancia == 0) {
        Serial.println("Banheiro liberado");
        delay(5000);
        gatoUsandoBanheiro = false;
        digitalWrite(LED_VERMELHO, LOW);
      }
    }
  }
}

int validarDistancia(){

  int d1 = 0;
  int d2 = 0;
  int d3 = 0;
  int media = 0;

  d1 = sonar.ping_cm();
  delay(500);
  d2 = sonar.ping_cm();
  delay(500);
  d3 = sonar.ping_cm();

  Serial.print("D1: " + String(d1));
  Serial.print("    | D2: " + String(d2));
  Serial.println("    | D3: " + String(d3));

  media = (d1 +d2 + d3) / 3;

  Serial.println("Distancia média: " + String(media));
  Serial.println("-------------------------------------------------");

  return media;
}

void verificaStatusSistema(){

  int distancia_validada = validarDistancia();

  Serial.println("######### Verificando Status do Sistema #########");

  boolean controle = true;

  // Enquanto a leitura do sensor trouxer valor zero os LEDs piscam sequenciamente
  while(controle){

    int distancia_validada = validarDistancia();

    if(distancia_validada > 0){
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
