#include <EEPROM.h>



// & CODE  S T A R T    
typedef struct TimbreInfo // 11 BYTES
{
    uint8_t duration;
    uint8_t hour;
    uint8_t minute;
    bool day[7];
    bool isEveryday;
}timbre_t;

typedef struct TimbreRawData // 4 BYTES
{
    uint8_t data[4];
}tdata_t;

enum DAYS_WEEL{
    LU,
    MA,
    MI,
    JU,
    VI,
    SA,
    DO
};

#include "DS3231.h"

const PROGMEM header_t mode_names[] =
{
	"Desactivado",
	"Automatico",
    "ERROR"
};

#define AUTOMATIC true
#define MANUAL	false

// #    C L A S S      
/*  ( EEPos, slots ) */
class TimbreAuto
{
    public:

        /*  (slots, EEPos) */
        TimbreAuto(uint8_t slots, int eeprom_start_pos);
        ~TimbreAuto();

        #ifdef USE_SERIAL_PRINTS
            void print(timbre_t t);
            void listAll();
        #endif

        // void load();
        uint8_t begin();
        bool switchMode();

        void clear();

        uint8_t length();
        
        bool is(uint8_t idx);
        bool exist(tdata_t td);
        uint8_t add(timbre_t t);
        uint8_t add(uint8_t HH, uint8_t MM);
        void set(uint8_t idx, timbre_t timbre);
        void set(uint8_t idx, tdata_t td);
        void del(uint8_t idx);
        timbre_t get(uint8_t idx);
        char* toStr(uint8_t _idx);

		timbre_t createNew(); 
        timbre_t parse(tdata_t tdata);
        tdata_t parse(timbre_t t);
        tdata_t read(uint8_t idx);

        bool status();
        char* getStatus();

        tdata_t *T;
        DS3231 rtc;
        uint8_t errorCode=0;
        bool mode;
    private:
        char buffer[20];
        char nullChar = '\0';
        int EPOS;
        const int epos = 5;
        timbre_t timbre;
        uint8_t len;
        int _pos_(uint8_t idx);
};

TimbreAuto :: TimbreAuto (uint8_t max_slots, int eeprom_start_pos = 500)
{
    EPOS = eeprom_start_pos;
    len = max_slots;
    T = new tdata_t[max_slots];
}

TimbreAuto :: ~TimbreAuto ()
{
    if (T)
        delete[] T;
}

int TimbreAuto :: _pos_ (uint8_t idx)
{
    return (EPOS + (int)idx * 4 );
}

uint8_t TimbreAuto :: length ()
{ 
    return len; 
}

/* Borra EEPROM y RAM */
void TimbreAuto :: clear ()
{
    tdata_t empty_td;
    for (uint8_t i = 0; i < 4; i++)
        empty_td.data[i] = 0;
    
    for (int i = EPOS; i < EPOS+(int)(len*4); i++)
        EEPROM.update(i,0);
    
    for (uint8_t i = 0; i < len; i++)
        T[i] = empty_td;
}

/*
 * 0 - OK
 * 1 - Fatal RTC Disconnected
 * 2 - Sin pila, actualizar hora
 */
uint8_t TimbreAuto :: begin ()
{
    mode = EEPROM.read(epos);
    rtc.begin();
    for (uint8_t i = 0; i < len; i++)
        T[i] = read(i);

    rtc.read();
    if(rtc.second > 59)
        errorCode = 1;
    else if(rtc.year == 0)
        errorCode = 2;

    return errorCode;
}

bool TimbreAuto :: switchMode()
{
    if (errorCode == 0)
    {
        mode = !mode;
        EEPROM.update(epos, mode);
        return true;
    }
    return false;
}

bool TimbreAuto :: is (uint8_t idx)
{
    return (T[idx].data[0] > 0 && idx < len);
}


bool TimbreAuto :: exist(tdata_t td)
{
    for (uint8_t i = 0; i < len; i++)
    {
        if (T[i].data[0] != 0)
        {
            if (td.data[1] == T[i].data[1] && td.data[2] == T[i].data[2])
                return true;
        }
    }
    return false;
}
/*
 * 0 - Ya existe en ese horario
 * 1 - No queda mas espacio
 * 2 - timbre exitoso
 * */
uint8_t TimbreAuto :: add(uint8_t HH, uint8_t MM)
{
    tdata_t td;
    td.data[0] = 5;
    td.data[1] = HH;
    td.data[2] = MM;
    td.data[3] = B10000000;

    if(exist(td))
        return 0;
    
    for (uint8_t i = 0; i < len; i++)
    {
        if (!is(i))
        {
            set(i, td);
            return 2;
        }
    }
    return 1;
}
/*
 * 0 - Ya existe en ese horario
 * 1 - No queda mas espacio
 * 2 - timbre exitoso
 * */
