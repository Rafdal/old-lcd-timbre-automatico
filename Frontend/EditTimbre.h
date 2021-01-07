const PROGMEM header_t editTimbre_headers[] =
{
	"EDITAR ESTE TIMBRE",
	"Cambiar hora      >",
	"Cambiar dias      >",
	"Cambiar duracion  >",
	"Eliminar"
};

const PROGMEM header_t insert_timbre_title = "Nuevo timbre";

Menu editTimbre;
NumBox insertTimbre;

#include "CambiarHora.h"
#include "EliminarTimbre.h"
#include "CambiarDias.h"
#include "CambDuracion.h"

layout_t init_insert_timbre()
{
	insertTimbre.begin(2, insert_timbre_title, 3, ':');
	insertTimbre.add(23);
	insertTimbre.add(59);
}

layout_t init_edit_timbre()
{
	editTimbre.begin(4, editTimbre_headers);
	editTimbre.add(run_cambiar_hora);
	editTimbre.add(run_camb_dias);
	editTimbre.add(run_cambiar_duracion);
	editTimbre.add(run_eliminar_timbre);

	init_insert_timbre();
	init_cambiar_hora();
	init_camb_dias();
	init_cambiar_duracion();
}

void run_edit_timbre()
{
	timbre_t t = timbre.get(TimbreList.idx);
	if (t.duration > 0)
		editTimbre.run();
	else if(MessageBox.ask(F("Crear nuevo timbre?")))
	{
		insertTimbre.value[0] = 12;
		do
		{
			insertTimbre.value[0] = timbre.rtc.hour;
			insertTimbre.value[1] = timbre.rtc.minute;
			insertTimbre.run();
			if (insertTimbre.save)
			{
				tdata_t td;
				td.data[0] = 5;
				for (uint8_t i = 0; i < 2; i++)
					td.data[1+i] = insertTimbre.value[i];
				
				td.data[3] = B10000000;

				if (timbre.exist(td))
				{
					MessageBox.error.show(F("Ya existe 1 timbre  en ese horario"));	
					insertTimbre.status = true;
				}
				else
				{
					timbre.set(TimbreList.idx, td);
					MessageBox.saved();
				}				
			}  
		} while (insertTimbre.status);
	}
}