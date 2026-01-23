#include "grpc/grpc_include.h"
#include <stdio.h>

int
main(void)
{
    printf("running");
    grpc_server_setup();
    return 0;
}
