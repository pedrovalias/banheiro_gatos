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

  boolean gato_usou_banheiro = verifica_banheiro();

  Serial.print("Gato usou o banheiro: "); Serial.println(gato_usou_banheiro);

  distancia = sonar.ping_cm();
  
  if(distancia > limite_distancia && gato_usou_banheiro){  
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(LED_AZUL, HIGH);
    digitalWrite(PINO_VALVULA_1, LOW);
    Serial.println("Valvula Ligada");
    delay(8000);
    digitalWrite(PINO_VALVULA_1, HIGH);
    digitalWrite(LED_AZUL, LOW);      
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

boolean verifica_banheiro(){
  
  distancia = sonar.ping_cm();
  
  if(distancia <= limite_distancia && distancia != 0){
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, HIGH);
    Serial.println("O Gato está no banheiro");
    delay(15000);                                 
    return true;  
  }
  return false;
}

void verificaStatusSistema(){

  int d = 0;
  boolean controle = true;

  // Enquanto a leitura do sensor trouxer valor zero os LEDs piscam sequenciamente
  while(controle){

    d = sonar.ping_cm();

    if(d > 0){
      controle = false;
    }
    apagaLEDS();
    piscaLEDS();
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
    digitalWrite(LED_AZUL, HIGH);
    delay(700);
    digitalWrite(LED_VERMELHO, HIGH);
}
