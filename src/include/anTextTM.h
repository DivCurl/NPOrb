#ifndef AN_TEXT_TM
#define	AN_TEXT_TM

#include "npAnimation.h"
#include "types.h"
#include "ascii.h"
#include "bitmap.h"

class anTextTM : public npAnimation {
public:
    anTextTM();
    anTextTM( npDisplay* pDisplay, mode_t = MODE_NULL, int frames = 0, opt_t = OPT_NULL, scale_t = SCALE_1_1 );
    ~anTextTM();
    int Draw();
        
private:
    float angle;
};

#endif
