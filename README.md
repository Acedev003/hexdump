# hexdump
A commandline Hex-viewer tool written in C.

![image](/extras/hexdump_v0.1.0.png)

### Usage

`hexviewer [options] <filename>`

Options:

    -s: skips specified amount of bytes (Default value: 0)
    -n: interpret only 'n' bytes of input (Default value: 0) [Prints entire file if value is 0 or below]
    -f: Saves output into the specified file
    -h: View help

### Installing

Pre-built binaries (built on a 64 bit PC) for both windows and linux is already present in the `/bin` directory.

If you want to rebuild the project just run `make` in the root directory.

Using this project as a useful command-line requires pointing the `/bin` to the enviroment path in windows and/or linux respectively.
