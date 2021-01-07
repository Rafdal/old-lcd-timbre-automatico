void runBorrarTimbres()
{
    if (MessageBox.askSure(F("Borrar timbres")))
    {
        timbre.clear();
        MessageBox.show(F("Borrados"));
    }
}