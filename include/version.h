#pragma once
// FIRMWARE_VERSION_DATE is injected at build time by tools/gen_version.py
// via -DFIRMWARE_VERSION_DATE="..." in the compiler build flags.
// This fallback handles IDE / static-analysis tools that bypass the
// PlatformIO build step (e.g. IntelliSense, clangd, cppcheck).
#ifndef FIRMWARE_VERSION_DATE
#  define FIRMWARE_VERSION_DATE "unknown"
#endif
