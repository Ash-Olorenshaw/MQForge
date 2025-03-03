# MQForge
**MetaQuotes Forge**
alpha v0.0.1

A tool for making compiling MetaQuotes files from the commandline using the MetaQuotes compiler `Meta Editor` easier!

`MQForge` is designed for use with MQ4 code (should probably work with MQ5 too, but untested). It does *not* replace `Meta Editor` as compiler, it simply extends its functionality to make it easier to compile `.mq4` files from the commandline. 

Some of the really cool (in my humble opinion) features of `MQForge` are:

`coloured outputs`:
![Coloured outputs](/Screenshots/main.png?raw=true "Coloured output example")


`MQForge` achieves this through these steps:
- read all files recursively in current directory
- establish dependency tree (warn/stop if dependencies not found)
- compile all files in the correct sequence using `metaeditor.exe`
- print all warnings, errors, etc to the commandline (instead of inside a `.log` file)

## Usage

Setting up and using `MQForge` is pretty simple. Once you have your binary, if you won't be using a `forge_commands.json` file, you simply invoke it in the directory where your files are with the path to your `metaeditor.exe` file, like so:
```nu-script
MQForge -me /path/to/metaeditor.exe
```

If you want to run `MQForge` in a specific directory, pass it a filepath as its first argument like so:
```nu-script
MQForge /path/to/project -me /path/to/metaeditor.exe
```

Further functionality does exist, and it can be seen through the help file, which is invoked with the `--help` flag, which is also shown here:

<details>
    <summary>Helpfile</summary>

    MQForge

    OPTIONAL: 
    [<filepath>]

    REQUIRED: 
    [-me/--meta-editor]       -   Location for your metaeditor.exe file.

    OPTIONAL:
    [-h/--help]               -   print this helpfile
    [-v/--version]            -   print program version
    [-dh/--default-headers]   -   Location directory of standard .mqh header files (this is usually something like 'C:/users/USERNAME/AppData/Roaming/MetaQuotes/Terminal/USERID/MQL4/Include/').
    [-wine/--use-wine]        -   (default: false) Whether to use Wine to run 'metaeditor.exe' - only available on Linux. 
    [-clr/--colourful]        -   (default: true) Whether to provide a coloured output.
    [-se/--suppress-errors]   -   (default: false) Whether to suppress launch errors for metaeditor.exe (really only matters with Wine).
    [-path/--use-path]        -   (default: true) Whether to search your PATH for .ex4, .dll, and .mqh files.
    [-s/--alt-settings]       -   Alternate settings file as opposed to the default 'forge_commands.json' file.

</details>

Boolean flags can be specified either by just passing the flag like this:
```nu-script
MQForge --use-wine
```

or, if you want a specific non-default value (like false), you can pass it like this:
```nu-script
MQForge --use-wine false
```

if you want to make compiling easier if you're going to be compiling over and over, you can define a `forge_commands.json` file in the root of your project. This file gives you all the same options that the flags do; here is an example file with the default values:
```json
{
	"meta_editor" : "/path/to/metaeditor.exe",
	"default_header_location" : "path/to/MQL4/Include/",
	"use_wine" : false,
	"compiler_colours" : true,
	"suppress_errors" : false,
	"use_path" : true
}
```

With this file it makes it even easier. You can just run MQForge like this in your project directory:
```nu-script
MQForge
```
## Building

Currently MQForge is only setup for building with Linux, however, you can still build on Windows with WSL.

MQForge works using `i686-w64-mingw32` for building for Windows and `gcc` for Linux.

To build, follow the following:

```nu-script
# clone this repo
git clone https://github.com/Ash-Olorenshaw/MQForge.git

# cd into dir
cd MQForge

# building for Linux:
cmake -DTARGET_PLATFORM=Linux -B build -S .

# building for Windows:
cmake -DTARGET_PLATFORM=Windows -B build -S . 

# cd into build directory
cd build 

# make the project
make
```

After these steps, you should either have a `MQForge` or `MQForge.exe` file in the build directory for your use!


