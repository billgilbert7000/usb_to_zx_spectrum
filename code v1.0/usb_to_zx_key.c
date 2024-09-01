#include "init.h"

//#include "bsp/board.h"
#include "tusb.h"

#include <stdlib.h>

#include "scancode_zx.h"
#include "logger.h"

#pragma GCC optimize("Ofast")




bool kb_enabled = true;
uint8_t kb_addr = 0;
uint8_t kb_inst = 0;

bool blinking = false;
//bool repeating = false;
//bool repeatmod = false;
//uint32_t repeat_us = 35000;
//uint16_t delay_ms = 250;
alarm_id_t repeater;

uint8_t prev_rpt[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t prev_kb = 0;
uint8_t resend_kb = 0;
uint8_t resend_ms = 0;
uint8_t repeat = 0;
uint8_t leds = 0;


#define MS_TYPE_STANDARD  0x00
#define MS_TYPE_WHEEL_3   0x03
#define MS_TYPE_WHEEL_5   0x04

#define MS_MODE_IDLE      0
#define MS_MODE_STREAMING 1

#define MS_INPUT_CMD      0
#define MS_INPUT_SET_RATE 1

uint8_t ms_type = MS_TYPE_STANDARD;
uint8_t ms_mode = MS_MODE_IDLE;
uint8_t ms_input_mode = MS_INPUT_CMD;
uint8_t ms_rate = 100;
uint32_t ms_magic_seq = 0x00;
///////////////////////////////////////////////////////////////////////////////////////////////
// Language ID: English
#define LANGUAGE_ID 0x0409

typedef struct {
  tusb_desc_device_t desc_device;
  uint16_t manufacturer[32];
  uint16_t product[32];
  uint16_t serial[16];
  bool mounted;
} dev_info_t;
// CFG_TUH_DEVICE_MAX is defined by tusb_config header
dev_info_t dev_info[CFG_TUH_DEVICE_MAX] = { 0 };


//--------------------------------------------------------------------+
// TinyUSB Host callbacks
//--------------------------------------------------------------------+
void print_device_descriptor(tuh_xfer_t* xfer);

void utf16_to_utf8(uint16_t *temp_buf, size_t buf_len);
//-------------------------------------------------------------
void print_lsusb(void) {
  bool no_device = true;
  for ( uint8_t daddr = 1; daddr < CFG_TUH_DEVICE_MAX+1; daddr++ ) {
//  может использовать the_mounted(daddr), но в tinyusb есть ошибка
// вместо этого используйте флаг локального подключения
    dev_info_t* dev = &dev_info[daddr-1];
    if ( dev->mounted ) {
       debug_print("Device %u: ID %04x:%04x %s %s\r\n", daddr,
                    dev->desc_device.idVendor, dev->desc_device.idProduct,
                    (char*) dev->manufacturer, (char*) dev->product);

      no_device = false;
    }
  }

  if (no_device) {
     debug_print("No device connected (except hub)");
  
}
}
//--------------------------------------------------------------------
/*
// Invoked when device is mounted (configured)
void tuh_mount_cb (uint8_t daddr)
{
   if(DEBUG) printf("Device attached, address = %d\r\n", daddr);

  dev_info_t* dev = &dev_info[daddr-1];
  dev->mounted = true;

  // Get Device Descriptor
  tuh_descriptor_get_device(daddr, &dev->desc_device, 18, print_device_descriptor, 0);
}

/// Вызывается, когда устройство размонтировано (отсоединено)
void tuh_umount_cb(uint8_t daddr)
{
   debug_print("Device removed, address = %d\r\n", daddr);
  dev_info_t* dev = &dev_info[daddr-1];
  dev->mounted = false;

  // print device summary
  print_lsusb();
}*/
//-----------------------------------------------------------------
void print_device_descriptor(tuh_xfer_t* xfer)
{
if ( XFER_RESULT_SUCCESS != xfer->result )
  {
     debug_print("Failed to get device descriptor\r\n");
    return;
  }
} 
//--------------------------------------------------------------------+
// String Descriptor Helper
//--------------------------------------------------------------------+

static void _convert_utf16le_to_utf8(const uint16_t *utf16, size_t utf16_len, uint8_t *utf8, size_t utf8_len) {
  // TODO: Check for runover.
  (void)utf8_len;
  // Get the UTF-16 length out of the data itself.

  for (size_t i = 0; i < utf16_len; i++) {
    uint16_t chr = utf16[i];
    if (chr < 0x80) {
      *utf8++ = chr & 0xff;
    } else if (chr < 0x800) {
      *utf8++ = (uint8_t)(0xC0 | (chr >> 6 & 0x1F));
      *utf8++ = (uint8_t)(0x80 | (chr >> 0 & 0x3F));
    } else {
      // TODO: Verify surrogate.
      *utf8++ = (uint8_t)(0xE0 | (chr >> 12 & 0x0F));
      *utf8++ = (uint8_t)(0x80 | (chr >> 6 & 0x3F));
      *utf8++ = (uint8_t)(0x80 | (chr >> 0 & 0x3F));
    }
    // TODO: Handle UTF-16 code points that take two entries.
  }
}

// Count how many bytes a utf-16-le encoded string will take in utf-8.
static int _count_utf8_bytes(const uint16_t *buf, size_t len) {
  size_t total_bytes = 0;
  for (size_t i = 0; i < len; i++) {
    uint16_t chr = buf[i];
    if (chr < 0x80) {
      total_bytes += 1;
    } else if (chr < 0x800) {
      total_bytes += 2;
    } else {
      total_bytes += 3;
    }
    // TODO: Handle UTF-16 code points that take two entries.
  }
  return total_bytes;
}

void utf16_to_utf8(uint16_t *temp_buf, size_t buf_len) {
  size_t utf16_len = ((temp_buf[0] & 0xff) - 2) / sizeof(uint16_t);
  size_t utf8_len = _count_utf8_bytes(temp_buf + 1, utf16_len);

  _convert_utf16le_to_utf8(temp_buf + 1, utf16_len, (uint8_t *) temp_buf, buf_len);
  ((uint8_t*) temp_buf)[utf8_len] = '\0';
} 
///////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
uint8_t type_hid = 0;
//------------------------------------------------------------------------------
// mouse
   uint8_t mouse_x = 0xff;
   uint8_t mouse_y = 0xff ;
   uint8_t mouse_b = 0xff ;//#FADF - поpт  кнопок
   uint8_t joy_k =   0xff;
   
volatile int8_t mouseDirectionX = 0;    // X direction (0 = decrement, 1 = increment)
volatile int8_t mouseEncoderPhaseX = 0; // X Quadrature phase (0-3)

volatile int8_t mouseDirectionY = 0;    // Y direction (0 = decrement, 1 = increment)
volatile int8_t mouseEncoderPhaseY = 0; // Y Quadrature phase (0-3)

volatile int16_t mouseDistanceX = 0; // Distance left for mouse to move
volatile int16_t mouseDistanceY = 0; // Distance left for mouse to move
//------------------------------------------------------------------------------
//void mouse(uint8_t const *report, uint16_t len ) 
void mouse(hid_mouse_report_t const* report, uint16_t len ) 

{
//debug_print("B=%02X X=%02d Y=%02d #1F=%02X        %02X\r\n", mouse_b, mouse_x, mouse_y, joy_k ,type_hid ); 
}
//------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Вызывается при получении отчета от устройства через конечную точку прерывания
// Примечание: если есть идентификатор отчета (составной), то это 1-й байт отчета
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
   switch(tuh_hid_interface_protocol(dev_addr, instance)) {

    case  HID_ITF_PROTOCOL_KEYBOARD: 
          kb_addr = dev_addr;
          kb_inst = instance;
          keyboard (report,len);
          break;

    case HID_ITF_PROTOCOL_MOUSE: //mouse(report, len);break;
        {

      }    
     default:break;// gamepad (report,len);break;
  // continue to request to receive report
   }
 if (!tuh_hid_receive_report(dev_addr, instance)) if(DEBUG) printf("Error: cannot request to receive report\r\n");

}

