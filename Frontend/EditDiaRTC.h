const PROGMEM header_t dia_title = "Que dia es hoy?";

NumBox EditDiaRTC;

layout_t init_editar_dia()
{
    EditDiaRTC.begin(1, dia_title, 3);
	EditDiaRTC.add(n_dia, 6);
}

void run_editar_dia_rtc()
{
	timbre.rtc.read(); // Leer el Reloj RTC
	
	// Cargo los datos en la pantalla y la ejecuto
	EditDiaRTC.value[0] = timbre.rtc.wday-1;
	
	EditDiaRTC.run();
	if (EditDiaRTC.save)
	{
		// Guardo los datos en el reloj y actualizo la hora
		timbre.rtc.read();
		timbre.rtc.wday = 1+EditDiaRTC.value[0]; 
		timbre.rtc.update();
		MessageBox.saved();
	}
}