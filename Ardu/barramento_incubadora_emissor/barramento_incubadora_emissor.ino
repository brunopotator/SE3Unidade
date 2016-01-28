#include "RCSwitch.h"
#include "DHT.h"

RCSwitch emissor = RCSwitch();

//RFID pode ser entre 0 e 1023 (10bits)
//Temp e Umid vão ser enviados informações de 0 a 127, que está dentro do escopo do nosso projeto
//LDR, SOM passarão pelo map para limitar até 15 (4bits)
#define RFID 487
#define DESLOCAMENTO_RFID 22
#define DESLOCAMENTO_TEMP 15
#define DESLOCAMENTO_UMID 8
#define DESLOCAMENTO_LDR 4
#define DESLOCAMENTO_SOM 0

//Pinos Analógicos
#define PINO_SOM 0          //Ligar Saida Sensor Som ky-038 ou Potenciometro
#define PINO_LDR 1          //Ligar Saida Sensor LDR ou Potenciometro
#define DHTPIN A2           // Ligar no Pino Analógico
#define DHTTYPE DHT11       // Modelo DHT11
DHT dht(DHTPIN, DHTTYPE);   // Definindo DHT

//Pinos Digitais
#define PINO_EMISSOR 4  //Ligar no pino digital

//Funcao para enviar long pelo RF
void emitir (unsigned long infoxx) {
  emissor.send(infoxx, 32);
}

//Função para gerar long com dados da 1 transmissão
unsigned long concatenarSensores( unsigned long temp, unsigned long umid, unsigned long ldr, unsigned long som) {
  unsigned long rf = RFID;
  unsigned long info = rf << DESLOCAMENTO_RFID;
  info = info | (temp << DESLOCAMENTO_TEMP);
  info = info | (umid << DESLOCAMENTO_UMID);
  info = info | (ldr << DESLOCAMENTO_LDR);
  info = info | som;

  return info;
}

void setup() {
  Serial.begin(9600);
  //Configuração do Receptor
  emissor.enableTransmit(PINO_EMISSOR);

  //Configuração Sensores
  pinMode(PINO_LDR, INPUT);
  pinMode(PINO_SOM, INPUT);
  dht.begin();
}

void loop() {
  //Emissão de Dados
  unsigned long temp = dht.readTemperature();
  unsigned long umid = dht.readHumidity();
  unsigned long valLdr, valSom;
  valLdr = analogRead(PINO_LDR);
  valLdr = map(valLdr, 0, 1023, 0, 15);
  valSom = analogRead(PINO_SOM);
  valSom = map(valSom, 0, 1023, 0, 15);
  unsigned long info1 = concatenarSensores(temp, umid, valLdr, valSom);
  Serial.print("Info1: "); Serial.println(info1); Serial.print("Info1BIN: "); Serial.println(info1, BIN);
  emitir(info1);
  delay(100);
  //Teste Print
  //Serial.print("Info1: "); Serial.println(info1); Serial.print("Info2: "); Serial.println(info2);
  Serial.print("Temp: "); Serial.print(temp); Serial.print("\t\tUmid: "); Serial.println(umid); Serial.print("Ldr: "); Serial.print(valLdr); Serial.print("\t\t\tSom: "); Serial.println(valSom);

}
