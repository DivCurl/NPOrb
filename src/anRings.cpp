#include "include/anRings.h"

using namespace std;
extern const int MAX_THETA_DIV;


anRings::anRings( npDisplay* pDisplay, mode_t mode, int frames, opt_t opts, scale_t customScaling )
: npAnimation( pDisplay, mode, frames, opts, customScaling ) { }

anRings::~anRings() { }

int anRings::Draw() {
    counter ctr( 25, ON );
    // counter fadeCtr( 50, ON );
    counter ctrColorFade( 50, ON );
    // counter ctrTest( 1000, ON );
    static int x, y = 0;

    // Main animation loop
    while ( ( framesDrawn < frames ) || modeFlags.test( MODE_REPEAT ) ) {

        if ( ctr.Done() ) {
            ctr.Reset();
            
            Set( x, y, rgbwGetByAngle( angle ) );

            if ( ++x >= MAX_THETA_DIV ) { 
                x = 0;
                if (++y > GetRowTop() ) {
                    // whole orb full - increment cycle count
                    ++cycle;
                    
                }
            }
            
            if ( ctrColorFade.Done() ) {
                if ( ( angle += 2.0f ) > 360.0f ) { 
                    angle -= 360.0f;                    
                }
            }
            
            if ( cycle > 5 ) {
                break;
            }
            
        }
    }

    return ( MODE_NEXT );
}