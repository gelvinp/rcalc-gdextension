#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

if "-std=c++17" in env["CXXFLAGS"]:
    env["CXXFLAGS"].remove("-std=c++17")
if "-fno-exceptions" in env["CXXFLAGS"]:
    env["CXXFLAGS"].remove("-fno-exceptions")

env.Append(CXXFLAGS=["-std=c++20"])

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/", "vendor/librcalc/include"])
env.Append(CXXFLAGS=["-isystem", "vendor/librcalc/include/modules/"])
sources = Glob("src/*.cpp")

output_path = "bin/librcalcgd{}{}".format(env["suffix"], env["SHLIBSUFFIX"])
macos_output_path = "bin/librcalcgd.{}.{}.framework/librcalcgd.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        )

env.Append(LIBPATH=["./vendor/librcalc/lib"])
if env["target"] == "template_release":
    if env["platform"] == "linux":
        if env["arch"] == "x86_64":
            env.Append(LIBS=["rcalc.linux.release.x86_64"])
            env.Depends(output_path, "vendor/librcalc/lib/librcalc.linux.release.x86_64.a")
    elif env["platform"] == "macos":
        env.Append(LIBS=["rcalc.macos.release.universal.a"])
        env.Depends(macos_output_path, "vendor/librcalc/lib/librcalc.macos.release.universal.a")
    elif env["platform"] == "windows":
        if env["arch"] == "x86_64":
            env.Append(LIBS=["librcalc.win.release.x86_64.mingw"])
            env.Depends(output_path, "vendor/librcalc/lib/librcalc.win.release.x86_64.mingw.a")
else:
    if env["platform"] == "linux":
        if env["arch"] == "x86_64":
            env.Append(LIBS=["rcalc.linux.debug.x86_64"])
            env.Depends(output_path, "vendor/librcalc/lib/librcalc.linux.debug.x86_64.a")
    elif env["platform"] == "macos":
        env.Append(LIBS=["rcalc.macos.debug.universal.a"])
        env.Depends(macos_output_path, "vendor/librcalc/lib/librcalc.macos.debug.universal.a")
    elif env["platform"] == "windows":
        if env["arch"] == "x86_64":
            env.Append(LIBS=["librcalc.win.debug.x86_64.mingw"])
            env.Depends(output_path, "vendor/librcalc/lib/librcalc.win.debug.x86_64.mingw.a")

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "bin/librcalcgd.{}.{}.framework/librcalcgd.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        output_path,
        source=sources,
    )

Default(library)
