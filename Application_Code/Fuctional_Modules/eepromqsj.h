#include <core_common.h>
#include <board_includes.h>
#include <device_includes.h>
#include <math_includes.h>
#include <basicflight_includes.h>
#include <app_includes.h>
#include <OS_includes.h>

void eeprom_read(int select);
void eeprom_write(int select);
void eeprom_PID_INT(void);
void eeprom_PID_STORE(void);
void read_fix(void);
void write_fix(void);
//extern int16 eeprom_readdate[9];
extern uint32_t eeprom_readdate[9];