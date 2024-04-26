# sw-lp-g030

Software for the STM32G030F6P6 on-board the Line Sensor Module.

## Important!

This repositorys & corresponding documentation should be considered dead code.
The second microcontroller is no longer functional, while the code here should
work, it can no longer be programmed. Please consider this when reviewing the
documentation herein, thank you!


## Development

### Dependencies

- Windows/Linux/Other compatible environment (can run vcpkg)
- Alternatively:
  - arm-none-eabi-gcc OR armclang (arm compiler 6)
  - ninja
  - cmsis toolbox
  - cmake
  - openOCD
  - 
### Building

Building has been tested via the vcpkg environment and vscode configurations.

If you have an internet connection and can use vcpkg, vcpkg will pull in all the relevant build dependencies for you. This is much preferred because C/C++ tooling is a terrible rabbit hole.

CMSIS-Packs are an easy way to obtain major dependencies (i.e. arm math libraries, lvgl, bsp packages, etc, which will be able to download and install [KEIL CMSIS-Packs](https://www.keil.arm.com/packs/)).

Requires STM32Cube CMSIS-Pack for G030 target.

### Using Open-CMSIS-Pack csolutions: VSCode

*Useful for group members* 

As Keil Studio / Mbed Studio are both based (indirectly) off FOSS Code, there is a suite of extensions available for CMSIS csolution projects.

These have been included here in the CMSIS.code-profile. This provides a visual studio code profile that will install the extensions required.  Use VSCode if you want to make your life easier, else you can use the CMSIS-Toolbox via command line or scripts.

VSCode tasks are provided for program/debug, though they likely will require modification
as they are configured for debuggers other than a CMSIS-DAP.

### Code styling

Code style is enforced by Clang-Format ([LLVM Style](https://llvm.org/docs/CodingStandards.html#header-guard)). All contributed code should follow this style.

## PCB Schematic

See below for schematic:

\image{inline} html schematic.jpg height=712px
