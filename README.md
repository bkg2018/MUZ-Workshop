# MUZ-Workshop

***NOTICE: this is preliminary work ***

MUZ is a Z-80 development workshop for MacOS with sub components aimed at all OS-es, inspired by the [Small Computer Monitor Workshop from Steven Cousins](https://smallcomputercentral.wordpress.com/projects/small-computer-workshop/).

MUZ C++ code is C++ 11 compliant. Its heart is a static library with an internal **Z-80 assembler** and a **computer model**. For the initial Z-80 version, MUZ computer model is integrating the [YAZE Z-80 emulator](http://www.mathematik.uni-ulm.de/users/ag/yaze-ag) as virtual CPU and implements classes for the [RC2014 computer](https://rc2014.co.uk/).

* ***MUZ-Computer*** has been designed to emulate the RC2014 built with a Backplane Pro and Z-80 CPU, Clock, Pageable ROM, 64KB RAM and SIO/2 Serial modules. *This is work in progress and is not available yet.*

* ***MUZ-Assembler*** is designed to be compatible with Small Computer Workshop outputs, and will compile the SC Monitor sources with no modification. MUZ-Assembler features a lot of helps for the programmer, like cross-references, label listings and warnings. See more details in [MUZ-Assembler documentation](MUZ/Documentation/MUZ-Assembler.md).

* ***MUZ-Workshop*** is an EDI project in Swift for MacOS XCode. It links with the C++ MUZ library into a multi editor / debugguer / assembler development system, trying to bring to the Mac the same services as Steve Cousins SCWorkshop on Windows. *This is work in progress and is not available yet.* A Windows and Linux version might be done later, but Steve Cousins' own Workshop is the reference on Windows and there's no real need for another one.

At the time of this writing the only available executable is **`asmuz`**, a command-line utility around MUZ-Assembler which allows you to build HEX files and listings for [Steve Cousins Small Computer Monitor](https://smallcomputercentral.wordpress.com/projects/small-computer-monitor/). This tool can be built with XCode 10 on MacOS, Visual Studio 2017 on Windows 10, you will find the workspaces/solution under the `Platform` directory. It builds on Debian using Eclipse + CDT too and could be done using simple makefiles but this is not available in the Git deposit yet.


What you will find in directories: 

* `MUZ` : This is the main directory for the C++ code base and associated files
  * `asmuz` : main source directory for `asmuz` command-line tool sources
  * `Documentation` : holds various documents in Markdown formats, most are work docs and should not be considered as final specifications
  * `muzlib` : main source directory for `muzlib` library 
      * `MUZ-Assembler` : source files related to the `Assembler` class
      * `MUZ-Common` : source files common to all parts of MUZ
      * `MUZ-Computer`: source files related to the computer model classes (memory modules, port modus, computer, cpu ...)
    * `Tests` : source files for testing programs, they feature a main which must be linked to `muzlib`
  * `TestSources` : Z-80 assembler source files for unit-tests
  * `YAZE`: YAZE emulator source files integrated for MUZ
      
* `Platform` : Main directory for OS specific files
  * `MacOS` : directory for MacOS
      * `asmuz` : directory for the XCode project for command-line utility 
      * `MUZ-Workshop` : directory for the MacOS workshop sources
      * `MUZ-WorkshopTests`: directory for the MacOS unit tests
      * `muzlib` : directory for the MacOS project
      * `MUZ-Workshop.xcworkspace` : XCode 10 workspace for the Workshop application, the `muzlib` 
  * `Win` : directory for Windows
      * `asmuz` : directory for the Visual Studio 2017 project for command-line utility 
      * `MUZ-Workshop` : directory for the workshop project and sources
      * `muzlib` : directory for the Visual Studio 2017 project
      * `MUZ-Workshop.sln` : Visual Studio 2017 Solution for the Workshop application, the `muzlib` library and the `asmuz` command-line tool.



December 2018

Francis Piérot

