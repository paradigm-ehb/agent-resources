#ifndef PKGMANAGER_H
#define PKGMANAGER_H

#include "arena.h"
#include "base.h"

enum LinuxDistro
{
  DISTRO_UNKNOWN = 0,

  DISTRO_ARCH_LINUX,
  DISTRO_MANJARO,
  DISTRO_ENDEAVOUROS,
  DISTRO_ARCO,
  DISTRO_GARUDA,

  DISTRO_DEBIAN,
  DISTRO_UBUNTU,
  DISTRO_LINUX_MINT,
  DISTRO_POP_OS,

  DISTRO_FEDORA,
  DISTRO_RHEL,
  DISTRO_CENTOS_STREAM,
  DISTRO_ROCKY,
  DISTRO_ALMA,

  DISTRO_GENTOO,
  DISTRO_NIXOS,
  DISTRO_OPENSUSE,

  DISTRO_COUNT


};

struct PackageManager
{
  char *name;
  char *binary_path;
};

struct DistroStack
{
  PackageManager package_manager;
  LinuxDistro linux_distro;
};

internal LinuxDistro
find_lxd_pkgm(mem_arena *arena);

#endif
