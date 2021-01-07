const PROGMEM header_t cambHora_title = "Cambiar hora";

NumBox cambHora;

layout_t init_cambiar_hora()
{
    cambHora.begin(2, cambHora_title, 3, ':');
    cambHora.add(23);
    cambHora.add(59);
}

void run_cambiar_hora()
{
    timbre_t t = timbre.get(TimbreList.idx);
    cambHora.value[0] = t.hour;
    cambHora.value[1] = t.minute;
    cambHora.run();
    if (cambHora.save)
    {
        t.hour = cambHora.value[0];
        t.minute = cambHora.value[1];
        timbre.set(TimbreList.idx, t);
        MessageBox.saved();
    }
}