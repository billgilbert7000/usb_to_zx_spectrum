


#include "inttypes.h"

void keyboard (uint8_t const *report, uint16_t len);
void scancode_s(uint8_t code);
void key_on(uint8_t code); // клавиша нажата
void key_off(uint8_t code);// клавиша отпущена  
void SetAddr(uint8_t addr) ;
