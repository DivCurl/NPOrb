#include "include/anRings.h"

using namespace std;
extern const int MAX_THETA_DIV;


anRings::anRings( npDisplay* pDisplay, mode_t mode, int frames, opt_t opts, scale_t customScaling ) 
: npAnimation( pDisplay, mode, frames, opts, customScaling ) { }

anRings::~anRings() { }

int anRings::Draw() {
    // static float angle1, angle2, angle3, angle4;
    counter ctr( 50, ON );
    counter fadeCtr( 500, ON );
    counter colorCtr( 150, ON );
    int x, y = 0;
    
    // Main animation loop
    while ( ( framesDrawn < frames ) || modeFlags.test( MODE_REPEAT ) ) {        
             
        if ( ctr.Done() ) {                
            ctr.Reset();                 
            Set( x, y, rgbwGetByAngle( angle, 0 ) );

            if ( ++x >= MAX_THETA_DIV ) { 
                x = 0;
                if (++y > GetRowTop() ) {
                    // whole orb full
                    // break while loop and terminate animation
                    break; 
                }
            }

            if ( colorCtr.Done() ) {    
                colorCtr.Reset();                
                if ( ( angle += 2.0f ) > 360.0f ) { 
                    angle -= 360.0f;                    
                }
            }

            framesDrawn++;
        }            
    }   
    
    
    return ( MODE_NEXT );
}