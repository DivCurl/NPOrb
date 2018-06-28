#include "./include/npAnimation.h"  

using namespace std;

npAnimation::npAnimation() :
    firstScan( 1 ),
    pDisplay( NULL ),
    frames( 0 ) {
        optFlags.set ( OPT_NULL );
        modeFlags.set( MODE_NULL ); 
} 

npAnimation::npAnimation( npDisplay* pDisplay, mode_t mode, int frames, opt_t opts, scale_t customScale ) : 
    pDisplay ( pDisplay ),
    frames( frames ),
    customScale ( customScale ) {
        optFlags.set ( opts );
        modeFlags.set( mode ); 
        framesDrawn = 0;    
        skip = 0;
        ret = MODE_NULL;   
        SetScaling();
        Clr();
        ctrDebounce.SetDelay( 100 );
}

npAnimation::~npAnimation() { }

void npAnimation::SetScaling() {
    if ( optFlags.test ( OPT_CUSTOM_SCALE ) ) {
        switch ( customScale ) {
            case SCALE_1_1:
                xyScale = { 1, 1 };
                break;
                
            case SCALE_1_2:
                xyScale = { 1, 2 };
                break;
                
            case SCALE_1_3:
                xyScale = { 1, 3 };
                break;
                
            case SCALE_1_4:
                xyScale = { 1, 4 };
                break;
                
            case SCALE_1_5:
                xyScale = { 1, 5 };
                break;
                
            default: 
                xyScale = { 1, 1 };
                break;
        }                   
    }
}

void npAnimation::SetMode( mode_t mode ) {
    modeFlags.set( mode );
}

void npAnimation::ClrMode( mode_t mode ) {
    modeFlags.reset( mode );
}

bool npAnimation::CheckAnimSwitch( void ) {
    // Ensure switch has cleanly turned off before we start timer
    // on new animation
    if ( PORTBbits.RB3 ) {
        modeOneShot = 1;
    }
    
    if ( modeOneShot ) {
        if ( !PORTBbits.RB3 ) {    
            ctrDebounce.Start();
        } else {
            ctrDebounce.Stop();
        }

        if ( ctrDebounce.Done() ) {
            return ( 1 );
        }
}
                
    return ( 0 );
    
}

void npAnimation::StartDelayCounter( uint16_t delay ) {
    ctrDelay.Start( delay );
}

bitset<16> npAnimation::GetModeFlags( void ) {
    return ( modeFlags );
}

int npAnimation::GetID( void ) {
    return ( id );
}

uint32_t npAnimation::GetFrames( void ) {
    return ( frames );
}

uint16_t npAnimation::GetRowBottom() {
    return ( pDisplay->rowBottom );
}

uint16_t npAnimation::GetRowTop() {
    return ( pDisplay->rowTop );
}

uint16_t npAnimation::GetColLeft() {
    return ( pDisplay->colLeft );
}

uint16_t npAnimation::GetColRight() {
    return ( pDisplay->colRight );
}

uint16_t npAnimation::GetRowCount() {
    return ( GetRowBottom() + GetRowTop() + 1 );
}

uint16_t npAnimation::GetColCount() {
    return ( GetColLeft() + GetColRight() + 1 );
}

float npAnimation::GetRowMiddle() {
    return ( ( (float)GetRowCount() + 1.0f ) / 2 - 1 );
}

float npAnimation::GetColMiddle() {
    return ( ( (float)GetColCount() + 1.0f ) / 2 - 1 );
}

coord2d_t npAnimation::GetDisplayBottomLeftCoord() {   
    return ( coord2d_t { GetColLeft(), GetRowBottom() } );
}


coord2d_t npAnimation::GetDisplayTopRightCoord() {
    return ( coord2d_t { GetColRight(), GetRowTop() } );
}

float npAnimation::GetDisplayCenter() {
    return ( ( GetColRight() - GetColLeft() ) / 2.0f );
}

float npAnimation::Remap( float in, float inMin, float inMax, float outMin, float outMax ) {
    in = ( in < inMin ) ? inMin : in;
    in = ( in > inMax ) ? inMax : in;
    return ( ( in - inMin ) / ( inMax - inMin ) * ( outMax - outMin ) + outMin );
}

void npAnimation::Set( uint16_t x, uint16_t y, rgbw_t color, uint16_t brt ) {
    // calculate scaled brightness relative to global brightness
    // uint16_t scaledBrt = (float)brt / 255 * pDisplay->globalBrightness;
    uint16_t index;
    
    if ( pDisplay != NULL ) {
        if ( ( index = pDisplay->GetColorArrayIndex( x * xyScale.x, y * xyScale.y ) ) >= 0 ) {  // returns -1 if coords are out of bounds
            pDisplay->frameBuffer[ index ] = ( 0xFF << 24 ) | ( color.b << 16 ) | ( color.g << 8 ) | color.r ;      
        }
    }
}

