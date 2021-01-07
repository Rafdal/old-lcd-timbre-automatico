const PROGMEM header_t menu_headers[] =
{
	"MENU",
	"Cambiar modo",
	"Timbres           >",
	"Configuracion     >"
};

Menu Home;

#include "MenuTimbres.h"
#include "Config.h"

void run_cambiar_modo()
{
	if(timbre.switchMode())
		MessageBox.show(F("Modo cambiado!"), 1500);
	else
		MessageBox.error.show(F("Sin Pila RTC"),1200);
	Home.status = false;
}

layout_t init_menu()
{
	Home.begin(3, menu_headers);
	Home.add(run_cambiar_modo);
	Home.add(runMenuTimbres);
	Home.add(run_config);

	init_menu_timbres();
	init_config();
}