#ifndef MACROS_H
#define MACROS_H

#include <GL/gl.h>
// https://www.chiark.greenend.org.uk/~sgtatham/mp/

/*
 * This Header contains some weird pre-processor code that allows for
 * the "implementation" of new statements.
 * 
 * Although it seems like if(1)'s and while(1)'s with break's are a waste of
 * resources, they should definitely get optimized out with -O2 flag.
 */

#define _TOKPASTEINNER(x, y) x ## y
#define _TOKPASTE(x, y) _TOKPASTEINNER(x, y)
#define _LABEL(id) _TOKPASTE(_LABEL_ ## id ## _, __LINE__)

#define _BEFORE_AFTER(labid, before, after)                             \
    if (1) {                                                            \
        before;                                                         \
        goto _LABEL(labid);                                             \
    }                                                                   \
    else                                                                \
        while (1)                                                       \
            if (1) {                                                    \
                after;                                                  \
                break;                                                  \
            } else                                                      \
            _LABEL(labid):

#define DRAW(mode) \
    _BEFORE_AFTER(1, glBegin(mode), glEnd())

#endif