// Fill entire display with specified color
void npAnimation::Set( rgbw_t color, uint16_t brt ) {
       
    for ( int index = 0; index < FB_SIZE; index++ ) {
        // pDisplay->frameBuffer[ index ] = ( 0xFF << 24 ) | ( color.b << 16 ) | ( color.g << 8 ) | color.r ;
        pDisplay->frameBuffer[ index ] = ( 0xFF << 24 ) | ( color.b << 16 ) | ( color.g << 8 ) | color.r ;
    }
    
}

void npAnimation::Set( const pixel& px ) {
    Set( px.coord.x, px.coord.y, px.color, px.brt );
}

// Clear pixel at specified coordinate
void npAnimation::Clr( uint16_t x, uint16_t y ) {
    uint16_t index;
    
    if ( pDisplay != NULL ) {
        if ( ( index = pDisplay->GetColorArrayIndex( x, y ) ) >= 0 ) {  // returns -1 if coords are out of bounds
            pDisplay->frameBuffer[ index ] = 0xFF000000;
        }
    }
}

// Clear the entire framebuffer if no coords specified
void npAnimation::Clr( void ) {
    pDisplay->ClrFB();
}

// Fills entire row with specified color
/*
void npAnimation::SetRow( uint16_t row, rgbw_t color, uint16_t brt ) {
    for ( int i = 0; i <  pDisplay->numNeopixels; i++ ) {
        Set( i, row, color, brt );
    }
}
*/ 

void npAnimation::SetRow( uint16_t row, uint16_t xMin, uint16_t xMax, rgbw_t color, uint16_t brt ) {
    if ( xMin < xMax ) {
        for ( int i = xMin; i < xMax; i++ ) {
            Set( i, row, color, brt );
        } 
    } 
    else {
        for ( int i = xMax; i > xMin; i-- ) {
            Set( i, row, color, brt );
        }
    }       
}

/*
void npAnimation::SetRowFade( uint16_t row, float angleStart, float angleStop ) {
    float angleDelta = fabs( angleStop - angleStart ) / pDisplay->numNeopixels;
    
    for ( int i = 0; i <  pDisplay->numNeopixels; i++ ) {
        Set( i, row, rgbwGetByAngle( angleStart + ( i * angleDelta ), 0 ) );
    }
}
 */

// Fills entire column with specified color
void npAnimation::SetCol( uint16_t col, rgbw_t color, uint16_t brt ) {
    for ( int j = 0; j <= GetRowTop(); j++ ) {
        Set( col, j, color, brt );
    }
}

// Fill column with start/end points specified
void npAnimation::SetCol( uint16_t col, uint16_t yMin, uint16_t yMax, rgbw_t color, uint16_t brt ) {    
    if ( yMin < yMax ) {
        for ( int i = yMin; i <= yMax; i++ ) {
            Set( col, i, color, brt );
        }
    } 
    else {    // if yMin > yMax, we are drawing the column top-to-bottom so reverse the direction of the loop
        for ( int i = yMax; i <= yMin; i++ ) {
            Set( col, i, color, brt );
        }
    }
}

void npAnimation::SetColFade( uint16_t col, float angleStart, float angleStop ) {
    float angleDelta = abs( angleStop - angleStart ) / pDisplay->GetMaxLED();
    
    for ( int i = 0; i <  pDisplay->GetMaxLED(); i++ ) {
        Set( col, i, rgbwGetByAngle( angleStart + ( i * angleDelta ), 0 ) );
    }
}

void npAnimation::ClrRow( uint16_t row ) {
    for ( int i = 0; i <= pDisplay->colRight; i++ ) {
        Clr( i, row );
    }
}

void npAnimation::ClrCol( uint16_t col ) {
    for ( int i = 0; i <  pDisplay->maxLED; i++ ) {
        Clr( col, i );
    }
}

// copy pixel from source to destination
void npAnimation::Cpy( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 ) {
    rgbw_t color = pDisplay->GetColorAtCoord( x1, y1 );
    Set( x2, y2, color );
}

