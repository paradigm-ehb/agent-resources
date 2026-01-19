#include "base/base_arena.c"
#include "base/base.c"
#include "logger/logger.c"
#include "journal/journald.c"
#include "libnet/net.c"
#include "libres/resources.cc"
#include "libvm/qc.c"
#include "auth/auth.cc"
#include "dbus/dbushandler.cc"
#include "grpc_service/grpc_service.cc"
#include "agent/core.cc"
#ifdef local_internal
#undef local_internal
#endif
#ifdef internal
#undef internal
#endif
/*
 * TODO(nasr): c++ versioning issue
 * */
/*
#include "libpkg/pkg.c"
#include "libpkg/distro/arch_pac.cc"
#include "libpkg/distro/debian_apt.cc"
#include "libpkg/distro/fedora_dnf.cc"
*/

