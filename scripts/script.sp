#include <spmod>

public PluginInfo pluginInfo =
{
	name = "test",
    version = "0.0.0",
	author = "author",
	url = "https://github.com/Amaroq7/SPMod"
};

public void OnPluginInit()
{
    printToServer("Test output\n");
}
