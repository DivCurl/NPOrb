#ifndef AN_TOUGH_MUDDER
#define	AN_TOUGH_MUDDER

#include "npAnimation.h"
#include "types.h"
#include "ascii.h"
#include "bitmap.h"

class anToughMudder : public npAnimation {
public:
    anToughMudder();
    anToughMudder( npDisplay* pDisplay, mode_t = MODE_NULL, int frames = 0, opt_t = OPT_NULL, scale_t = SCALE_1_1 );
    ~anToughMudder();
    int Draw();
        
private:
    float angle;
};

#endif
