#include "init.h"


uint8_t tab_key[128] = {0x00};// таблица нажатых клавиш
uint8_t tab_key_old[128] = {0x00};// таблица предыдушего нажатия клавиш

//----------------------------------------------------------
 // table usb
#define NC 0xFF
//      ряд Y0
#define CS 0x00 //CAPS SHIFT
#define _Z 0x01
#define _X 0x02
#define _C 0x03
#define _V 0x04
//      ряд Y1
#define _A 0x10
#define _S 0x11
#define _D 0x12
#define _F 0x13
#define _G 0x14
//      ряд Y2
#define _Q 0x20
#define _W 0x21
#define _E 0x22
#define _R 0x23
#define _T 0x24
//      ряд Y3
#define _1 0x30
#define _2 0x31
#define _3 0x32
#define _4 0x33
#define _5 0x34
//      ряд Y4
#define _0 0x40
#define _9 0x41
#define _8 0x42
#define _7 0x43
#define _6 0x44
//      ряд Y5
#define _P 0x50
#define _O 0x51
#define _I 0x52
#define _U 0x53
#define _Y 0x54
//      ряд Y6
#define EN 0x60 // ENTER
#define _L 0x61
#define _K 0x62
#define _J 0x63
#define _H 0x64
//      ряд Y7
#define SP 0x70 //SPACE
#define SS 0x71 //SYMBOL SHIFT
#define _M 0x72
#define _N 0x73
#define _B 0x74


