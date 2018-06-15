#include "include/anComets.h"

using namespace std;
extern const int MAX_THETA_DIV;

anComets::anComets( npDisplay* pDisplay, mode_t mode, int frames, opt_t opts, scale_t customScaling ) 
: npAnimation( pDisplay, mode, frames, opts, customScaling ) { }

anComets::~anComets() { }

int anComets::Draw() {
    const int NUM_COMETS = FR_LED_COUNT / 2;
    
    // Main animation loop  
    counter fadeCtr( 50, ON );
    counter ctrColorFade( 50, ON );
    counter ctr( 75, ON );
    coord2d_t comets[ NUM_COMETS ];
    rgbw_t colors[ NUM_COMETS ];
    
    // Init
    for ( int i = 0; i < NUM_COMETS; ++i ) {
        comets[ i ].x = rand() % MAX_THETA_DIV;
        comets[ i ].y = i * 2;
        colors[ i ] = rgbwGetByAngle( rand() % 360 );
    }
    
    while ( ( framesDrawn < frames ) || modeFlags.test( MODE_REPEAT ) ) {             
            
        if ( fadeCtr.Done() ) {
            fadeCtr.Reset();
            FadeOut();
        }

        if ( ctr.Done() ) {
            ctr.Reset();

            for ( int j = 0; j < NUM_COMETS; j++ ) {
                // Odd numbers move backwards, even move forwards
                if ( j % 2 ) {
                    if ( ++comets[ j ].x >= MAX_THETA_DIV ) {
                        comets[ j ].x = 0;
                    }
                }
                else {
                    if ( --comets[ j ].x <= 0 ) {
                        comets[ j ].x = MAX_THETA_DIV - 1;
                    }
                }

                Set( comets[ j ].x, comets[ j ].y, rgbwGetByAngle( angle )/* colors[ j ] */ );
            }
            
            if ( ctrColorFade.Done() ) {
                if ( ( angle += 4.0f ) > 360.0f ) { 
                    angle -= 360.0f;                    
                }
            }

            ++framesDrawn;
        }   
    }      
    
    return ( MODE_NEXT );
}