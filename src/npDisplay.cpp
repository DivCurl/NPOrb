#include "./include/npDisplay.h"

using namespace std;

extern uint32_t txSpiDmaBuff[ 1 + FR_LED_COUNT ];  

npDisplay::npDisplay( void ) :
    colorMode( RGB ),                // default to RGB mode
    globalBrightness ( 255 ),
    bytesPerPixel ( 3 ),
    rowTop( 0 ),
    rowBottom( 0 ),
    colLeft( 0 ),
    colRight( 0 ) {
  //  frameBuffer = new uint8_t[ bytesPerPixel * 30 ]();
}

npDisplay::npDisplay( colorMode_t colorMode ) :
    colorMode( colorMode ),
    globalBrightness ( 255 ),
    maxLED( FR_LED_COUNT ),
    rowTop( FR_LED_COUNT - 1 ),
    rowBottom( 0 ),
    colLeft( 0 ),
    colRight( MAX_THETA_DIV - 1 ) {
    switch ( colorMode ) {
        case ( RGB ):
            bytesPerPixel = 3;
            break;

        case ( RGBW ):
            bytesPerPixel = 4;
            break;

        default:
            break;
    }
    
    /* ADDED STRICTLY FOR DOTSTAR STUFF */
    // rowTop = FR_LED_COUNT - 1;
    // rowBottom = 0;
    // colLeft = 0;
    // colRight = MAX_THETA_DIV - 1;    
    // maxLED = FR_LED_COUNT;
    
}

npDisplay::~npDisplay( void ) {
   // delete[] frameBuffer;
}

int npDisplay::GetColorArrayIndex( uint16_t x, uint16_t y ) {
    uint16_t height, width;
    height = maxLED;
    width = MAX_THETA_DIV;

    if ( ( x > width - 1 ) || ( y > height - 1 ) ){
        return ( -1 );  // out of bounds
    }
    
    else {
        return ( ( y + x * height ) /* * bytesPerPixel */ );
    }
}

uint8_t npDisplay::GetMaxLED() {
    return ( maxLED );
}

uint8_t npDisplay::GetNumNeopixels() {
    return ( numNeopixels );
}

uint8_t npDisplay::GetBytesPerPixel() {
    return ( bytesPerPixel );
}

uint8_t npDisplay::GetBrightness() {
    return ( globalBrightness );
}


void npDisplay::SetBrightness( uint16_t brt ) {
    if ( brt < MIN_BRT ) {
        globalBrightness = MIN_BRT;
    }
    else if ( brt > MAX_BRT ) {
        brt = MAX_BRT;
    }
    else {
        globalBrightness = brt;
    }

    /*
    // update framebuffer to new brightness
    for ( int i = 0; i < frameBytes; i++ ) {
        frameBuffer[ i ] = ( frameBuffer[ i ] * ( globalBrightness + 1 ) ) >> 8;
    }
    */
}

/*
uint16_t npDisplay::GetFrameBytes() {
    return ( frameBytes );
}
*/

colorMode_t npDisplay::GetColorMode () {
    return colorMode;
}

void npDisplay::ClrFB() {
    // memset( frameBuffer, 0, sizeof(  frameBuffer ) );
    for ( int n = 0; n < FB_SIZE; n++ ) {
        frameBuffer[ n ] = 0xFF000000;
    }
}

// returns packed RGB or RGBW DWORD
// color array is G-R-B[-W], returned DWORD is R-G-B[-W]
uint32_t npDisplay::GetPackedColorAtCoord( uint16_t x, uint16_t y ) {
     uint16_t index;
     uint32_t color;

    if ( ( index = GetColorArrayIndex( x, y ) ) >= 0 ) {  // returns -1 if coords are out of bounds
        /*
        if ( colorMode == RGB ) {
            color |= frameBuffer[ index ] << 8; // R
            color |= frameBuffer[ index + 1 ] << 16; // G
            color |= frameBuffer[ index + 2 ]; // B
        }
        else {
            color |= frameBuffer[ index ] << 16; // R
            color |= frameBuffer[ index + 1 ] << 24; // G
            color |= frameBuffer[ index + 2 ] << 8; // B
            color |= frameBuffer[ index + 3 ]; // W
        }
        */
        
        color = ( frameBuffer[ index ] & 0xFF << 16 ) | ( frameBuffer[ index ] & 0xFF00 << 8 ) | ( frameBuffer[ index ] & 0xFF0000 );

        return ( color );
    }
    else {
        return ( 0 );
    }
}

rgbw_t npDisplay::GetColorAtCoord( uint16_t x, uint16_t y ) {
    uint16_t index;
    rgbw_t color;
    
    if ( ( index = GetColorArrayIndex( x, y ) ) >= 0 ) {  // returns -1 if coords are out of bounds
        /*
        if ( colorMode == RGB ) {
            color.g = frameBuffer[ index ];
            color.r = frameBuffer[ index + 1 ];
            color.b = frameBuffer[ index + 2 ];
        }
        else {
            color.g = frameBuffer[ index ];
            color.r = frameBuffer[ index + 1 ];
            color.b = frameBuffer[ index + 2 ];
            color.w = frameBuffer[ index + 3 ];
        }
        */
        
        color.r = frameBuffer[ index ] & 0xFF;
        color.g = frameBuffer[ index ] & 0xFF00;
        color.b = frameBuffer[ index ] & 0xFF0000;

        return ( color );
    }
}

void npDisplay::PushFB() {
    memcpy( frameBufferAlt, frameBuffer, sizeof( frameBufferAlt ) );
}

void npDisplay::PopFB() {
    memcpy( frameBuffer, frameBufferAlt, sizeof( frameBuffer ) );
}

uint32_t* npDisplay::GetFBPointer() {
    return frameBuffer;
}

// __attribute__ ( ( optimize( "unroll-loops" ) ) )
// Note: in this implementation (DotStar LEDs), this function is now called directly from T4 interrupt
void npDisplay::Refresh( int mode ) {           
    fbOffset = arc * FR_LED_COUNT;       
    
    // ** Refresh Display **
    // This is too slow; I've experimented with and successfully achieved good control and timing
    // with SPI transfer using DMA; leaving this here for reference.
    // *********************
    
    // Start Frame Packet
    /*
    SpiChnPutC( SPI_CHANNEL1, 0x0000 );
    // LED Packets
    for ( int n = 0; n < FR_LED_COUNT; n++ ) {        
        SpiChnPutC( SPI_CHANNEL1, frameBuffer[ fbOffset + n ] );     // LED Frame  [ Blue ] [ Green ] [ Red ]
    }
    // End Frame Packet    
    // SpiChnPutC( SPI_CHANNEL1, 0xFFFF );    
    */
    
    // Start Frame Packet
    txSpiDmaBuff[ 0 ] = 0;
    // 1 division of overall framebuffer
    memcpy( &txSpiDmaBuff[ 1 ], &frameBuffer[ fbOffset ], FR_LED_COUNT * 4 ); 
    // Kick off DMA-SPI transfer
    DmaChnStartTxfer( DMA_CHANNEL1, DMA_WAIT_NOT, 0 );	    
    
    // Increment the division to be refreshed on next interrupt 
    if ( ++arc >= MAX_THETA_DIV ) {
        arc = 0;
    }
}

