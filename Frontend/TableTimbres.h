const PROGMEM header_t timbre_title = "Lista de timbres";

Table TimbreList;

#include "EditTimbre.h"

void cargar_timbres()
{
	for (uint8_t i = 0; i < TimbreList.range; i++)
	{
		TimbreList.setHeader(i, timbre.toStr(i + TimbreList.offset));
	}
}

layout_t init_table_timbres()
{
    TimbreList.begin(timbre.length(), cargar_timbres, timbre_title);
	TimbreList.attachEvent(run_edit_timbre);
	
	init_edit_timbre();
}

void runTableTimbres()
{
	TimbreList.run();
}