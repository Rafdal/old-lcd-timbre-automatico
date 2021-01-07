const PROGMEM header_t crear_timbre_headers[] =
{
    "NUEVO TIMBRE",
    "Timbre rapido     >",
    // "Timbre avanzado   >"
};

const PROGMEM header_t timbre_rapido_title = "Timbre rapido";

Menu crearTimbre;
NumBox timbreRapido;

// #include "(WIP)TimbreAvanzado.h"

void runTimbreRapido()
{
    timbreRapido.value[0] = 12;
    do
    {
        timbreRapido.value[0] = timbre.rtc.hour;
        timbreRapido.value[1] = timbre.rtc.minute;
        timbreRapido.run();
        if (timbreRapido.save)
        {
            uint8_t resultCode = timbre.add(timbreRapido.value[0], timbreRapido.value[1]);
            if (resultCode == 0) // #0 
            {
                MessageBox.error.show(F("Ya existe 1 timbre  en ese horario"));
                timbreRapido.status = true;
            }
            else
            {
                if(resultCode == 1) // #1 
                    MessageBox.error.show(F("Se ha alcanzado el  N maximo de timbres"));
                else // #2 
                    MessageBox.saved();
                break;
            }
        }  
    } while (timbreRapido.status);
}

layout_t init_crear_timbre()
{
    timbreRapido.begin(2, timbre_rapido_title, 3, ':');
    timbreRapido.add(23);
    timbreRapido.add(59);

    crearTimbre.begin(1, crear_timbre_headers);
    crearTimbre.add(runTimbreRapido);
    // crearTimbre.add(NULL);
}

void run_crear_timbre()
{
    crearTimbre.run();
}