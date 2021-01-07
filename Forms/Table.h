// typedef void (*void_event_t)(void);

typedef char row_t[20];

class Table
{
private:
    uint8_t max_elements;
    uint8_t startPos;

    void_event_t event;
    void_event_t load_data_event;
    const char* title;

    uint8_t arrowPos;
    row_t row[3];
    bool initialized;
public:
    uint8_t range;
    uint8_t offset; // First element
    uint8_t idx; // Current element


    bool status;

    Table();
    ~Table();
    // Config Methods
    void begin(uint8_t _max_elements, void_event_t _load_data_event, const char *_title, uint8_t _startPos, uint8_t _range);
    void attachEvent(void_event_t ev);
    void setHeader(uint8_t _idx, char* _header);


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

Table::Table()
{   
}
Table::~Table()
{
}

void Table :: begin(uint8_t _max_elements, void_event_t _load_data_event, const char *_title, uint8_t _startPos = 1, uint8_t _range = 3)
{
    for (uint8_t i = 0; i < _range; i++)
        row[i][0] = '\0';

    load_data_event = _load_data_event;
    max_elements = _max_elements;
    startPos = _startPos;
    title = _title;
    range = _range;
    initialized = true;
    event = NULL;
}

void Table :: setHeader(uint8_t _idx, row_t _header)
{
    if (_idx < range)
        memcpy(&(row[_idx]), _header, sizeof(row_t));
        // strcpy(&(row[_idx]), );
}

void Table :: down()
{
    if (status)
    {
        if (idx < max_elements-1)
        {
            idx++;

            if (arrowPos >= range-1)
            {
                if (offset < max_elements-range)
                    offset++;
            }
            else
                arrowPos++;
        }
        else
        {
            arrowPos = 0;
            idx = 0;
            offset = 0;
        }
        show();
    }
}

void Table :: up()
{
    if (status)
    {
        if (idx > 0)
        {
            idx--;

            if (arrowPos > 0)
                arrowPos--;
            else
            {
                if (offset > 0)
                    offset--;
            }
        }
        else
        {
            arrowPos =range-1;
            idx = max_elements-1;
            offset = idx-range+1;
        }
        show();
    }
}

void Table :: click()
{
    if (status)
    {
        if (event != NULL)
            (*event)();
        else
            MessageBox.error.nullPointer();
        show();
    }
}

void Table :: exit()
{
    if (status)
    {
        offset = 0;
        arrowPos = 0;
        idx = 0;
        status = false;
    }
}

void Table :: attachEvent (void_event_t ev)
{
        event = ev;
}

void Table :: run ()
{   
    if(initialized)
    {
        focus();
        while (status)
        {
            control_t thisCtrl = ctrl.read();
            if (thisCtrl.down)
                down();
            if (thisCtrl.up)
                up();
            if (thisCtrl.exit)
                exit();
            if (thisCtrl.ok)
                click();

            while (thisCtrl.upLong)
            {
                up();
                ctrl.delay(175);
                thisCtrl = ctrl.read();
            }
            while (thisCtrl.dwLong)
            {
                down();
                ctrl.delay(175);
                thisCtrl = ctrl.read();
            }

            ctrl.runRealTime();
        }
    }
    else
        MessageBox.error.notInitialized();
}

void Table :: focus()
{
    show();
    status = true;
}

void Table :: show ()
{
    if (load_data_event != NULL)
        (*load_data_event)();
    else
        MessageBox.error.nullPointer();

    lcd.clear();

    memory.set(title);
    if (memory.sizeOf() > 0)
    {
        lcd.setCursor(0,0);
        lcd.print(memory.take());
    }

    char buffer[4];
    for (uint8_t i = 0; i < range; i++)
    {
        sprintf(buffer, "%2u=", 1+offset+i);
        lcd.setCursor(1, startPos+i);
        lcd.print(buffer);
        lcd.print(row[i]);
    }
    ch.arrow(0,startPos+arrowPos);
}