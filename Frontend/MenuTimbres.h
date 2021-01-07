const PROGMEM header_t MenuTimbres_headers[] =
{
	"TIMBRES",
	"Nuevo             >",
	"Ver lista         >",
	"Borrar todos"
};

Menu MenuTimbres; 

#include "TableTimbres.h"
#include "BorrarTimbres.h"
#include "CrearTimbre.h"

layout_t init_menu_timbres()
{
	MenuTimbres.begin(3, MenuTimbres_headers);
	MenuTimbres.add(run_crear_timbre);
	MenuTimbres.add(runTableTimbres);
	MenuTimbres.add(runBorrarTimbres);

	init_table_timbres();
}

void runMenuTimbres()
{
	MenuTimbres.run();
}