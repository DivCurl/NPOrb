#include "include/anRain.h"

using namespace std;
extern const int MAX_THETA_DIV;


anRain::anRain( npDisplay* pDisplay, mode_t mode, int frames, opt_t opts, scale_t customScaling )
: npAnimation( pDisplay, mode, frames, opts, customScaling ) { }

anRain::~anRain() { }

int anRain::Draw() {
    counter ctr( 100, ON );
    // counter fadeCtr( 50, ON );
    counter ctrColorFade( 50, ON );
    // counter ctrTest( 1000, ON );

    // Main animation loop
    while ( ( framesDrawn < frames ) || modeFlags.test( MODE_REPEAT ) ) {

        if ( ctr.Done() ) {
            ctr.Reset();
            ShiftDown();    // shift down to simulate falling drops

            int i, r_i, rnd;

            rnd = rand() % 4;   // max random number of 'droplets' to start with on top row every cycle

            for ( i = 0; i <= rnd; i++ ) {
                r_i = rand() % MAX_THETA_DIV;  // determine where to put the pixels on the x-axis
                Set( r_i, GetRowTop(), rgbwGetByAngle( angle ) );    // draw new random pixel on top row
            }

            if ( ctrColorFade.Done() ) {
                if ( ( angle += 2.0f ) > 360.0f ) { 
                    angle -= 360.0f;                    
                }
            }
            
            ++framesDrawn;
        }
    }

    return ( MODE_NEXT );
}