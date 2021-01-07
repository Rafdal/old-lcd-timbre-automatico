// typedef void (*void_event_t)(void);

class ParamList
{
private:
    uint8_t max_elements;
    uint8_t startPos;
    uint8_t range;

    uint8_t *_max;
    uint8_t *_min;
    const header_t *headers;
    bool initialized;
    uint8_t offset;
    uint8_t arrowPos;
    uint8_t currentElement;
    void _boolSwitch();
    void _increase();
    void _decrease();
    void _exit();

        uint8_t addCount;
public:
    uint8_t *value;
    bool status;
    bool save;
    ParamList();
    ~ParamList();
    // Config Methods
    void begin(uint8_t _max_elements, const header_t *_headers, uint8_t _startPos, uint8_t _range);

    void add(uint8_t max, uint8_t min, uint8_t defaultVal);
    void run();
    void focus();

    // Operation Methods
    void up();
    void down();
    void right();
    void left();

    void show();
};

// #    BEGIN      

ParamList::ParamList()
{   
}

ParamList::~ParamList()
{
    if(value)
        delete[] value;
    if(_max)
        delete[] _max;
    if(_min)
        delete[] _min;    
}

void ParamList :: _boolSwitch()
{
    value[currentElement-1] = value[currentElement-1] ^ 1;
}

void ParamList :: begin(uint8_t _max_elements, const header_t *_headers, uint8_t _startPos = 1, uint8_t _range = 3)
{
    if(value)
        delete[] value;
    if(_max)
        delete[] _max;
    if(_min)
        delete[] _min;   

    value = new uint8_t[_max_elements];
    _max = new uint8_t[_max_elements];
    _min = new uint8_t[_max_elements];
    headers = _headers;

    range = _range;
    startPos = _startPos;
    max_elements = _max_elements + 1;

    initialized = true;
}

void ParamList :: _exit()
{
    if (MessageBox.askSave())
        save = true;

    offset = 0;
    arrowPos = 0;
    currentElement = 0;
    status = false;
}

void ParamList :: down()
{
    if (status)
    {
        if (currentElement < max_elements-1)
        {
            currentElement++;

            if (arrowPos >= range-1)
            {
                if (offset < max_elements-range)
                    offset++;
            }
            else
                arrowPos++;
            show();
        }
    }
}

void ParamList :: up()
{
    if (status)
    {
        if (currentElement > 0)
        {
            currentElement--;

            if (arrowPos > 0)
                arrowPos--;
            else
            {
                if (offset > 0)
                    offset--;
            }
            show();
        }
    }
}

void ParamList :: right()
{
    if (status)
    {
        if (currentElement == 0)
        {
            _exit();
        }
        else
        {
            if(value[currentElement-1] < _max[currentElement-1])
                value[currentElement-1]++;
            else
                value[currentElement-1] = _min[currentElement-1];
            show();
        }
    }
}

void ParamList :: left()
{
    if (status)
    {
        if (currentElement != 0)
        {
            if(value[currentElement-1] > _min[currentElement-1])
                value[currentElement-1]--;
            else
                value[currentElement-1] = _max[currentElement-1];
            show();
        }
        else
        {
            _exit();
        }
        
    }
}

void ParamList :: add (uint8_t max, uint8_t min = 0, uint8_t defaultVal = 0)
{
    if (max_elements > addCount+1)
    {
        value[addCount] = defaultVal;
        _max[addCount] = max;
        _min[addCount] = min;
        addCount++;
    }
}

void ParamList :: run ()
{   
    if(initialized)
    {
        save = false;
        focus();
        while (status)
        {
            control_t thisCtrl = ctrl.read();
            if (thisCtrl.down)
                down();
            if (thisCtrl.up)
                up();
            if (thisCtrl.exit)
                left();
            if (thisCtrl.ok)
                right();

            while (thisCtrl.exitLong)
            {
                left();
                ctrl.delay(175);
                thisCtrl = ctrl.read();
            }
            while (thisCtrl.okLong)
            {
                right();
                ctrl.delay(175);
                thisCtrl = ctrl.read();
            }
            ctrl.runRealTime();

            if (ctrl.isInactive())
                _exit();
        } 
    }
    else
        MessageBox.error.notInitialized();
}

void ParamList :: focus()
{
    show();
    status = true;
}

void ParamList :: show ()
{
    lcd.clear();
    // ch.clear();
    memory.set(headers);
    if (memory.sizeOf(0) > 0)
    {
        lcd.setCursor(0,0);
        lcd.print(memory.take(0));
    }

    for (uint8_t i = offset; i < max_elements && i < range+offset; i++)
    {
        lcd.setCursor(1, startPos+i-offset);
        if (i == 0)
            lcd.print(F("SALIR"));
        else
        {            
            lcd.print(' ');
            lcd.print(memory.take(i));

            lcd.setCursor(11, startPos+i-offset);
            lcd.print('=');

            lcd.setCursor(15, startPos+i-offset);
            if (_max[i-1] - _min[i-1] == 1)
            {
                if (value[i-1])
                    lcd.print(F("SI"));
                else
                    lcd.print(F("NO"));
            }
            else            
                lcd.print(value[i-1]);
        }
    }
    uint8_t y = startPos+arrowPos;
    if (currentElement==0)
        ch.arrow(0, y);
    else
    {
        ch.leftArrow(13, y);
        ch.arrow(19,y);
    }
}