// Mov pixel from source to destination
void npAnimation::Mov( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 ) {
    // rgbw_t color = pDisplay->GetColorAtCoord( x1, y1 );
    // Set( x2, y2, color );
    // Clr( x1, y1 );
    if ( pDisplay != NULL ) {
        uint16_t index1 = pDisplay->GetColorArrayIndex( x1, y1 );
        uint16_t index2 = pDisplay->GetColorArrayIndex( x2, y2 );
        
        if ( ( index1 && index2 ) ) {  // valid coordinates in buffer
            pDisplay->frameBuffer[ index2 ] = pDisplay->frameBuffer[ index1 ];
            Clr( x1, y1 );
        }
    }    
}

// Swap pixels
void npAnimation::Swap( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 ) {
    rgbw_t color1 = pDisplay->GetColorAtCoord( x1, y1 );
    rgbw_t color2 = pDisplay->GetColorAtCoord( x2, y2 );
    Set( x2, y2, color1 );
    Set( x1, y1, color2 );
}

/*
void npAnimation::CpyRow( uint16_t src, uint16_t dest ) {
    for ( int i = 0; i <  pDisplay->numNeopixels; i++ ) {
        rgbw_t clr = pDisplay->GetColorAtCoord( i, src );
        Set( i, dest, clr );
    }
}
*/ 

void npAnimation::ShiftLeft() {
    for ( int i = pDisplay->colLeft; i < pDisplay->colRight; i++ ) {
        for ( int j = pDisplay->rowBottom; j <= pDisplay->rowTop; j++ ) {
            int idx1 = pDisplay->GetColorArrayIndex( i, j );    // LED on this column
            int idx2 = pDisplay->GetColorArrayIndex( i + 1, j );  // LED on the next column
            
            if ( idx1 >= 0 && idx2 >= 0 ) {
                pDisplay->frameBuffer[ idx1 ] = pDisplay->frameBuffer[ idx2 ]; 
            }
        }        
    }
    
    ClrCol( pDisplay->colRight );
}

void npAnimation::ShiftLeft( uint16_t colLeft, uint16_t colRight ) {
    for ( int i = colLeft; i < colRight; i++ ) {
        for ( int j = pDisplay->rowBottom; j <= pDisplay->rowTop; j++ ) {
            int idx1 = pDisplay->GetColorArrayIndex( i, j );    // LED on this column
            int idx2 = pDisplay->GetColorArrayIndex( i + 1, j );  // LED on the next column
            
            if ( idx1 >= 0 && idx2 >= 0 ) {
                pDisplay->frameBuffer[ idx1 ] = pDisplay->frameBuffer[ idx2 ]; 
            }
        }        
    }    
    
    ClrCol( colRight );
}

void npAnimation::ShiftRight() {
    for ( int i = pDisplay->colRight; i > pDisplay->colLeft; i-- ) {
        for ( int j = pDisplay->rowBottom; j <= pDisplay->rowTop; j++ ) {
            int idx1 = pDisplay->GetColorArrayIndex( i, j );    // LED on this column
            int idx2 = pDisplay->GetColorArrayIndex( i - 1, j );  // LED on the previous column
            
            if ( idx1 >= 0 && idx2 >= 0 ) {
                pDisplay->frameBuffer[ idx1 ] = pDisplay->frameBuffer[ idx2 ]; 
            }
        }        
    }    
    
    ClrCol( pDisplay->colLeft );
}

void npAnimation::ShiftRight( uint16_t colLeft, uint16_t colRight ) {
    for ( int i = colRight; i > colLeft; i-- ) {
        for ( int j = pDisplay->rowBottom; j <= pDisplay->rowTop; j++ ) {
            int idx1 = pDisplay->GetColorArrayIndex( i, j );    // LED on this column
            int idx2 = pDisplay->GetColorArrayIndex( i - 1, j );  // LED on the previous column
            
            if ( idx1 >= 0 && idx2 >= 0 ) {
                pDisplay->frameBuffer[ idx1 ] = pDisplay->frameBuffer[ idx2 ]; 
            }
        }        
    }    
    
    ClrCol( colLeft );
}

void npAnimation::ShiftUp() {
    for ( int i = pDisplay->colLeft; i <= pDisplay->colRight; i++ ) {
        for ( int j = pDisplay->rowTop; j > pDisplay->rowBottom; j-- ) {
            int idx1 = pDisplay->GetColorArrayIndex( i, j );    // LED on this row
            int idx2 = pDisplay->GetColorArrayIndex( i, j-1 );  // LED on row below
            
            if ( idx1 >= 0 && idx2 >= 0 ) {
                pDisplay->frameBuffer[ idx1 ] = pDisplay->frameBuffer[ idx2 ]; 
            }
        }        
    }    
    
    ClrRow( pDisplay->rowBottom );
}

