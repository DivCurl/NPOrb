#ifndef IO_H
#define	IO_H

#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING

#include <cstdint>
#include <p32xxxx.h>
#include <plib.h> 
#include "types.h"

/* RANT WARNING
 * I had to decode ALL of these by hand via a debug terminal session with the 4D-Systems touchscreen display. 
 * What's hilarious to me is that the values of these packets don't even seem to be generated in an simple, logical, or 
 * otherwise obvious way...you'd think that a pushbutton for example would have a particular signature bytecode or something, 
 * ... but oh no that would
 * be WAY too fucking simple. Instead, force your developer to sit and read the value of EACH INDVIDUAL screen control.
 * Also: 4D Workshop is a TERRIBLE, childish looking, unpolished piece of shit software that hasn't changed much in the year or two
 * since I first played with it. I mean, wow...it really does suck. 
 * My suggestion therefore is absolutely not to use a 4D Systems display and go with something much nicer and developer friendly.  
 */

// Defines for LCD UART read/write messages, as inferred from 4D Workshop debugging
#define LCD_ACK             0x06
#define LCD_NAK             0x15
#define LCD_DISPLAY_ON      0x071E0600011E
#define LCD_DISPLAY_OFF     0x071E0600001F 
#define LCD_MODE_PAUSE_ON   0x071E00000118 
#define LCD_MODE_PAUSE_OFF  0x071E00000019 
#define LCD_MODE_REPEAT_ON  0x071E0200011A 
#define LCD_MODE_REPEAT_OFF 0x071E0200001B 
#define LCD_MODE_STBY_ON    0x071E01000119 
#define LCD_MODE_STBY_OFF   0x071E01000018
#define LCD_MODE_PREVIOUS   0x071E0500001C
#define LCD_MODE_NEXT       0x071E09000010 
#define LCD_SET_PAUSE_ON    0x011E0000011E
#define LCD_SET_PAUSE_OFF   0x011E0000001F
#define LCD_SET_REPEAT_ON   0x011E0200011C
#define LCD_SET_REPEAT_OFF  0x011E0200001D
#define LCD_SET_STBY_ON     0x011E0100011F
#define LCD_SET_STBY_OFF    0x011E0100001E
#define LCD_SET_DISPLAY_ON  0x011E06000118
#define LCD_SET_DISPLAY_OFF 0x011E06000019
// Brightness slider
#define LCD_BRIGHT_0        0x070400000003                 
#define LCD_BRIGHT_1        0x070400000102 
#define LCD_BRIGHT_2        0x070400000201             
#define LCD_BRIGHT_3        0x070400000300 
#define LCD_BRIGHT_4        0x070400000407 
#define LCD_BRIGHT_5        0x070400000506 
#define LCD_BRIGHT_6        0x070400000605
#define LCD_BRIGHT_7        0x070400000704
#define LCD_BRIGHT_8        0x07040000080B
#define LCD_BRIGHT_9        0x07040000090A
#define LCD_SET_BRIGHT_0    0x010400000005
#define LCD_SET_BRIGHT_1    0x010400000104
#define LCD_SET_BRIGHT_2    0x010400000207
#define LCD_SET_BRIGHT_3    0x010400000306
#define LCD_SET_BRIGHT_4    0x010400000401
#define LCD_SET_BRIGHT_5    0x010400000500
#define LCD_SET_BRIGHT_6    0x010400000603
#define LCD_SET_BRIGHT_7    0x010400000702
#define LCD_SET_BRIGHT_8    0x01040000080D
#define LCD_SET_BRIGHT_9    0x01040000090C
// Fade delay slider
#define LCD_FADE_0          0x070401000002 
#define LCD_FADE_1          0x070401000103
#define LCD_FADE_2          0x070401000200
#define LCD_FADE_3          0x070401000301
#define LCD_FADE_4          0x070401000406
#define LCD_FADE_5          0x070401000507
#define LCD_FADE_6          0x070401000604
#define LCD_FADE_7          0x070401000705
#define LCD_FADE_8          0x07040100080A 
#define LCD_FADE_9          0x07040100090B 
#define LCD_SET_FADE_0      0x010401000004
#define LCD_SET_FADE_1      0x010401000105
#define LCD_SET_FADE_2      0x010401000206
#define LCD_SET_FADE_3      0x010401000307
#define LCD_SET_FADE_4      0x010401000400
#define LCD_SET_FADE_5      0x010401000501
#define LCD_SET_FADE_6      0x010401000602
#define LCD_SET_FADE_7      0x010401000703
#define LCD_SET_FADE_8      0x01040100080C
#define LCD_SET_FADE_9      0x01040100090D
// Strobe delay slider
#define LCD_STROBE_0        0x070402000001 
#define LCD_STROBE_1        0x070402000100 
#define LCD_STROBE_2        0x070402000203
#define LCD_STROBE_3        0x070402000302
#define LCD_STROBE_4        0x070402000405
#define LCD_STROBE_5        0x070402000504
#define LCD_STROBE_6        0x070402000607 
#define LCD_STROBE_7        0x070402000706 
#define LCD_STROBE_8        0x070402000809
#define LCD_STROBE_9        0x070402000908 
#define LCD_SET_STROBE_0    0x010402000007
#define LCD_SET_STROBE_1    0x010402000106
#define LCD_SET_STROBE_2    0x010402000205
#define LCD_SET_STROBE_3    0x010402000304  
#define LCD_SET_STROBE_4    0x010402000403
#define LCD_SET_STROBE_5    0x010402000502
#define LCD_SET_STROBE_6    0x010402000601
#define LCD_SET_STROBE_7    0x010402000700
#define LCD_SET_STROBE_8    0x01040200080F
#define LCD_SET_STROBE_9    0x01040200090E
// SPARES
#define LCD_GAIN_0          0x070403000000 
#define LCD_GAIN_1          0x070403000101 
#define LCD_GAIN_2          0x070403000202 
#define LCD_GAIN_3          0x070403000303 
#define LCD_GAIN_4          0x070403000404 
#define LCD_GAIN_5          0x070403000505 
#define LCD_GAIN_6          0x070403000606 
#define LCD_GAIN_7          0x070403000707 
#define LCD_GAIN_8          0x070403000808 
#define LCD_GAIN_9          0x070403000909 
// SLIDER 3 ...  
#define LCD_SET_GAIN_0      0x010403000006
#define LCD_SET_GAIN_1      0x010403000107
#define LCD_SET_GAIN_2      0x010403000204
#define LCD_SET_GAIN_3      0x010403000305  
#define LCD_SET_GAIN_4      0x010403000402
#define LCD_SET_GAIN_5      0x010403000503
#define LCD_SET_GAIN_6      0x010403000600
#define LCD_SET_GAIN_7      0x010403000701
#define LCD_SET_GAIN_8      0x01040300080E
#define LCD_SET_GAIN_9      0x01040300090F

// Adafruit Bluetooth Friend Commands
#define XT_MODE_BTN_RIGHT   0x2142383133
#define XT_MODE_BTN_LEFT    0x2142373134
#define XT_MODE_BTN_UP      0x2142353136
#define XT_MODE_BTN_DOWN    0x2142363135    
#define XT_MODE_BTN_1       0x214231313A 
#define XT_MODE_BTN_2       0x2142323139
#define XT_MODE_BTN_3       0x2142333138
#define XT_MODE_BTN_4       0x2142343137

extern "C" {
    void UART_MsgHandler( uint64_t packet );
    void UART_SendMessage( const uint64_t buffer, int len );
    void UART_ReadByte();
}

#endif

