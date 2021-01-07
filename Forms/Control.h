typedef struct ControlData
{
    bool ok;
    bool down;
    bool up;
    bool exit;
    bool upLong;
    bool dwLong;
    bool okLong;
    bool exitLong;
} control_t;

typedef void layout_t;

typedef void (*void_event_t)(void);

#include <JC_Button.h>

class Control
{
private:
    Button btnUp = Button(A2);
    Button btnDn = Button(3);
    Button btnOK = Button(A0);
    Button btnEX = Button(A3);

    void_event_t realTimeLoop, loop_1s;
    unsigned long lastMillis;
    uint8_t inactiveTime, timeout;
public:
    Control();
    ~Control();

    void setRealTimeLoop(void_event_t rtl);
    void setLoopPerSec(void_event_t lxS);
    void setInactivityTimeout(uint8_t _timeout);
    void runRealTime();

    bool isInactive();
    bool someonePressed();

    control_t read();
    void delay(unsigned int t);
    #ifdef USE_SERIAL_PRINTS
        void print(control_t c);
    #endif
};

Control::Control()
{
}

Control::~Control()
{
}
void Control :: setRealTimeLoop(void_event_t rtl)
{
    realTimeLoop = rtl;
    btnUp.begin();
    btnDn.begin();
    btnEX.begin();
    btnOK.begin();
}

void Control :: setLoopPerSec(void_event_t lxS)
{
    loop_1s = lxS;
}

void Control :: setInactivityTimeout(uint8_t _timeout)
{
    timeout = _timeout;
}

bool Control :: isInactive()
{
    if (timeout != 0)
    {
        return (inactiveTime >= timeout);    
    }
    return false;
}

void Control :: runRealTime()
{
    if (realTimeLoop != NULL)
        (*realTimeLoop)();


    if (loop_1s != NULL)
    {
        if (millis() - lastMillis >= 1000)
        {
            (*loop_1s)();
            inactiveTime++;
            lastMillis = millis();
        }
    }
}

bool Control :: someonePressed()
{
    control_t c = read();
    return (c.down || c.exit || c.ok || c.up);
}

control_t Control :: read()
{
    btnUp.read(); 
    btnDn.read(); 
    btnOK.read(); 
    btnEX.read();    

    control_t ctrl;

    ctrl.ok = btnOK.wasReleased();
    ctrl.up = btnUp.wasReleased();
    ctrl.down = btnDn.wasReleased();
    ctrl.exit = btnEX.wasReleased();

    ctrl.dwLong = btnDn.pressedFor(500);
    ctrl.upLong = btnUp.pressedFor(500);
    ctrl.okLong = btnOK.pressedFor(500);
    ctrl.exitLong = btnEX.pressedFor(500);
    
    bool isActive = ctrl.ok | ctrl.up | ctrl.down | ctrl.exit;

    if (isActive)
        inactiveTime = 0;     

    return ctrl;
}

void Control :: delay(unsigned int t)
{
    unsigned long stamp = millis();
    while (stamp + (unsigned long)t >= millis())
    {
        runRealTime();
    }
}

#ifdef USE_SERIAL_PRINTS
    void Control :: print(control_t c)
    {
        char buf[20];
        sprintf(buf, "ok=%u up=%u dw=%u ex=%u\n",
            (uint8_t)c.ok,
            (uint8_t)c.up,
            (uint8_t)c.down,
            (uint8_t)c.exit
        );
        Serial.print(buf);
    }
#endif