void npAnimation::ShiftColUp( uint16_t col ) {    
    for ( int j = pDisplay->rowTop; j > pDisplay->rowBottom; j-- ) {
        int idx1 = pDisplay->GetColorArrayIndex( col, j );    // LED on this row
        int idx2 = pDisplay->GetColorArrayIndex( col, j-1 );  // LED on row below
        
        if ( idx1 >= 0 && idx2 >= 0 ) {
            pDisplay->frameBuffer[ idx1 ] = pDisplay->frameBuffer[ idx2 ]; 
        }
    }        
    
    Clr( col, 0 );
}

void npAnimation::ShiftUp( uint16_t rowBottom, uint16_t rowTop ) {
    for ( int i = pDisplay->colLeft; i <= pDisplay->colRight; i++ ) {
        for ( int j = rowTop; j > rowBottom; j-- ) {
            int idx1 = pDisplay->GetColorArrayIndex( i, j );    // LED on this row
            int idx2 = pDisplay->GetColorArrayIndex( i, j - 1 );  // LED on row below
            
            if ( idx1 >= 0 && idx2 >= 0 ) {
                pDisplay->frameBuffer[ idx1 ] = pDisplay->frameBuffer[ idx2 ]; 
            }
        }        
    }    
    
    ClrRow( rowBottom );
}

void npAnimation::ShiftDown() {    
    for ( int i = pDisplay->colLeft; i <= pDisplay->colRight; i++ ) {
        for ( int j = pDisplay->rowBottom; j < pDisplay->rowTop; j++ ) {
            uint32_t idx1 = pDisplay->GetColorArrayIndex( i, j );    // LED on this row
            uint32_t idx2 = pDisplay->GetColorArrayIndex( i, j + 1 );  // LED on row above
            
            if ( idx1 >= 0 && idx2 >= 0 ) {              
                pDisplay->frameBuffer[ idx1 ] = pDisplay->frameBuffer[ idx2 ]; 
            }
        }        
    }   
    
    ClrRow( pDisplay->rowTop );
}

void npAnimation::ShiftDown( uint16_t rowBottom, uint16_t rowTop ) {    
    for ( int i = pDisplay->colLeft; i <= pDisplay->colRight; i++ ) {
        for ( int j = rowBottom; j < rowTop; j++ ) {
            int idx1 = pDisplay->GetColorArrayIndex( i, j );    // LED on this row
            int idx2 = pDisplay->GetColorArrayIndex( i, j + 1 );  // LED on row above
            
            if ( idx1 >= 0 && idx2 >= 0 ) {                
                pDisplay->frameBuffer[ idx1 ] = pDisplay->frameBuffer[ idx2 ]; 
            }
        }        
    }    
    
    ClrRow( rowTop );
}

void npAnimation::FadeOut( int fadeMode, int numSteps, uint16_t minBrt ) {
    int brtStep, tmp;       // tmp needs to be int so we can detect underflow in if-test when using step method
    
    if ( fadeMode == 1 ) {  // NOTE: MODE 1 IS NOT CURRENTLY WORKING
        brtStep = 255 / numSteps;
    }
        
    for ( int i = 0; i < FB_SIZE; i++ ) {
        if ( fadeMode == 1 ) {  // use step method
            tmp = pDisplay->frameBuffer[ i ];
            
            if ( ( tmp -= brtStep ) <= 0 ) {
                pDisplay->frameBuffer[ i ] = 0;
            } 
            else {
                pDisplay->frameBuffer[ i ] -= tmp;
            }                
        } 
        else {    // use shift method
            // if ( pDisplay->frameBuffer[ i ] ) {         // if current color is non-zero
            //    if ( ( pDisplay->frameBuffer[ i ] >> 1 ) >= minBrt ) {  // shift as long as we won't undershoot minimum brightness 
                    pDisplay->frameBuffer[ i ] &= 0xFF000000 | ( pDisplay->frameBuffer[ i ] & 0xFFFFFF ) >> 1;
            //    }                
            // }   
        }
    }    
}

void npAnimation::Blit( const vector<pixel>& px ) {   
    if ( !px.empty() ) {
        vector<pixel>::const_iterator it;

        for ( it = px.begin(); it != px.end(); it++ ) { 
            npAnimation::Set( it->coord.x, it->coord.y, it->color, it->brt );

        }
    }
}

int npAnimation::PollGlobalModes() {    
    return 0;
}

void npAnimation::RefreshDisplay( fbBlendMode_t blendMode ) {
    pDisplay->Refresh( );
    // if blend mode is set to clear, purge contents of the FB before
    // drawing next frame otherwise conents of the new frame with be blended
    // with the remnant framebuffer
    if ( blendMode == FB_CLEAR ) {
        Clr();
    }
}

