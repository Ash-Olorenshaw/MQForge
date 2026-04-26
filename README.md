# MQForge
**MetaQuotes Forge**
alpha v0.0.1

A tool for making compiling MetaQuotes files from the commandline using the MetaQuotes compiler `Meta Editor` easier!

`MQForge` is designed for use with MQ4 code (should probably work with MQ5 too, but untested). It does *not* replace `Meta Editor` as compiler, it simply extends its functionality to make it easier to compile `.mq4` files from the commandline.


## Features

- better error messages output directly in your terminal:

	![Coloured outputs](/Screenshots/main.png?raw=true "Coloured output example")
- simplicity:
	```bash
	# instead of
 	/path/to/metaeditor.exe /compile:"file.mq4" /log:errors.log

 	# simply run
 	MQForge

 	# or
 	MQForge -me /path/to/metaeditor.exe
 	# if you don't have a config file setup.
 	```
- easy multifile compilation

  	If you use `#import` instead of `#include`, `MQForge` simplifies compiling all these files by establishing a dependency tree before compiling in order.

### Internals

`MQForge` follows these rough steps when performing a compile:
- read all files recursively in current directory
- establish dependency tree (warn/stop if dependencies not found)
- compile all files in the correct sequence using `metaeditor.exe`
- print all warnings, errors, etc to the commandline (instead of inside a `.log` file)

## Usage

Setting up and using `MQForge` is pretty simple.

To get a binary, head over to the [**Releases**](https://github.com/Ash-Olorenshaw/MQForge/releases) page and grab the .zip relevant to your platform and extract it. It should contain a single binary, which is all you need.

Once you have your binary, if you won't be using a `forge_commands.json` file, you simply invoke it in the directory where your files are with the path to your `metaeditor.exe` file, like so:
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

if you want to make compiling easier if you're going to be compiling over and over, you can define a `forge_commands.json` file in the root of your project (an example titled `forge_commands_example.json` is in the root of this project). This file gives you all the same options that the flags do; here is an example file with the default values:
```json
{
	"meta_editor" : "/path/to/metaeditor.exe",
	"default_header_location" : "path/to/MQL4/Include/",
	"use_wine" : false,
	"compiler_colours" : true,
	"suppress_errors" : false,
	"use_path" : true,
	"target" : "*"
}
```

With this file it makes it even easier. You can just run MQForge like this in your project directory:
```nu-script
MQForge
```

The only additional parameter of note is `target` in `forge_commands.json`, which does not have a
corresponding compile flag. This can either be a string `"*"` representing that you want all files
compiled or you can provide an array of strings of the files that you want.

The `target` parameter will spider out and find dependencies of the file you are compiling, so you
do not need to specify them all in the array.

## Building

Currently MQForge is only setup for building with Linux, however, you can still build on Windows with WSL.

The main reason for this is that building relies on Bash and GNU C extensions in the codebase. This
means that generally you'll want to use the `mingw` version of `gcc` to compile as opposed to
`msvc`. I used to `CMake`, but I just vastly prefer writing my compiliation scripts in C.

```nu-script
# clone this repo
git clone https://github.com/Ash-Olorenshaw/MQForge.git

# cd into dir
cd MQForge
```

If you are using a Posix shell (Bash, Dash, Zsh, etc) then just do the following
```nu-script
# make script executable
chmod +x ./compile.c

# run
./compile.c [COMPILER]
```
There are also two optional flags that can be passed to `./compile.c`: `run` and `debug`.

If you aren't using a Posix compliant shell (Pwsh, Fish, NuShell) you can either do:
```
bash ./runbench.c [COMPILER]
```

Or the following:

```nu-script
# gcc is not necessary, just for example
# compile
gcc "$0" \
	./build_scripts/utils.c ./build_scripts/args.c \
	-o ./compile

# make executable
chmod +x ./compile

# run
./compile [COMPILER]
```

After these steps, you should either have a `MQForge` or `MQForge.exe` file in the build directory for your use!

## Credits

[cJSON](https://github.com/DaveGamble/cJSON) - used for JSON parsing, an excellent library!


