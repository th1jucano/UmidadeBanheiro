#include "PiscaLed.h"

// Construtor

PiscaLed::PiscaLed(int pino, unsigned long tempoOn, unsigned long tempoOff){

    pinLed = pino;
    tempoLigado = tempoOn;
    tempoDesligado = tempoOff;
    ultimoTempo = 0;
    estadoAtual = false;

}

void PiscaLed::begin(){

    pinMode(pinLed, OUTPUT);
    digitalWrite(pinLed, LOW);
    ultimoTempo = millis();

}

void PiscaLed::atualizar(){
    unsigned long tempoAtual = millis();

    if (estadoAtual &&(tempoAtual - ultimoTempo >= tempoLigado)){
        digitalWrite(pinLed, LOW);
        estadoAtual = false;
        ultimoTempo = tempoAtual;
    }
    else if (!estadoAtual &&(tempoAtual - ultimoTempo >= tempoDesligado)){
        digitalWrite(pinLed, HIGH);
        estadoAtual = true;
        ultimoTempo = tempoAtual;
    }

}

void PiscaLed::setTempos(unsigned long tempoOn, unsigned long tempoOff){
    tempoLigado = tempoOn;
    tempoDesligado = tempoOff;
}

void PiscaLed::ligar(){

    digitalWrite(pinLed, HIGH);
    estadoAtual = true;

}

void PiscaLed::desligar(){

    digitalWrite(pinLed, LOW);
    estadoAtual = false;

}

bool PiscaLed::getEstado(){
    return estadoAtual;
}

void PiscaLed::reiniciar(){

    ultimoTempo = millis();
    estadoAtual = false;
    digitalWrite(pinLed, LOW);
}