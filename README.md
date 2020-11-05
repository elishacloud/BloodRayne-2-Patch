# Windows 10 BloodRayne 2 Patch

### Introduction

This is a project that fixes the BloodRayne 2 'BR2 FSAA Patch 1.666' patch on Windows 8.1 and Windows 10.

### Installation

1. Download the latest release from the repository's [Releases](https://github.com/elishacloud/BloodRayne-2-Patch/releases) page and unzip all files it to the 'BloodRayne 2' folder, where the br2.exe file exists.
2. Run the 'br2fsaaConfig.exe' config tool and configure the settings you desire.  It is recommended to run "Auto Config" option to configure the patch correctly.
3. (optional) Modify the `d3d8.ini` file settings as you desire.
4. Make sure all compatibility settings are disabled.

![Compatibility Settings](https://raw.githubusercontent.com/elishacloud/wiki-attachments/master/BloodRayne-2-Patch/Compatibility.png)

### Uninstallation

Delete the following files from the 'BloodRayne 2' folder:
* br2Cuda.dll
* br2fsaa.ini
* br2fsaaConfig.exe
* br2hd.ini
* br2perlin.exe
* cudart32_32_16.dll
* d3d8.dll
* d3d8.ini
* d3d8.log
* d3d8patch.dll
* readme.html

### Troubleshooting

If you run into issues using the BloodRayne 2 patch, try running the `br2fsaaConfig.exe` program and then selecting the 'Auto Config' option. This should guide you through properly configuring the patch.

### Donations

All my work here is free and can be freely used.  For more details on how you can use this module see the [license](#license) section below.  However, if you would like to donate to me then check out my [donations page](https://PayPal.me/elishacloud).  All donations are for work already completed!  Please don't donate for future work or to try and increase my development speed.  Thanks!

### License

Copyright (C) 2020 Elisha Riedlinger

This software is provided 'as-is', without any express or implied warranty. In no event will the author(s) be held liable for any damages arising from the use of this software. Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

This project uses code from several other projects. Below is a list of locations that source code was taken from:

* [Aqrit's ddwrapper](http://bitpatch.com/ddwrapper.html): Includes code to read the ini config file.
* [d3d8to9](https://github.com/crosire/d3d8to9): Includes the full Direct3D 8 to Direct3D 9 code.
* [DxWrapper](https://github.com/elishacloud/dxwrapper): Includes code to create wrapper dlls and resets the display adapter.
* [DxWnd](https://sourceforge.net/projects/dxwnd/): Includes code from DxWnd for API hooking.
* [rohitab.com](http://www.rohitab.com/discuss/topic/40594-parsing-pe-export-table/): Code in GetProcAddress function taken from source code found on rohitab.com

### Development
This project is written in C++ using Microsoft Visual Studio Community 2017.

Thanks for stopping by!
