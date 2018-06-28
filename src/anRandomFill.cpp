#include "include/anRandomFill.h"

using namespace std;
extern const int MAX_THETA_DIV;

anRandomFill::anRandomFill( npDisplay* pDisplay, mode_t mode, int frames, opt_t opts, scale_t customScaling ) 
: npAnimation( pDisplay, mode, frames, opts, customScaling ) { }

anRandomFill::~anRandomFill() { }

int anRandomFill::Draw() {
    uint16_t stepArray[ FR_LED_COUNT * MAX_THETA_DIV ];
    counter ctrMain( 15, ON );
    counter ctrColor( 100, ON );
    uint16_t rnd1, rnd2, tmp, count;
    // Init array
    for ( int i = 0; i < FR_LED_COUNT * MAX_THETA_DIV; ++i ) {
        stepArray[ i ] = i;
    }
    
    // Pseudo-randomize the array    
    for ( uint16_t i = 0; i <= ( FR_LED_COUNT * MAX_THETA_DIV ) * 2; i++ ) {
        // swap element with random element
        rnd1 = rand() % ( FR_LED_COUNT * MAX_THETA_DIV );
        rnd2 = rand() % ( FR_LED_COUNT * MAX_THETA_DIV );
        tmp = stepArray[ rnd1 ];
        stepArray[ rnd1 ] = stepArray[ rnd2 ];
        stepArray[ rnd2 ] = tmp;
    }
    
    // Main animation loop
    while ( ( framesDrawn < frames ) || modeFlags.test( MODE_REPEAT ) ) {        
        if ( ctrMain.Done() ) {
            ctrMain.Reset();
            int x = stepArray[ count ] % MAX_THETA_DIV;
            int y = stepArray[ count ] / MAX_THETA_DIV;
            Set( x, y, rgbwGetByAngle( angle ) );    // draw new random pixel on top row
         
            if ( ++count > ( FR_LED_COUNT * MAX_THETA_DIV ) ) {
                break;
            }
        }
        
        if ( ctrColor.Done() ) {
            ctrColor.Reset();            
            if ( ( angle += 3.0f ) > 360.0f ) { 
                angle -= 360.0f;                    
            } 
        }
        
    }
    
    return ( MODE_NEXT );
}