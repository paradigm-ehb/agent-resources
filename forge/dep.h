#define internal static
 struct package_set
{
  const char *manager;
  const char *packages[16];
};

internal struct package_set deps = {

#if defined(DISTRO_FEDORA)

  .manager = "dnf",
  .packages = {
    "grpc-devel",
    "protobuf-devel",
    "protobuf-compiler",
    "abseil-cpp-devel",
    "openssl-devel",
    "c-ares-devel",
    "re2-devel",
    "zlib-devel",
    NULL },

#elif defined(DISTRO_ARCH)

  .manager = "pacman",
  .packages = {
    "grpc",
    "protobuf",
    "abseil-cpp",
    "openssl",
    "c-ares",
    "re2",
    "zlib",
    NULL },

#else
/*
   * TODO(nasr): preprocessor error
   * */
#endif
};
