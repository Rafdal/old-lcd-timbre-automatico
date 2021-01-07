const PROGMEM header_t camb_dur_title = "Duracion (segundos)"; 

NumBox CambDur;

layout_t init_cambiar_duracion()
{
    CambDur.begin(1, camb_dur_title, 3);
    CambDur.add(12,1,5);
}

void run_cambiar_duracion()
{
    timbre_t t = timbre.get(TimbreList.idx);
    CambDur.value[0] = t.duration;
    CambDur.run();
    if (CambDur.save)
    {
        t.duration = CambDur.value[0];
        timbre.set(TimbreList.idx, t);
        MessageBox.saved();
    }
}