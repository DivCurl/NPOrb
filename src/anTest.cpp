#include "include/anTest.h"
#include "include/matrix.h"
#include "include/types.h"

using namespace std;

anTest::anTest( npDisplay* pDisplay, mode_t mode, int frames, opt_t opts, scale_t customScaling ) 
: npAnimation( pDisplay, mode, frames, opts, customScaling ) { }

anTest::~anTest() { }

// STRICTLY FOR TESTING //
int anTest::Draw() {
    // Main animation loop   
    
    counter ctrTest( 1000, ON );    
   
    while ( ( framesDrawn < frames ) || modeFlags.test( MODE_REPEAT ) ) {     
        /*
        // For testing T4 interrupt timings
        if ( ctrTest.Done() ) {
            ctrTest.Reset();
            PORTBbits.RB7 ^= 1;
        }
        */         

    } // end main loop         
    
    return ( MODE_NEXT );
}