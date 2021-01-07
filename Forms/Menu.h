// typedef void (*void_event_t)(void);

class Menu
{
private:
    uint8_t max_elements;
    uint8_t startPos;
    uint8_t range;

    void_event_t *event;
    void_event_t *init_event;
    const header_t *headers;

    uint8_t offset = 0;
    uint8_t arrowPos = 0;
    uint8_t currentElement = 0;
    bool initialized;

        uint8_t addCount;
public:
    bool status = false;
    Menu();
    ~Menu();
    // Config Methods

    void begin(uint8_t _max_elements, const header_t *_headers, uint8_t _startPos, uint8_t _range);
    void add(void_event_t ev);

    void run();
    void focus();

    // Operation Methods
    void up();
    void down();
    void click();
    void exit();

    void show();
};

// #    BEGIN      

Menu::Menu()
{   
}
Menu::~Menu()
{
    if(event)
        delete[] event;
}

void Menu :: begin(uint8_t _max_elements, const header_t *_headers, uint8_t _startPos = 1, uint8_t _range = 3)
{
    if(event)
        delete[] event;

    max_elements = _max_elements;
    event = new void_event_t[_max_elements];
    headers = _headers;

    for (uint8_t i = 0; i < _max_elements; i++)
        event[i] = NULL;

    startPos = _startPos;
    range = _range;
    initialized = true;
}

void Menu :: down()
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

void Menu :: up()
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

void Menu :: click()
{
    if (status)
    {
        if(event[currentElement] != NULL)
        {
            (*event[currentElement])();
        }
        else
            MessageBox.error.nullPointer();
        show();
    }
}

void Menu :: exit()
{
    if (status)
    {
        offset = 0;
        arrowPos = 0;
        currentElement = 0;
        status = false;
    }
}

void Menu :: add (void_event_t ev)
{
    if (max_elements > addCount)
    {
        event[addCount] = ev;
        addCount++;
    }
}

void Menu :: run ()
{   
    if(initialized)
    {
        focus();
        while (status)
        {
            control_t thisCtrl = ctrl.read(); // Leo los controles

            if (thisCtrl.down)
                down(); // bajar
            if (thisCtrl.up)
                up(); // subir
            if (thisCtrl.exit || ctrl.isInactive())
                exit(); // salir
            if (thisCtrl.ok)
                click(); // seleccionar

            ctrl.runRealTime(); // Ejecutar intervalos de tiempo real
        }
    }
    else
        MessageBox.error.notInitialized();
}

void Menu :: focus()
{
    show();
    status = true;
}

void Menu :: show ()
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
        lcd.print(memory.take(1+i));

    }
    ch.arrow(0,startPos+arrowPos);
}