uint8_t table_key_zx[256] = 
 {
  NC, NC,/*00 Reserved*/
  NC, NC,/*01 Keyboard ErrorRollOver*/
  NC, NC,/*02 Keyboard POSTFail*/
  NC, NC,/*03 Keyboard ErrorUndefined[*/
  _A, NC,/*04 A*/
  _B, NC,/*05 B*/
  _C, NC,/*06 C*/
  _D, NC,/*07 D*/
  _E, NC,/*08 E*/
  _F, NC,/*09 F*/
  _G, NC,/*0A G*/
  _H, NC,/*0B H*/    
  _I, NC,/*0C I*/
  _J, NC,/*0D J*/
  _K, NC,/*0E K*/
  _L, NC,/*0F L*/
  
  _M, NC,/*10 M*/
  _N, NC,/*11 N*/
  _O, NC,/*12 O*/
  _P, NC,/*13 P*/
  _Q, NC,/*14 Q*/
  _R, NC,/*15 R*/
  _S, NC,/*16 S*/
  _T, NC,/*17 T*/  
  _U, NC,/*18 U*/
  _V, NC,/*19 V*/
  _W, NC,/*1A W*/
  _X, NC,/*1B X*/
  _Y, NC,/*1C Y*/
  _Z, NC,/*1D Z*/
  _1, NC,/*1E 1*/
  _2, NC,/*1F 2*/   

  _3, NC,/*20 3*/
  _4, NC,/*21 4*/
  _5, NC,/*22 5*/
  _6, NC,/*23 6*/
  _7, NC,/*24 7*/
  _8, NC,/*25 8*/
  _9, NC,/*26 9*/
  _0, NC,/*27 0*/
  EN, NC,/*28 Enter*/
  SP, CS,/*29 Escape/ Break CS+Space*/
  _0, CS,/*2A Keyboard Delete (Backspace)  CS+0*/
  _1, CS,/*2B Tab / Edit CS+1*/
  SP, NC,/*2C Space*/    
  _J, SS,/*2D -      SS+J*/
  _K, SS,/*2E +      SS+K*/
  NC, NC,/*2F [*/ 

  _L, SS,/*30 SS+L*/
  NC, NC,/*31 \*/
  NC, NC,/*32 `*/
  _O, SS,/*33 ; SS+O*/
  _P, SS,/*34 SS+P  "*/
  NC, NC,/*35 Keyboard Grave Accent and Tilde*/
  _M, SS,/*36 , SS+M*/
  _N, SS,/*37 . SS+N*/
  _C, SS,/*38 ? SS+C*/
  _2, CS,/*39 Caps Lock  CS+2*/
  NC, NC,/*3A F1*/
  NC, NC,/*3B F2*/    
  NC, NC,/*3C F3*/
  NC, NC,/*3D F4*/
  NC, NC,/*3E F5*/
  NC, NC,/*3F F6*/

  NC, NC,/*40 F7 */
  NC, NC,/*41 F8 */
  NC, NC,/*42 F9 */
  NC, NC,/*43 F10 */
  NC, NC,/*44 F11 */
  NC, NC,/*45 F12 */
  _9, CS,/*46 PrintScreen Graph Mode CS+9*/
  NC, NC,/*47 Scroll Lock*/
  NC, NC,/*48 Pause */
  NC, NC,/*49 Insert */
  NC, NC,/*4A Home */
  _3, CS,/*4B PageUp CS+3 True Video*/    
  NC, NC,/*4C Del */
  NC, NC,/*4D End */
  _4, CS,/*4E PageDown CS+4 Inv Video*/
  _8, CS,/*4F RightArrow CS+8*/
  
  _5, CS,/*50 LeftArrow CS+5*/
  _6, CS,/*51 DownArrow  CS+6*/
  _7, CS,/*52 UpArrow CS+7*/
  NC, NC,/*53 Keypad Num Lock and Clear*/
  _V, SS,/*54 Keypad / SS+V*/
  NC, NC,/*55 Keypad * */
  _I, SS,/*56 Keypad - */
  _K, SS,/*57 Keypad + */  
  EN, NC,/*58 Keypad Enter */
  _1, NC,/*59 Keypad 1 */
  _2, NC,/*5A Keypad 2 */
  _3, NC,/*5B Keypad 3 */
  _4, NC,/*5C Keypad 4 */
  _5, NC,/*5D Keypad 5 */
  _6, NC,/*5E Keypad 6 */
  _7, NC,/*5F Keypad 7 */ 

  _8, NC,/*60 Keypad 8 */
  _9, NC,/*61 Keypad 9 */
  _0, NC,/*62 Keypad 0 */
  NC, NC,/*63 Keypad Del . */
  NC, NC,/*64*/
  NC, NC,/*65*/
  NC, NC,/*66*/
  NC, NC,/*67 Keypad =*/
  NC, NC,/*68 F13*/
  NC, NC,/*69 F14*/
  NC, NC,/*6A F15*/
  NC, NC,/*6B F16*/    
  NC, NC,/*6C F17*/
  NC, NC,/*6D F18*/
  NC, NC,/*6E F19*/
  NC, NC,/*6F F20*/ 
/* клавиши ALT CTRL SHIFT  специально для ZX  */
  SS, CS,/*70 F21 ALT   Ext.Mode SS+CS  */
  CS, NC,/*71 F22 CTRL  / CS ZX  */ 
  SS, NC,/*72 F23 SHIFT / SS ZX  */
  NC, NC,/*73 F24 WIN */
  NC, NC,/*74*/
  NC, NC,/*75*/
  NC, NC,/*76*/
  NC, NC,/*77*/
  NC, NC,/*78*/
  NC, NC,/*79*/
  NC, NC,/*7A*/
  NC, NC,/*7B*/    
  NC, NC,/*7C*/
  NC, NC,/*7D*/
  NC, NC,/*7E*/
  NC, NC,/*7F*/   
 };
//------------------------------------------------------------------
void SetAddr(uint8_t addr) {
  gpio_put(AX0,bitRead(addr,0));
  gpio_put(AX1,bitRead(addr,1));
  gpio_put(AX2,bitRead(addr,2));
  gpio_put(AX3,bitRead(addr,3));
  gpio_put(AY0,bitRead(addr,4));
  gpio_put(AY1,bitRead(addr,5));
  gpio_put(AY2,bitRead(addr,6));
    //      Serial.print("Table: ");
   // Serial.println( addr, BIN );
}

void SetKey(bool data){
   gpio_put(CSMT, 1); //выбор чипа
   gpio_put(STBMT, 1); //строб on
   gpio_put(DATMT, data); //данные
   gpio_put(STBMT, 0); //строб off    
   gpio_put(CSMT, 0);   
}

