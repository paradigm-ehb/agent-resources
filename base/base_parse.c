#include "base/base_parse.h"

local_internal b8
compare_string(const char *c1, const char *c2)
{
    if (sizeof(c1) != sizeof(c2))
    {
        return -1;
    }

    for (
    u64 word_idx = 0;
    word_idx <= sizeof(*c1);
    ++word_idx)
    {
        if (*c1 != *c2)
        {
            return -1;
        }
        ++c1;
        ++c2;
    }

    return 0;
}

local_internal char *
parse_proc_files(const char *path, const char *delim, global_arena *arena)
{
    if (!path || !delim || !arena)
    {
        test(0);
        return NULL;
    }

    umm delim_len = sizeof(*delim);
    int fd        = open(path, O_RDONLY);

    u64 bytes = 0;

    char  buffer[BUFFER_SIZE_SMALL];
    char *out;

    /*
      processor\t:0\n
      NOTE(nasr): interesting this loads the complete buffer at once
    */
    bytes = read(fd, buffer, sizeof(buffer));
    {
        char *start;
        char *end;

        for (u64 buffer_position = 0;
        buffer_position < bytes;
        ++buffer_position)

            for (u64 line_position = 0;
            line_position < bytes;
            ++line_position)
            {
                char *line_bf = &buffer[buffer_position];

                if (line_bf[line_position] == '\t' || line_bf[line_position] == ' ')
                {
                    continue;
                }

                if (line_bf[line_position] == delim[0])
                {
                    start = &line_bf[line_position];
                    continue;
                }

                start = &line_bf[line_position];
                if (line_bf[line_position] == '\n')
                {
                    end = &line_bf[line_position];
                    break;
                }
            }
        // break;
    }

    /*
     out = PUSH_ARRAY(arena, char, sizeof(end));
     MemCpy(out, end, sizeof(end));
    */
    close(fd);
    return out;
}

int
main(void)
{
    global_arena *arena = arena_create(MiB(8));

    const char *path  = "/proc/cpuinfo";
    const char *delim = ":";

    parse_proc_files(path, delim, arena);

    return 0;
}
