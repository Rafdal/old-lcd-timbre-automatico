#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 11, 12, 10, 9, 2);

// #define USE_SERIAL_PRINTS

#include "Forms/Forms.h"

#include "Reloj/TimbreAuto.h"
TimbreAuto timbre(32);

unsigned long lastMillis;

uint8_t pinRelay = 4;

#include "Frontend/Home.h"

void setup()
{
	pinMode(pinRelay, OUTPUT);
	Serial.begin(230400);
	ctrl.setRealTimeLoop(realTimeLoop);
	ctrl.setLoopPerSec(loop_1s);
	ctrl.setInactivityTimeout(61);
	lcd.begin(20, 4);
	ch.init();
	lcd.clear();


	switch (timbre.begin())
	{
	case 1:
		{
			digitalWrite(pinRelay, LOW);
			MessageBox.error.fatal(F("RTC Desconectado!"));
		}
		break;
	
	case 2:
		{
			MessageBox.error.show(F("Cambiar Pila RTC o  Configurar FechaHora"),5000);
		}
		break;
	default:
		ctrl.delay(700);
		MessageBox.setCursor(1,1);
		MessageBox.animate1(F("TIMBRE  AUTOMATICO"),1500,1500);
		lcd.clear();
		break;
	}
	ctrl.delay(500);

	
	init_crear_timbre();
	init_menu();
}

// int lastRAM, minRAM=2000;
void realTimeLoop()
{
	/* int RAM = freeMemory();
	if (lastRAM != RAM)
	{
		if (RAM < minRAM)
			minRAM = RAM;
		
		Serial.print(F("RAM: ")); 
		Serial.print(RAM);

		Serial.print(F("\tMinRAM: "));
		Serial.println(minRAM);
		lastRAM = RAM;
	} */     
}

void loop_1s()
{
	timbre.rtc.read();
	bool releState = timbre.status();
	digitalWrite(pinRelay, releState);
}

void loop()
{
	control_t thisCtrl;
	do
	{
		if (millis() - lastMillis >= 1000)
		{
			StaticScreen();
			lastMillis = millis();
		}
		ctrl.runRealTime();
	} while (!ctrl.someonePressed());

	Home.run();	
}

void StaticScreen()
{
	timbre.rtc.read();
	lcd.clear();
	lcd.setCursor(0,0);

	lcd.print(timbre.rtc.getDayName());

	lcd.setCursor(10,0);
	lcd.print(timbre.rtc.getDate());
	lcd.setCursor(0,1);
	lcd.print(F("Hora:"));
	lcd.setCursor(12,1);
	lcd.print(timbre.rtc.getTime());
	lcd.setCursor(0,3);
	lcd.print(F("Modo:  "));
	lcd.print(timbre.getStatus());
}