//---------------------------------------------------
void scancode_s(uint8_t code)
{
   if (code & 0x02) tab_key[0x72]=1;// left shift  
   if (code & 0x20) tab_key[0x72]=1;// righr shift 

   if (code & 0x04) tab_key[0x70]=1;  // left alt     0000 0100
   if (code & 0x40) tab_key[0x70]=1;  // right alt   0100 0000
   
   if (code & 0x01) tab_key[0x71]=1; // right ctrl   
   if (code & 0x10) tab_key[0x71]=1; // left ctrl
   
   if (code & 0x08) tab_key[0x73]=1; // win  l
   if (code & 0x80) tab_key[0x73]=1; // win r
   }
//----------------------------------------------------------
bool led_cl=true;

void key_on(uint8_t code) // клавиша нажата
{
            code = code & 0x7f;//???
            if (code==0x39) /*39 Caps Lock  */
            {
               if (led_cl) kb_set_leds(1); 
                else kb_res_leds(1);
                led_cl = !led_cl; // тригер  Caps lock
            }
            else
            kb_set_leds(0);// 0. Num lock 1.  Caps lock 2.  Scroll

            gpio_put(LEDPIN, 1);//led
            ws2812_set_rgb(0, 1, 0);

             code = code<<1; // умножение на 2
             
             if (table_key_zx[code]==NC) return;// если на код нет нажатия клавиши ZX
             if (table_key_zx[code+1]!=NC) //если есть клавиша модификатор
             {
             SetAddr(table_key_zx[code+1]);// адрес  клавиши модификатора zx CS или SS 
             SetKey(1);//нажатие клавиши
             }
             SetAddr(table_key_zx[code]);// адрес клавиши zx 
             SetKey(1);//нажатие клавиши
              
              debug_print( "Key ON: 0x%02X  0x%02X 0x%02X \r\n",code>>1,table_key_zx[code],table_key_zx[code+1]);
              
                          
             
}

void key_off(uint8_t code)// клавиша отпущена
{
             kb_res_leds(0);// 0. Num lock 1.  Caps lock 2.  Scroll
             gpio_put(LEDPIN, 0);//led
             ws2812_set_rgb(0, 0, 1); 

             code = code<<1; // умножение на 2
             if (table_key_zx[code]==NC) return;// если на код нет нажатия клавиши ZX
             if (table_key_zx[code+1]!=NC) //если есть клавиша модификатор
             {
              SetAddr(table_key_zx[code+1]);// адрес  клавиши модификатора zx CS или SS 
              SetKey(0);//отпускание клавиши
             }
              SetAddr(table_key_zx[code]);// адрес клавиши zx 
              SetKey(0);//отпускание клавиши

              debug_print( "Key OFF: 0x%02X  0x%02X 0x%02X \r\n",code>>1,table_key_zx[code],table_key_zx[code+1]);
}
//----------------------------------------------------------------------------
void keyboard(uint8_t const *report, uint16_t len)
{


debug_print("0x%02X 0x%02X 0x%02X 0x%02X  0x%02X 0x%02X 0x%02X\r\n", report[0] , report[2], report[3], report[4], report[5], report[6], report[7]);

    scancode_s(report[0]); // запись в таблицу 0 кода alt ctrl shift win
    // tab_key[report[1]]=1;// запись в таблицу 1 кода reserved
    tab_key[report[2]] = 1; // запись в таблицу 2 кода
    tab_key[report[3]] = 1; // запись в таблицу 3 кода
    tab_key[report[4]] = 1; // запись в таблицу 4 кода
    tab_key[report[5]] = 1; // запись в таблицу 5 кода
    tab_key[report[6]] = 1; // запись в таблицу 6 кода
    tab_key[report[7]] = 1; // запись в таблицу 7 кода

    for (uint8_t i = 0; i < 127; i++)
    {
        uint8_t d = (tab_key[i] << 1) | (tab_key_old[i]); // 0b000000x0 |0b0000000y
        // if (d==0) // не нажато уже выключать не нужно
        if (d == 2)
            key_on(i); // нажато сейчас
        // if (d==3) // нажато уже включать не нужно
        if (d == 1)
            key_off(i);              // клавиша отпущена  сейчас
        tab_key_old[i] = tab_key[i]; // копирование таблицы
        tab_key[i] = 0;              // стирание таблицы
    }
}

 //----------------------------------------------------------
