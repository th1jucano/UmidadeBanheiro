#include <Arduino.h>
#include <DHT.h>
#include <PiscaLed.h>

int rele = 8;
int sensor = 10;
float somaLeituras = 0;
float qtdLeituras = 0;
float media = 0;
int pinoLed = 13;

DHT dht(sensor, DHT11);
PiscaLed led(13);

void setup() {
  dht.begin();
  Serial.begin(9600);
  pinMode(rele, OUTPUT);
  pinMode(sensor, INPUT);
  led.begin();
}

void loop() {
  float umidade = dht.readHumidity();
  if (umidade > 10){
    digitalWrite(rele, 1);
  }else{
    digitalWrite(rele, 0);
  } 
  somaLeituras = somaLeituras + umidade;
  qtdLeituras = qtdLeituras + 1;
  media = somaLeituras / qtdLeituras;
  Serial.print("Umidade: ");
  Serial.println(umidade);
  Serial.print("Media ate o momento: ");
  Serial.println(media);
  delay(1000);
  led.atualizar();

}
