const PROGMEM header_t cambiar_dias_headers[] =
{
    "CAMBIAR DIAS",
    "Lunes",
    "Martes",
    "Miercoles",
    "Jueves",
    "Viernes",
    "Sabado",
    "Domingo"
};

ParamList cambDias;

layout_t init_camb_dias()
{
    cambDias.begin(7, cambiar_dias_headers);
    cambDias.add(1);
    cambDias.add(1);
    cambDias.add(1);
    cambDias.add(1);
    cambDias.add(1);
    cambDias.add(1);
    cambDias.add(1);
}

void run_camb_dias()
{
    MessageBox.setCursor(0,0);
    MessageBox.show(F("Indique en que dias quiere que el timbrese accione:"),50);
    lastMillis = millis();
    while (millis() - lastMillis < 3200)
    {
        if (ctrl.someonePressed())
            break;
    }
    
    timbre_t t = timbre.get(TimbreList.idx);
    if (t.isEveryday)
    {
        for (uint8_t i = 0; i < 5; i++)
            cambDias.value[i] = 1;
        for (uint8_t i = 5; i < 7; i++)
            cambDias.value[i] = 0;
    }
    else
    {
        for (uint8_t i = 0; i < 7; i++)
            cambDias.value[i] = (uint8_t)t.day[i];
    }

    cambDias.run();

    if (cambDias.save)
    {
        t.isEveryday = true;

        for (uint8_t i = 0; i < 5; i++)
            t.isEveryday &= (bool)cambDias.value[i];
        for (uint8_t i = 5; i < 7; i++)
            t.isEveryday &= !((bool)cambDias.value[i]);

        if (!t.isEveryday)
        {
            for (uint8_t i = 0; i < 7; i++)
                t.day[i] = cambDias.value[i];      
        }
        
        timbre.set(TimbreList.idx, t);
        MessageBox.saved();
    }
}