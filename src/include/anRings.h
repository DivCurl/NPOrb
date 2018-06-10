#ifndef AN_RINGS
#define	AN_RINGS

#include "npAnimation.h"
#include "types.h"

class anRings : public npAnimation {
public:
    anRings();
    anRings( npDisplay* pDisplay, mode_t = MODE_NULL, int frames = 0, opt_t = OPT_NULL, scale_t = SCALE_1_1 );
    ~anRings();
    int Draw();
        
private:
    float angle;
    int step;       
};

#endif
