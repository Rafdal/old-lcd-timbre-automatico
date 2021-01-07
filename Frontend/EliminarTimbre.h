void run_eliminar_timbre()
{
    if(MessageBox.askSure(F("Eliminar timbre")))
    {
        timbre.del(TimbreList.idx);
        MessageBox.show(F("  Eliminado"));
        editTimbre.status = false;
    }
}