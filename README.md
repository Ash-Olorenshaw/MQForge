# MQForge
pre-alpha 0.0.1

**MetaQuotes Forge**

*(name still in progress - might be changed haha)*

## Welcome to MQForge!
A tool for making compiling MetaQuotes files from the commandline using the MetaQuotes compiler `Meta Editor` easier!

`MQForge` is designed for use with MQ4 code (should probably work with MQ5 too, but untested). It does *not* replace `Meta Editor` as compiler, it simply extends its functionality to make it easier to compile `.mq4` files from the commandline. 

`MQForge` achieves this through these steps:
- read all files recursively in current directory
- establish dependency tree (warn/stop if dependencies not found)
- compile all files in the correct sequence using `metaeditor.exe`
- print all warnings, errors, etc to the commandline (instead of inside a `.log` file)

The setting up and using `MQForge` is pretty simple. Once you have your binary, if you won't be using a `forge_commands.json` file, you simply invoke it in the directory where your files are with the path to your `metaeditor.exe` file, like so:
```nu-script
MQForge -me "/path/to/metaeditor.exe"
```
Yep, as simple as that!

Further functionality does exist, however, and it can be seen through the help file, which is invoked with the `--help` flag, which is also shown here:
```
-h/--help               -   print this helpfile
-me/--meta-editor       -   REQUIRED: Location for your metaeditor.exe file.
-dh/--default-headers   -   Location directory of standard .mqh header files (this is usually something like 'C:/users/USERNAME/AppData/Roaming/MetaQuotes/Terminal/USERID/MQL4/Include/').
-wine/--use-wine        -   Whether to use Wine to run 'metaeditor.exe' - only available on Linux. (default: false)
-clr/--colourful        -   Whether to provide a coloured output. (default: true)
-se/--suppress-errors   -   Whether to suppress launch errors for metaeditor.exe (really only matters with Wine). (default: false)
-path/--use-path        -   Whether to search your PATH for .ex4, .dll, and .mqh files. (default: true)
-s/--alt-settings       -   Alternate settings file as opposed to the default 'forge_commands.json' file.
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
```
MQForge
```
