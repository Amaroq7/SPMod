# SPMod [![License](https://img.shields.io/badge/License-GPLv3-brightgreen.svg?style=flat-square)](LICENSE)
SourcePawn Scripting Engine for Half-Life 1 based games

## About

SPMod is a Metamod's plugin which is intended to be used as plugin system for Half-Life 1 and its derivatives. It uses [SourcePawn](https://github.com/alliedmodders/sourcepawn) as scripting language and allows to change completely the rules of a game or can ease the server administration by adding commands. SPMod's abilites can be extended by writing extensions, known as modules.

### Motivation

You may ask, why you should use this project while there are more mature and ready to use addons out there, which do the same, like [AMX Mod X](http://www.amxmodx.org) or [AMX Mod](http://www.amxmod.net), answer is simple, you shouldn't! It's just a prototype, not production ready in any mean, but you still can build and give it a try!

Goals:
* modern codebase
* modern API for modules
* actively developed scripting language

### Example plugin

```sourcepawn
struct PluginInfo
{
   public const char[] name;
   public const char[] version;
   public const char[] author;
   public const char[] url;
};

public PluginInfo pluginInfo =
{
	name = "test",
    version = "0.0.0",
	author = "author",
	url = "https://github.com/Amaroq7/SPMod"
};

native printToConsole(const char[] text);
forward void pluginInit();

public void pluginInit()
{
    printToConsole("Test output\n");
}
```

## Building & Installing

Steps to build SPMod from source and install it can be found project's wiki soon.

## API Reference

API is still in development, it'll be located on project's wiki.

## License

SPMod is licensed under the General Public License version 3 - see the [LICENSE](LICENSE) file for details.
