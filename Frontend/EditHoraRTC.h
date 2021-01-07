const PROGMEM header_t hora_title = "Cambiar hora reloj";

NumBox EditHoraRTC;

layout_t init_editar_hora()
{
    EditHoraRTC.begin(3, hora_title, 3, ':');
	EditHoraRTC.add(23);
	EditHoraRTC.add(59);
	EditHoraRTC.add(59);
}

void run_editar_hora_rtc()
{
	timbre.rtc.read(); // Leer el Reloj RTC
	
	// Cargo los datos en la pantalla y la ejecuto
	EditHoraRTC.value[0] = timbre.rtc.hour;
	EditHoraRTC.value[1] = timbre.rtc.minute;
	EditHoraRTC.value[2] = timbre.rtc.second;

	EditHoraRTC.run();
	if (EditHoraRTC.save)
	{
		// Guardo los datos en el reloj y actualizo la hora
		timbre.rtc.read();
		timbre.rtc.hour = EditHoraRTC.value[0]; 
		timbre.rtc.minute = EditHoraRTC.value[1]; 
		timbre.rtc.second = EditHoraRTC.value[2]; 	
		timbre.rtc.update();
		MessageBox.saved();
	}
}