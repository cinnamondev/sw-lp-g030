# sw-lp-g030

Software for the STM32G030F6P6 on-board the Line Sensor Module.

## Development

### Building

Development Dependencies
- Internet connection
- Powershell (optional, required if using provided VSCode configurations) (if on Windows, packaged version is fine).

All other dependencies will be pulled OTA via vcpkg (for build dependences), which will install cmsis toolbox (which will be able to download and install [KEIL CMSIS-Packs](https://www.keil.arm.com/packs/) for BSP).

Use Open-CMSIS-Pack tools.
Uses STM32 BSP

### Using Open-CMSIS-Pack csolutions: VSCode

As Keil Studio / Mbed Studio are both based (indirectly) off FOSS Code, there is a suite of extensions available for CMSIS csolution projects.

These have been included here in the CMSIS.code-profile. This provides a visual studio code profile that will install the extensions required.  Use VSCode if you want to make your life easier, else you can use the CMSIS-Toolbox via command line or scripts.

VSCode tasks are provided for program/debug, though they likely will require modification
as they are configured for debuggers other than a CMSIS-DAP.

### Code styling

Code style is enforced by Clang-Format ([LLVM Style](https://llvm.org/docs/CodingStandards.html#header-guard)). All contributed code should follow this style.
