# cmsis-stm32

Open CMSIS Project template for target STM32F401RE. This includes the dependencies for the STM32CUBEF4 HAL configurable via RTE_Devices.h.


## Usage in Visual Studio Code

CMSIS.code-profile is a visual studio code profile that can be imported into your editor. it contains within itself any helpful extensions. This is of course, optional, but reccomended, as the CMSIS extensions for vscode will tie in very well and make life very easy. 
Clang format is also to be setup on the style would like to correspond in the [embedded systems project](https://github.com/embedded-systems-30), though the use of this template in particular there is unlikely. (Use KEIL / Mbed studio. though honestly im pretty sure its the same structure maybe with a few closed source extensions i have yet to look too closely. but it looks to me like kinda bs since it is indirectly based off VSCode.)

All actual dependencies (for the exception of st-link drivers) (compilers, etc...) will be downloaded via vcpkg artifacts (compilers, csolution setups, debug tools), then further any KEIL/CMSIS packs will be added by the CMSIS tools.

ARM embedded debugger is best for this task.

Visual Studio task configuration is also provided for ease of setup.

## Lab Shield Datasheet

### Display

https://www.seacomp.com/sites/default/files/references/Sitronix-ST7565R.pdf


https://www.st.com/en/development-tools/stsw-link009.html#get-software
st link drivers are needed for using the st link component of the dev board /w openocd.