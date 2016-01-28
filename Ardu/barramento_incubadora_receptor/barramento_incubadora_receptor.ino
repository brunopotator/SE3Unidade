#include "RCSwitch.h"

//#define MIDDLEWARE

RCSwitch receptor = RCSwitch();

//RFID pode ser entre 0 e 1023 (10bits)
//Temp e Umid vão ser enviados informações de 0 a 127, que está dentro do escopo do nosso projeto
//LDR, SOM passarão pelo map para limitar até 15 (4bits)
#define RFID 487
#define DESLOCAMENTO_RFID 22
#define DESLOCAMENTO_TEMP 15
#define DESLOCAMENTO_UMID 8
#define DESLOCAMENTO_LDR 4
#define DESLOCAMENTO_SOM 0

//Pinos Digitais
#define PINO_RECEPTOR 0     //0 = Primeiro pino de Interrupção; No Arduino Uno R3/Nano =  Pino Digital nº 2

//Variaveis Globais
unsigned long info1;

#ifdef MIDDLEWARE
struct Sensores { // necessário inserir todos os eixos dos sensores
  int temp, umid, valLdr, valSom, rfid;
};
Sensores sensor;
#endif


void receber() {
  unsigned long info;

  if (receptor.available()) {
    info = receptor.getReceivedValue();
    //Serial.print("Info_Recebido: ");
    //Serial.println(info, BIN);
    receptor.resetAvailable();
  }

  if (RFIDValido(info)) {
    info1 = info;
  }
  else {
    info1 = -1;
  }
}

boolean RFIDValido(long info) {
  boolean valido = false;

  int rfid = info >> DESLOCAMENTO_RFID;
  if (rfid == RFID) {
    valido = true;
  }
  return valido;
}

#ifdef MIDDLEWARE
int extrairRfid(long info) {
  int rfid = info >> DESLOCAMENTO_RFID;
  return rfid;
}

int extrairTemp(long info) {
  int temp = (info & 4161536) >> DESLOCAMENTO_TEMP;
  return temp;
}

int extrairUmid(long info) {
  int umid = (info & 32512) >> DESLOCAMENTO_UMID;
  return umid;
}

int extrairLdr(long info) {
  int ldr = (info & 240) >> DESLOCAMENTO_LDR;
  return ldr;
}

int extrairSom(long info) {
  int som = (info & 15);
  return som;
}
#endif

void enviarSensores() {
#ifdef MIDDLEWARE
  int tam = sizeof(sensor); // retorna o tamanho total em bytes
  char buff[tam]; // cria um buffer que recebe o tamanho
  memcpy(&buff, &sensor, tam); // envia o endereço do buffer, o endereço dos eixos e o tamanho total

  Serial.write('I'); // inicializar
  Serial.write((uint8_t*)&buff, tam); // converte o buff para
  Serial.write('F'); // finalizar
#else
  int tam = sizeof(info1); // retorna o tamanho total em bytes
  Serial.write('I'); // inicializar
  Serial.write((uint8_t*)&info1, tam); // Envia info1
  Serial.write('F'); // finalizar
#endif
}


void setup() {
  Serial.begin(9600);
  //Configuração do Receptor
  receptor.enableReceive(PINO_RECEPTOR);
#ifdef MIDDLEWARE
  info1 = -1;
#endif
  //Serial.print(sizeof(sensor)); Tamanho de sensor = 10
}

void loop() {
  //Emissão de Dados
  receber();
  delay(150);
  //Serial.print("Info1: "); Serial.println(info1); Serial.print("Info2: "); Serial.println(info2);
  if ( info1 != -1 ) {
#ifdef MIDDLEWARE
    sensor.rfid = extrairRfid(info1);
    sensor.temp = extrairTemp(info1);
    sensor.umid = extrairUmid(info1);
    sensor.valLdr = extrairLdr(info1);
    sensor.valSom = extrairSom(info1);
#endif
    enviarSensores();
  }

  //Serial.print("Temp: "); Serial.print(sensor.temp); Serial.print("\t\tUmid: "); Serial.println(sensor.umid); Serial.print("Ldr: "); Serial.print(sensor.valLdr); Serial.print("\t\t\tSom: "); Serial.println(sensor.valSom);
}



