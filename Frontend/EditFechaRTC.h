const PROGMEM header_t fecha_title = "Cambiar fecha reloj";

NumBox EditFechaRTC;

layout_t init_editar_fecha()
{
    EditFechaRTC.begin(3, fecha_title, 3, '/');
	EditFechaRTC.add(31,1);
	EditFechaRTC.add(12,1);
	EditFechaRTC.add(59);
}

void run_editar_fecha_rtc()
{
	timbre.rtc.read(); // Leer el Reloj RTC
	
	// Cargo los datos en la pantalla y la ejecuto
	EditFechaRTC.value[0] = timbre.rtc.mday;
	EditFechaRTC.value[1] = timbre.rtc.month;
	EditFechaRTC.value[2] = timbre.rtc.year;
	
	EditFechaRTC.run();
	if (EditFechaRTC.save)
	{
		// Guardo los datos en el reloj y actualizo la hora
		timbre.rtc.read();
		timbre.rtc.mday = EditFechaRTC.value[0]; 
		timbre.rtc.month = EditFechaRTC.value[1]; 
		timbre.rtc.year = EditFechaRTC.value[2]; 	
		timbre.rtc.update();
		MessageBox.saved();
	}
}