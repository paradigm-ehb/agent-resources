#include "libres/resources.h"
#include "libpkg/pkg.h"

#include "base/base.h"
#include "base/base_arena.h"

#include "libres/resources.h"

#include <string.h>

local_internal char *
find_lxd_pkgm(mem_arena *arena)
{
    Device *device = device_create(arena);
    device_read(device);
    enum LinuxDistro lxd;

    i32   len             = sizeof(device->os_version);
    char *distro_unparsed = (char *)PUSH_ARRAY(arena, char, len);

    memcpy(distro_unparsed, device->os_version, len);

    i32   word_idx = 0;
    char *buffer   = (char *)arena_push(arena, len, 1);

    while (distro_unparsed[word_idx] != '\0')
    {
        /* TODO:
     *
     * Convert character to lowercase using ASCII ordering.
     *
     * - Check whether the character is an uppercase letter.
     * - Uppercase ASCII range: 'A' .. 'Z'.
     * - If the character falls within this range:
     *     - Convert it to lowercase by applying the ASCII offset.
     * - Rationale:
     *     - Lowercase letters appear after uppercase letters in ASCII.
     *     - Case conversion can be done via direct numeric comparison,
     *       without internale or library calls.
     */
        /*
      Calculate the lower-case upper-case differnce
    */
        i8 lwc_diff = 'a' - 'A';
        if (distro_unparsed[word_idx] != ' ')
        {
            if (distro_unparsed[word_idx] < 'a')
            {
                distro_unparsed[word_idx] += lwc_diff;
            }
            buffer[word_idx] = distro_unparsed[word_idx];
        }

        ++word_idx;
    }

    buffer[word_idx] = '\0';

    return buffer;
}
