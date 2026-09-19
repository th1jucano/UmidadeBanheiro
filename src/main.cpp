#include <Arduino.h>
#include <DHT.h>
#include <PiscaLed.h>
#include <avr/wdt.h>

void desativarWatchdogAoIniciar() __attribute__((naked, used, section(".init3")));
void desativarWatchdogAoIniciar() {
  MCUSR = 0;
  wdt_disable();
}

int rele = 6;
int sensor = 7;
int leituraUmidade = 80;
float leituras[10] = {0};
int idxLeitura = 0;
float ultimaUmidade = 0;
unsigned long ultimaLeitura = 0;
unsigned long ultimoSerial = 0;
unsigned long intervalo = 120000;
int falhasConsecutivas = 0;

DHT dht(sensor, DHT22);
PiscaLed led(10, 125, 2000);

void setup() {
  dht.begin();
  Serial.begin(9600);
  pinMode(rele, OUTPUT);
  pinMode(sensor, INPUT);
  led.begin();
  ultimaLeitura = millis() - intervalo; // força leitura imediata ao ligar
  wdt_enable(WDTO_8S);
}

void loop() {
  led.atualizar();

  if (millis() - ultimoSerial >= 1000) {
    ultimoSerial = millis();
    Serial.print("Umidade: ");
    Serial.print(ultimaUmidade);
    Serial.print(" | Rele: ");
    Serial.println(digitalRead(rele));
  }

  if (millis() - ultimaLeitura >= intervalo) {
    ultimaLeitura = millis();
    float umidade = dht.readHumidity();

    if (isnan(umidade)) {
      falhasConsecutivas++;

      Serial.print("Falha no DHT: ");
      Serial.print(falhasConsecutivas);
      Serial.println("/5");

      if (falhasConsecutivas >= 5) {
        Serial.println("Reiniciando Arduino...");
        Serial.flush(); // espera a mensagem terminar de sair

        wdt_enable(WDTO_15MS);
        while (true) {
          // Sem renovar o watchdog: ele reinicia o Arduino.
        }
      }
    } else {
      falhasConsecutivas = 0; // leitura válida zera a contagem
    }

    ultimaUmidade = umidade;
    leituras[idxLeitura] = umidade;
    idxLeitura = (idxLeitura + 1) % 10;
    float media = 0;
    for (int i = 0; i < 10; i++) media += leituras[i];
    media /= 10;
    Serial.print("Media ate o momento: ");
    Serial.println(media);
    if (umidade > leituraUmidade){
      digitalWrite(rele, 1);
      intervalo = 120000;
    }else{
      digitalWrite(rele, 0);
      intervalo = 2000;
    }
  }
  wdt_reset();
}

 
