#include "include/anText.h"

using namespace std;
extern const int MAX_THETA_DIV;

anText::anText( npDisplay* pDisplay, mode_t mode, int frames, opt_t opts, scale_t customScaling ) 
: npAnimation( pDisplay, mode, frames, opts, customScaling ) { }

anText::~anText() { }

int anText::Draw() {
   
    while ( ( framesDrawn < frames ) || modeFlags.test( MODE_REPEAT ) ) {             
                    
    return ( MODE_NEXT );
}