uint8_t TimbreAuto :: add(timbre_t t)
{
    tdata_t td = parse(t);

    if(exist(td))
        return 0;
    
    for (uint8_t i = 0; i < len; i++)
    {
        if (!is(i))
        {
            set(i, td);
            return 2;
        }
    }
    return 1;
}

void TimbreAuto :: set(uint8_t idx, tdata_t td)
{
    int pos = _pos_(idx);
    for (uint8_t i = 0; i < 4; i++)
        EEPROM.update(pos+i,   td.data[i]);

    T[idx] = td;
}

void TimbreAuto :: set (uint8_t idx, timbre_t t)
{
    if (idx < len)
    {    
        set(idx, parse(t));
    }
}

timbre_t TimbreAuto :: get (uint8_t idx)
{
    timbre_t t = createNew();
    if (is(idx))
        t = parse(T[idx]);
    return t;
}

void TimbreAuto :: del (uint8_t idx)
{
    EEPROM.update(_pos_(idx), 0);
    T[idx].data[0] = 0;
}

#ifdef USE_SERIAL_PRINTS
    void TimbreAuto :: print (timbre_t t)
    {
        if (t.isEveryday)
            Serial.print("ALL ");
        else
        {
            for (uint8_t i = 0; i < 7; i++)
            {
                if (t.day[i])
                {
                    memory.set(n_dia);
                    Serial.print(memory.take(i));
                    Serial.print(' ');
                }
            }
        }
        Serial.print(t.hour);
        Serial.print(':');
        Serial.print(t.minute);
        Serial.print('	');
        Serial.println(t.duration);
    }

    void TimbreAuto :: listAll()
    {
        for (uint8_t idx = 0; idx < len; idx++)
        {
            if (is(idx)) // Si existe un timbre en la ranura <idx>, ejecutar
            {
                timbre_t t = get(idx); // Leer el timbre de la ranura <idx>

                Serial.print(idx);
                Serial.print(": "); 
                print(t);
            }
        }
    }
#endif

char* TimbreAuto :: toStr(uint8_t _idx)
{
    timbre_t t = get(_idx);
    if (t.duration > 0)
    {
        sprintf(buffer, "%02u:%02u", t.hour, t.minute);
        return buffer;
    }
    else
    {
        return &nullChar;
    }
}

timbre_t TimbreAuto :: createNew ()
{
	tdata_t td;
    for (uint8_t i = 0; i < 4; i++)
        td.data[i] = 0;
	return parse(td);
}

timbre_t TimbreAuto :: parse (tdata_t tdata)
{
    timbre_t t;
    t.duration = tdata.data[0];
    t.hour = tdata.data[1];
    t.minute = tdata.data[2];
    
    for (uint8_t i = 0; i < 7; i++)
        t.day[i] = tdata.data[3] & (B00000001 << i);
    t.isEveryday = tdata.data[3] & B10000000;
    return t;
}

tdata_t TimbreAuto :: parse (timbre_t t)
{
    tdata_t td;
    uint8_t days = 0;

    for (uint8_t i = 0; i < 7; i++)
        days |= t.day[i] << i;
    days |= t.isEveryday << 7;

    td.data[0] = t.duration;
    td.data[1] = t.hour;
    td.data[2] = t.minute;
    td.data[3] = days;
    return td;
}

tdata_t TimbreAuto :: read (uint8_t idx)
{   
    tdata_t td;
    int pos = _pos_(idx);
    for (uint8_t i = 0; i < 4; i++)
        td.data[i] = EEPROM.read(pos+i);
    return td;
}

bool TimbreAuto :: status()
{
    uint8_t HH = rtc.hour;
    uint8_t MM = rtc.minute;
    uint8_t SS = rtc.second;
    uint8_t DD = rtc.wday-1;

    if (!mode || errorCode != 0)
    {
        return false;
    }

    for (uint8_t t_idx = 0; t_idx < len; t_idx++)
    {
        if (is(t_idx))
        {
            timbre_t t = get(t_idx);
            if (t.hour == HH && t.minute == MM && t.duration > SS)
            {
                if (t.isEveryday)
                {
                    return DD < 5;
                }
                else
                {
                    return t.day[DD];
                }
            }
        }
    }
    return false;
}

char* TimbreAuto :: getStatus()
{
    memory.set(mode_names);
    if (errorCode == 2)
        return memory.take(errorCode);
    return memory.take((uint8_t)mode);
}