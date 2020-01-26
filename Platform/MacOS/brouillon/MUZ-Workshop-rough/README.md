# MacOS Specificities

Tested on Mojave (macOS 10.14)

* must delete sandboxing in target capabilities, or no access to desktop / documents etc
* output directories are created read-only by mkdir (222 umask ?), t least under desktop. Workaround is enabling write access from Finder or shell with chmod
