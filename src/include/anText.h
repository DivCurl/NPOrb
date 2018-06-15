#ifndef AN_TEXT
#define	AN_TEXT

#include "npAnimation.h"
#include "types.h"

class anText : public npAnimation {
public:
    anText();
    anText( npDisplay* pDisplay, mode_t = MODE_NULL, int frames = 0, opt_t = OPT_NULL, scale_t = SCALE_1_1 );
    ~anText();
    int Draw();
        
private:
    float angle;
    int step;       
};

#endif
