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
