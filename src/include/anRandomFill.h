#ifndef AN_RANDOM_FILL
#define	AN_RANDOM_FILL

#include "npAnimation.h"
#include "types.h"

class anRandomFill : public npAnimation {
public:
    anRandomFill();
    anRandomFill( npDisplay* pDisplay, mode_t = MODE_NULL, int frames = 0, opt_t = OPT_NULL, scale_t = SCALE_1_1 );
    ~anRandomFill();
    int Draw();
        
private:
    float angle;
    int step;       
};

#endif
