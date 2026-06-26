Import("env")
import subprocess
import os

GENERATED = "include/generated/firmware_version.h"

def _git(args):
    try:
        return subprocess.check_output(["git"] + args, stderr=subprocess.DEVNULL).decode().strip()
    except Exception:
        return ""

# git describe: clean tag → "v1.0.9", post-tag → "v1.0.9-5-gabcdef", dirty → appends "-dirty"
version = _git(["describe", "--tags", "--always", "--dirty"]) or "unknown"
print("[gen_version] {}".format(version))

# Primary mechanism: inject into C++ preprocessor via build flags.
# This works even if the generated header file cannot be written.
env.Append(CPPDEFINES=[("FIRMWARE_VERSION_DATE", '\\"{}\\\"'.format(version))])

# Secondary: write generated header for IDE/IntelliSense support (best-effort, non-fatal).
try:
    content = '#pragma once\n#define FIRMWARE_VERSION_DATE "{}"\n'.format(version)
    os.makedirs(os.path.dirname(GENERATED), exist_ok=True)
    existing = open(GENERATED).read() if os.path.exists(GENERATED) else ""
    if existing != content:
        with open(GENERATED, "w") as f:
            f.write(content)
except Exception as e:
    print("[gen_version] warning: could not write {}: {}".format(GENERATED, e))