///////////////////////////////////////////////////////////////////////////////////////////////
uint8_t const ledkey_on[] = { 0b001 , 0b010, 0b100 };
uint8_t const ledkey_off[] = { 0b110 , 0b101, 0b011 };
// 0. Num lock 1.  Caps lock 2.  Scroll

void kb_set_leds(uint8_t d) {
 
 leds = leds | ledkey_on[d];


  tuh_hid_set_report(kb_addr, kb_inst, 0, HID_REPORT_TYPE_OUTPUT, &leds, sizeof(leds));
}

void kb_res_leds(uint8_t d) {
 
 leds = leds & ledkey_off[d];


  tuh_hid_set_report(kb_addr, kb_inst, 0, HID_REPORT_TYPE_OUTPUT, &leds, sizeof(leds));
}


//kb_set_leds(0b100);// 1. 0b001 Num lock 2. 0b010 Caps lock 3. 0b011 Num lock + Caps lock  4. 0b100 Scroll

int64_t blink_callback(alarm_id_t id, void *user_data) {
  if(kb_addr) {
    if(blinking) {
      kb_set_leds(7);
      blinking = false;
      return 500000;
    } else {
      kb_set_leds(0);
      
    }
  }
  return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////
 // Invoked when device is mounted (configured)
/*  void tuh_mount_cb (uint8_t daddr)
{
   if(DEBUG) printf("tuh_mount_cb = %d\r\n", daddr);


} */
  
///////////////////////////////////////////////////////////////////////////////////////////
// Вызывается при подключении устройства с интерфейсом hid
// Дескриптор отчета также доступен для использования. tuh_hid_parse_report_descriptor()
// может использоваться для анализа общего /достаточно простого дескриптора.
// Примечание: если длина дескриптора отчета > CFG_TUH_ENUMERATION_BUFSIZE, он будет пропущен
// следовательно, report_desc = NULL, desc_len = 0
void tuh_hid_mount_cb(uint8_t daddr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len) {
 debug_print("HID device address = %d, instance = %d is mounted\n", daddr, instance);

  dev_info_t* dev = &dev_info[daddr-1];
  dev->mounted = true;

  // Get Device Descriptor
 //tuh_descriptor_get_device(daddr, &dev->desc_device, 18, print_device_descriptor, 0);

  


  switch(tuh_hid_interface_protocol(daddr, instance))
  {
    case HID_ITF_PROTOCOL_KEYBOARD:
     debug_print("HID Interface Protocol = Keyboard\n");
      
      kb_addr = daddr;
      kb_inst = instance;
      
      tuh_hid_receive_report(daddr, instance);
      ws2812_set_rgb(0, 1, 1);
    break;
    
    case HID_ITF_PROTOCOL_MOUSE:
      debug_print("HID Interface Protocol = Mouse\n");
     
      tuh_hid_receive_report(daddr, instance);
    break;
  }




}
// Вызывается, когда устройство с интерфейсом hid не подключено
void tuh_hid_umount_cb(uint8_t daddr, uint8_t instance) {
  debug_print("HID device address = %d, instance = %d is unmounted\r\n", daddr, instance);
  dev_info_t* dev = &dev_info[daddr-1];
  dev->mounted = false;

  // print device summary
  print_lsusb();

  ws2812_set_rgb(1, 0, 0);
}


//========================================================
void main() {

//board_init
  vreg_set_voltage(VOLTAGE);
	sleep_ms(100);
	set_sys_clock_khz(CPU_MHZ*1000, false);
	stdio_init_all();
// пин светодиода
    gpio_init(LEDPIN);
    gpio_set_dir(LEDPIN, GPIO_OUT);

  //Инициализация пинов пико
/* 
  pinMode(AX0, OUTPUT);   //AX0
  pinMode(AX1, OUTPUT);   //AX1
  pinMode(AX2, OUTPUT);   //AX2
  pinMode(AX3, OUTPUT);   //AX3
  pinMode(AY0, OUTPUT);   //AY0
  pinMode(AY1, OUTPUT);   //AY1
  pinMode(AY2, OUTPUT);   //AY2
  pinMode(RSTMT, OUTPUT); //RES
  pinMode(CSMT, OUTPUT);  //CS
  pinMode(DATMT, OUTPUT); //DAT
  pinMode(STBMT, OUTPUT); //STB
 */
/*инициализация пинов пико для управления 8816 по порядку
AX0 первый 11 штук */
for (size_t i = 0; i < 11; i++)
{
  gpio_init(i+AX0);
  gpio_set_dir(i+AX0, GPIO_OUT);
  gpio_put(i+AX0, 1);
}

  //Инициализация MT8816
  SetAddr(0x00);
  gpio_put(RSTMT,0);
  gpio_put(CSMT,0);
  gpio_put(DATMT,0);
  gpio_put(STBMT,0); //инициализация
  gpio_put(CSMT,1); //выбор чипа
  gpio_put(RSTMT,1);
  gpio_put(RSTMT,0);  //сброс
  gpio_put(CSMT,0);
  

 // while ( !Serial ) delay(10);   // wait for native usb
  printf("\nUSBtoZX-0.1 DEBUG=%s\n", DEBUG ? "true" : "false");

ws2812_init();
//ws2812_reset();

ws2812_set_rgb(0, 0, 1);


  tusb_init(); // инициализация USB OTG


  while(true) {

    tuh_task();  // Process updates for the keyboard and mouse.

    
  }
}
//=================================================================
