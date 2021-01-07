const PROGMEM header_t config_headers[] =
{
    "CONFIGURACION",
    "Editar hora       >",
    "Editar fecha      >",
    "Edit dia de semana>",
    "Reiniciar"
};

Menu config;

#include "EditHoraRTC.h"
#include "EditFechaRTC.h"
#include "EditDiaRTC.h"

void run_reiniciar()
{
    asm
    (
        "jmp 0x0000"
    );
}

layout_t init_config()
{
    config.begin(4, config_headers);
    config.add(run_editar_hora_rtc);
    config.add(run_editar_fecha_rtc);
    config.add(run_editar_dia_rtc);
    config.add(run_reiniciar);

    init_editar_hora();
    init_editar_fecha();
    init_editar_dia();
}

void run_config()
{
    config.run();
}