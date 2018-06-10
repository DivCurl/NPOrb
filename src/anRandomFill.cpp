#include "include/anRandomFill.h"

using namespace std;
extern const int MAX_THETA_DIV;


anRandomFill::anRandomFill( npDisplay* pDisplay, mode_t mode, int frames, opt_t opts, scale_t customScaling ) 
: npAnimation( pDisplay, mode, frames, opts, customScaling ) { }

anRandomFill::~anRandomFill() { }

int anRandomFill::Draw() {
    counter ctr( 300, ON );
    counter fadeCtr( 150, ON );
    // Main animation loop
    while ( ( framesDrawn < frames ) || modeFlags.test( MODE_REPEAT ) ) {        
        
    }
    
    return ( MODE_NEXT );
}