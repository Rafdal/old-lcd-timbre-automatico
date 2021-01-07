class MsgBox
{
private:
    uint8_t x, y;
    bool _ask();
public:
    MsgBox();
    ~MsgBox();

    void setCursor(uint8_t x, uint8_t y);
    void show(String text, uint16_t millis);
    void animate(String text, uint16_t millis, uint16_t delay);
    void animate1(String text, uint16_t millis, uint16_t delay);

    class Error
    {
    public:
        void show(String text, uint16_t millis);
        void nullPointer();
        void notInitialized();
        void fatal(String text);
    }error;
    
    void saved();

    bool ask(String question0);
    bool askSave();
    bool askSure(String quiestion0);
    bool ask(String question0, String question1);
    // bool ask(String question0, String question1, String question2);
};

MsgBox::MsgBox()
{
}

MsgBox::~MsgBox()
{
}

void MsgBox :: setCursor (uint8_t _x, uint8_t _y)
{
    x = _x;
    y = _y;
}

void MsgBox :: saved()
{
    show(F("Guardado!"),1200);
}

void MsgBox :: show (String text, uint16_t millis = 1000)
{
    lcd.clear();
    lcd.setCursor(x,y);

    lcd.print(text.substring(0,20));
    uint8_t len = text.length();

    if (len > 20)
    {
        lcd.setCursor(x,y+1);
        lcd.print(text.substring(20,40));
    }    
    if (len > 40)
    {
        lcd.setCursor(x,y+2);
        lcd.print(text.substring(40,60));
    }

    for (uint8_t i = 0; i < (1+len/20); i++)
    {
        ctrl.delay(millis);
        if (ctrl.someonePressed())
            return;
    }

    x=0;
    y=0;
}

void MsgBox :: animate(String text, uint16_t millis = 2500, uint16_t delay = 1000)
{
    String buffer;
    buffer.reserve(21);
    const uint8_t len = text.length();
    lcd.clear();

    for (uint8_t i = 0; i < len; i++)
    {
        buffer+= text[i];
        lcd.setCursor(x+(len-buffer.length())/2,y);
        lcd.print(buffer);
        ctrl.delay(millis/len);
        if (ctrl.someonePressed())
            return;
    }


    ctrl.delay(delay);
    x=0;
    y=0;
}

void MsgBox :: Error :: show(String text, uint16_t millis = 700)
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(F("ERROR!"));

    lcd.setCursor(0,2);
    
    lcd.print(text.substring(0,20));
    lcd.setCursor(0,3);
    if (text.length() > 20)
    {
        lcd.print(text.substring(20,40));
        ctrl.delay(millis);
    }    
    ctrl.delay(millis);
}

void MsgBox ::Error ::fatal(String text)
{
    for(;;)
    {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(F("ERROR FATAL!!!"));

        lcd.setCursor(0,2);
        
        lcd.print(text.substring(0,20));
        delay(1000);
    }
}

void MsgBox :: Error :: nullPointer()
{
    show(F("NULL pointer!"));
}

void MsgBox :: Error :: notInitialized()
{
    show(F("Not initialized!"));
}

void MsgBox :: animate1(String text, uint16_t millis = 2500, uint16_t delay = 1000)
{
    const uint8_t len = text.length();
    lcd.clear();

    for (uint8_t i = 0; i < len; i++)
    {
        lcd.setCursor(x+i,y);
        lcd.print(text[i]);
        ctrl.delay(millis/len);
        if (ctrl.someonePressed())
            return;
    }

    ctrl.delay(delay);
    x=0;
    y=0;
}

bool MsgBox :: _ask()
{
    ctrl.delay(500);

    ch.leftArrow(0,3);
    lcd.setCursor(2,3);
    lcd.print(F("NO"));

    ch.arrow(19,3);
    lcd.setCursor(16,3);
    lcd.print(F("SI"));
    while (true)
    {
        control_t ctrlData = ctrl.read();

        #ifdef USE_SERIAL_PRINTS
            ctrl.print(ctrlData);
        #endif

        if(ctrlData.exit)
            return false;
        if (ctrlData.ok)
            return true;

        ctrl.runRealTime();
        
        if (ctrl.isInactive())
            return false;
    }
}

bool MsgBox :: ask (String question0)
{
    lcd.clear();
        
    lcd.setCursor(0,0);
    lcd.print(question0);

    return _ask();
}

bool MsgBox :: askSave()
{
    lcd.clear();
        
    lcd.setCursor(0,0);
    lcd.print(F("Guardar?"));

    return _ask();
}

bool MsgBox :: askSure (String question0)
{
    lcd.clear();
        
    lcd.setCursor(0,0);
    lcd.print(question0);

    ch.ask(3,2);
    lcd.print(F("Estas seguro?"));
    ctrl.delay(500);

    return _ask();
}

bool MsgBox :: ask (String question0, String question1)
{
    lcd.clear();
        
    lcd.setCursor(0,0);
    lcd.print(question0);

    lcd.setCursor(0,1);
    lcd.print(question1);

    return _ask();
}

/* bool MsgBox :: ask (String question0, String question1, String question2)
{
    lcd.clear();
        
    lcd.setCursor(0,0);
    lcd.print(question0);

    lcd.setCursor(0,1);
    lcd.print(question1);

    lcd.setCursor(0,2);
    lcd.print(question2);

    return _ask();
} */