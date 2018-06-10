#ifndef AN_COMETS
#define	AN_COMETS

#include "npAnimation.h"
#include "types.h"

class anComets : public npAnimation {
public:
    anComets();
    anComets( npDisplay* pDisplay, mode_t = MODE_NULL, int frames = 0, opt_t = OPT_NULL, scale_t = SCALE_1_1 );
    ~anComets();
    int Draw();
        
private:
    float angle;
    int step;       
};

#endif
