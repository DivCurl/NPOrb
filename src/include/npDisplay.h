#ifndef NPDISPLAY_H
#define	NPDISPLAY_H 

#include <vector>
#include "Neopixel.h"
#include "types.h"
#include "rgbColor.h"

// #define DRAW_HALF_ARC 

// delays for Neopixel refresh
#ifdef __OPTIMIZE__
    // #define delay_t1(); { asm volatile( "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n" ); }
    // #define delay_t2(); { asm volatile( "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n" ); }
    #define delay_t1(); { asm volatile( "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n" ); }
    // #define delay_2(); { asm volatile( "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n" ); }
    #define delay_t2(); { asm volatile( "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n" ); }
#else
    #define delay_t1(); { asm volatile( "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n" ); }    
    #define delay_t2(); { asm volatile( "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n" ); }
#endif

// const int FR_LED_COUNT          = 44;
const int FR_LED_COUNT          = 40;
const int FR_HEADER_DWORD       = 1;    // always 0x0000
const int FR_FOOTER_DWORD       = 1;    // always 0xFFFF
const int FR_DWORD_PER_LED      = 1;    // 1 byte each - [ BRT ] [ Blue ] [ Green ] [ Red ] 
const int FR_DWORD_PER_FRAME    = FR_LED_COUNT * FR_DWORD_PER_LED;    // 44 LEDs on strand
const int MAX_THETA_DIV         = 100;
const int FB_SIZE = MAX_THETA_DIV * FR_DWORD_PER_FRAME;

// const int FB_SIZE = 32 * 4;
// #define FB_SIZE 128

const uint8_t MIN_BRT = 0;
const uint8_t MAX_BRT = 255;

const uint16_t portPin[] {
    0x01,   // 0
    0x02,   // 1
    0x04,   // 2
    0x08,   // 3
    0x10,   // 4
    0x20,   // 5
    0x40,   // 6
    0x80,   // 7
    0x100,   // 8
    0x200,   // 9
    0x400,   // 10
    0x800,   // 11
    0x1000,   // 12
    0x2000,   // 13
    0x4000,   // 14
    0x8000    // 15
};

class npDisplay {
    
friend class npAnimation;

public:
    npDisplay();
    npDisplay( int num );
    npDisplay( colorMode_t colorMode );
    ~npDisplay();
    int AddNeopixel( uint16_t numLEDs, volatile uint32_t* portSET, volatile uint32_t* portCLR, volatile uint32_t* portTRIS, uint16_t pinLED );  // adds LED object...strip, ring, whatever
    int AddNeopixel( Neopixel& np ); 
    int DeleteNeopixel( uint8_t pos );
    int GetColorArrayIndex( uint16_t x, uint16_t y );   
    void Refresh( int mode = FB_BLEND );
    void GetRefreshLUT( void );
    void SetPixel( uint16_t x, uint16_t y, rgbw_t color, uint16_t brt = 255 );
    void SetBrightness( uint16_t brt );
    void ClrFB();
    void PushFB();
    void PopFB();
    uint8_t GetMaxLED();
    uint8_t GetNumNeopixels();
    uint8_t GetBytesPerPixel();
    uint8_t GetBrightness();
    uint16_t GetFrameBytes();
    colorMode_t GetColorMode ();
    rgbw_t GetColorAtCoord( uint16_t x, uint16_t y );
    uint32_t GetPackedColorAtCoord( uint16_t x, uint16_t y );
    uint32_t* GetFBPointer();    

protected:  
    std::vector<Neopixel> neopixels;    
    uint32_t frameBuffer[ FB_SIZE ] = {};
    uint32_t frameBufferAlt[ 1 ] = {};
    // int t1LUT[ 1088 ] = {}; 
    uint8_t maxLED;                         // maximum connected LED count per Neopixel strand
    uint8_t bytesPerPixel;
    uint16_t globalBrightness;
    uint8_t numNeopixels;
    uint16_t frameBytes;   
    uint16_t refreshPulses;
    uint16_t rowBottom;
    uint16_t rowTop;
    uint16_t colLeft;
    uint16_t colRight;
    colorMode_t colorMode; 
    uint32_t count = 0;
    int pins = 0;    
    int arc = 0;
    int fbOffset = 0;
};
    
#endif