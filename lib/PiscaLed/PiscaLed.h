#ifndef PISCALED_H
#define PISCALED_H

#include <Arduino.h>

class PiscaLed {

private:
	int pinLed;
	unsigned long ultimoTempo;
	bool estadoAtual;
	unsigned long tempoLigado;
	unsigned long tempoDesligado;

public:
	// construtor
	PiscaLed(int pino, unsigned long tempoOn = 1000, unsigned long tempoOff = 500);

	// Métodos principais

	void begin();
	void atualizar();

	// Métodos auxiliares

	void setTempos(unsigned long tempoOn, unsigned long tempoOff);
	void ligar();
	void desligar();
	bool getEstado();
	void reiniciar();
};

#endif