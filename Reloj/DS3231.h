#include <Wire.h>
#include <Arduino.h>

typedef struct Time_Data
{
    uint8_t s;
    uint8_t m;
    uint8_t h;
    uint8_t d;
}time_t;

const PROGMEM header_t n_dia[] =
{   
    "LUNES",
    "MARTES",
    "MIERCOLES",
    "JUEVES",
    "VIERNES",
    "SABADO",
    "DOMINGO"
};

class DS3231
{
private:
    uint8_t dectobcd(const uint8_t val)
    {
        return ((val / 10 * 16) + (val % 10));
    }
    uint8_t bcdtodec(const uint8_t val)
    {
        return ((val / 16 * 10) + (val % 16));
    }

public:
    uint8_t second,minute,hour,wday,mday,month,year;
    
    DS3231();
    void begin();
    void write(uint8_t sec, uint8_t min, uint8_t hour, uint8_t wday, uint8_t mday, uint8_t mon, uint8_t year);
    void update();
    void read();
    time_t time();
    String getDateTime();
    String getTime();
    String getDate();
    char* getDayName();
};

DS3231::DS3231()
{
}

void DS3231::begin()
{
    Wire.begin();
}

void DS3231::write(uint8_t sec, uint8_t min, uint8_t hour, uint8_t wday, uint8_t mday, uint8_t mon, uint8_t year)
{
    uint8_t TimeDate[7] = {sec, min, hour, wday, mday, mon, year};

    Wire.beginTransmission(0x68);
    Wire.write(0x00);
    for (uint8_t i = 0; i <= 6; i++) {
        TimeDate[i] = dectobcd(TimeDate[i]);
        Wire.write(TimeDate[i]);
    }
    Wire.endTransmission();
}

void DS3231::update()
{
    uint8_t TimeDate[7] = {second, minute, hour, wday, mday, month, year};

    Wire.beginTransmission(0x68);
    Wire.write(0x00);
    for (uint8_t i = 0; i <= 6; i++) {
        TimeDate[i] = dectobcd(TimeDate[i]);
        Wire.write(TimeDate[i]);
    }
    Wire.endTransmission();
}

void DS3231::read()
{
    uint8_t TimeDate[7];        //second,minute,hour,dow,day,month,year
    Wire.beginTransmission(0x68);
    Wire.write(0x00);
    Wire.endTransmission();

	bool gotData = false;
	uint32_t start = millis(); // start timeout
	while(millis()-start < 100){
	  if (Wire.requestFrom(0x68, 7) == 7) {
      	gotData = true;
      	break;
      }
      delay(2);
    }
	if (!gotData)
    {
        second = 80;
    	return; // error timeout
    }
    second  = bcdtodec(Wire.read());
    minute  = bcdtodec(Wire.read());
    hour    = bcdtodec(Wire.read());
    wday    = bcdtodec(Wire.read());
    mday    = bcdtodec(Wire.read());
    month   = bcdtodec(Wire.read());
    year    = bcdtodec(Wire.read());   
}

time_t DS3231::time()
{
    time_t t;
    t.h = hour;
    t.m = minute;
    t.s = second;
    t.d = wday;
    return t;
}

char* DS3231::getDayName()
{
    memory.set(n_dia);
    return memory.take(wday-1);
}

String DS3231::getDate()
{
	char out[21];
	sprintf(out,"%02u/%02u/20%02u", mday, month, year);
	return String(out);
}

String DS3231::getTime()
{
	char out[10];
	sprintf(out,"%02u:%02u:%02u", hour, minute, second);
	return String(out);
}

String DS3231::getDateTime()
{
	char out[21];
    memory.set(n_dia);
    String shortDay = String(memory.take(wday)).substring(0,2);
	sprintf(out,"%2s %02u/%02u/%02u %02u:%02u:%02u", shortDay.c_str(), mday, month, year, hour, minute, second);
	return String(out);
}