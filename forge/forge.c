#include "base/base.h"

#ifdef DEBUG

/**
 * TODO(nasr): implement this posix flag
 * */
#define FLAGS
#elif defined(RELEASE)
#define FLAGS
#endif

local_internal void
compile(char *path)
{
    /*
    TODO(nasr): checkout tsoding nob

   */
}

local_internal int
build(int argc, char **argv)
{
    if (argc > 0)
    {
        /*
      TODO(nasr): do something
     */
    }

    compile(*argv);
    return 0;
}
