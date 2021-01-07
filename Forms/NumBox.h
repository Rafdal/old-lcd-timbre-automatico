// typedef void (*void_event_t)(void);

class NumBox
{
private:

    uint8_t max_elements=1;
    uint8_t *_max;
    uint8_t *_min;
    const char* title;
    const header_t *personalized_options;
    uint8_t currentElement;
    uint8_t offset;
    uint8_t addCount;

    // Control *local_ctrl; // Main control pointer

    // uint8_t offset = 0;
    // uint8_t arrowPos = 0;
    // uint8_t currentElement = 0;
    char separator;
    bool initialized;
public:
    bool status;
    uint8_t *value;
    bool save = false;


    NumBox();
    ~NumBox();
    // Config Methods

    void run();
    void focus();

    void begin(uint8_t _max_elements, const char* _title, uint8_t _offset, char sep);
    void add(uint8_t mx, uint8_t mn, uint8_t def);
    void add(const header_t *_personalized_options, uint8_t _max, uint8_t _min);

    // Operation Methods
    void _exit();
    void up();
    void down();
    void right();
    void left();

    void show();
};

// #    BEGIN      

NumBox::NumBox()
{
}

NumBox::~NumBox()
{   
    if(value)
        delete[] value;
    if(_max)
        delete[] _max;
    if(_min)
        delete[] _min;  
}

void NumBox::begin(uint8_t _max_elements, const char* _title, uint8_t _offset = 0, char sep = ' ')
{
    offset = _offset;
    value = new uint8_t[_max_elements];
    _max = new uint8_t[_max_elements];
    _min = new uint8_t[_max_elements];
    max_elements = _max_elements + 1;
    separator = sep;
    title = _title;
    initialized=true;
}

void NumBox::add(uint8_t mx = 99, uint8_t mn = 0, uint8_t def = 0)
{
    if (max_elements > addCount+1)
    {
        value[addCount] = def;
        _max[addCount] = mx;
        _min[addCount] = mn;
        addCount++;
    }
}

void NumBox :: add(const header_t *_personalized_options, uint8_t _max_, uint8_t _min_ = 0)
{
    if (max_elements > addCount+1 && max_elements == 2)
    {
        personalized_options = _personalized_options;
        _max[0] = _max_;
        _min[0] = _min_;
        addCount = 0xff;
    }
    while (max_elements != 2)
    {
        MessageBox.error.fatal(F("NumBox:100"));
    }
}


void NumBox :: _exit()
{
    if (MessageBox.askSave())
    {
        save = true;
    }
    
    currentElement = 0;
    status = false;
}

void NumBox :: down()
{
    if (status)
    {
        if (currentElement != 0)
        {
            uint8_t thisElement = currentElement-1;
            if (value[thisElement] > _min[thisElement])
                value[thisElement] -= 1;
            else
                value[thisElement] = _max[thisElement];
        }
        show();
    }     
}

void NumBox :: up()
{
    if (status)
    {
        if (currentElement != 0)
        {
            uint8_t thisElement = currentElement-1;
            if (value[thisElement] < _max[thisElement])
                value[thisElement] += 1;
            else
                value[thisElement] = _min[thisElement];
            show();
        }
    }
}

void NumBox :: right()
{
    if (status)
    {
        if (currentElement < max_elements-1)
            currentElement++;

        show();
    }
}

void NumBox :: left()
{
    if (status)
    {
        if (currentElement > 0)
            currentElement--;
        else
            _exit();
            // currentElement = max_elements-1;
        show();
    }
}

void NumBox :: run ()
{   
    if (initialized)
    {
        currentElement = 1;
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

            while (thisCtrl.dwLong)
            {
                down();
                ctrl.delay(175);
                thisCtrl = ctrl.read();
            }
            while (thisCtrl.upLong)
            {
                up();
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

void NumBox :: focus()
{
    show();
    status = true;
}

void NumBox :: show ()
{
    lcd.clear();

    memory.set(title);
    uint8_t len = (uint8_t) memory.sizeOf();
    if (len > 0 && len <= 20)
    {
        lcd.setCursor(1,0);
        lcd.print(memory.take());
    }

    // lcd.setCursor(0,1);
    // lcd.print(F("VOLVER"));
    // lcd.setCursor(0,2);
    // lcd.print(F("GUARDAR"));
    const uint8_t global_offset = 3;
    if(addCount != 0xff)
    {
        for (uint8_t i = 0; i < max_elements-1; i++)
        {
            lcd.setCursor(offset+global_offset+i*3,2);
            char val[3];
            sprintf(val, "%02u", value[i]);
            lcd.print(val);
            if (max_elements-2-i != 0)
            {
                lcd.setCursor(offset+global_offset+2+i*3,2);
                lcd.print(separator);
            }
        }
    }
    else
    {
        memory.set(personalized_options);
        lcd.setCursor(offset+global_offset,2);
        lcd.print(memory.take(value[0]));
    }
    
    uint8_t dY = 3,x = 2;
    if (currentElement > 0)
    {
        x = offset+global_offset+(currentElement-1)*3;
        if(addCount==0xff)
            x+=2;
        dY = 2;
        lcd.setCursor(0,0);
        lcd.print(' ');
        ch.bigArrowUp(x, 3-dY);
        ch.bigArrowDown(x, 3);
    }
    else
    {
        ch.leftArrow(0,0);
    }
}