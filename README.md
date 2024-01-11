# RCalc GDExtension

This repository contains the required components to build the GDExtension that powers [RCalc-GD](https://github.com/gelvinp/rcalc-gd), the editor plugin for Godot.

## Building

In order to build this GDExtension, you will need staticlib builds of RCalc.

To obtain these staticlib builds, follow these steps:

1. Clone the [RCalc](https://github.com/gelvinp/rcalc) repository and make sure to init + update its submodules.

2. Inside the RCalc root, run `scons build_type=staticlib` to generate a debug library, and `scons build_type=staticlib target=release` to generate a release library. More instructions on building RCalc can be found on its [wiki](https://github.com/gelvinp/rcalc/wiki).

3. The output of the staticlib builds are zip files named `lib_rclac.{platform}.{target}.{arch}.zip`. Extract both of these to `vendor/librcalc/` in the rcalc-gdextension root. You should have `vendor/librcalc/include/...` and `vendor/librcalc/lib/...`

4. From the rcalc-gdextension root, run `scons target=template_debug` to generate a debug gdextension, and `scons target=template_release` to generate a release gdextension. These output files can be found in `bin/`, and should be copied into `addons/rcalc_gd/bin/` inside the RCalc-GD root.

The following files are expected to exist in the `vendor/librcalc/lib` directory:

| Platform | target=`template_debug`           | target=`template_release`           |
| -------- | --------------------------------- | ----------------------------------- |
| Linux    | librcalc.linux.debug.x86_64.a     | librcalc.linux.release.x86_64.a     |
| MacOS    | librcalc.macos.debug.universal.a  | librcalc.macos.release.universal.a  |
| Windows  | librcalc.win.debug.x86_64.mingw.a | librcalc.win.release.x86_64.mingw.a |

Note than on **MacOS**, libraries are expected to be universal binaries containing both x86_64 and arm64 code. Build all four variants of the RCalc staticlib, and use `lipo` to create the universal binaries. For quick testing, you can also just rename the libraries, but the resulting GDExtension will only run on your architecture.

Note that on **Windows**, the staticlib is required to be built using MinGW (`scons use_mingw=yes`). MSVC builds of the staticlib are incompatible with the `godot-cpp` library due to differences in build configuration. Namely, Godot uses the release C runtime in debug builds, while RCalc uses the debug C runtime in debug builds, which improves debugging but is **not redistributable.** for this reason, only MinGW is supported for RCalc-GD.
