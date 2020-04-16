#include <spmod>

public PluginInfo pluginInfo =
{
	name = "Message test",
	version = "0.0.0",
	author = "author",
	url = "https://github.com/Amaroq7/SPMod"
};

MessageHook h1, h2;

public void OnPluginInit()
{
	Command("say /mblock", MsgTest);
	Command("say /h1", MsgTesth1);
	Command("say /h11", MsgTesth11);
	h1 = HookMessage(GetUserMsgId("SayText"), MessageHandler, false);
	h2 = HookMessage(GetUserMsgId("SayText"), MessageHandler, true);
}
public PluginReturn MsgTest(int client, Command cid)
{
	SetMsgBlock(76, MessageBlock_Once);
}
public PluginReturn MsgTesth1(int client, Command cid)
{
	PrintToServer("hook id %d", h1);
	UnhookMessage(h1);
}
public PluginReturn MsgTesth11(int client, Command cid)
{
	PrintToServer("11hook id %d", h1);
	h1 = HookMessage(GetUserMsgId("SayText"), MessageHandler, false);
	PrintToServer("11hook id %d post", h1);
}
public PluginReturn MessageHandler(MessageDest dest, int type, int receiver)
{
	PrintToServer("MsgHook: dest %d, %d type, receiver %d", dest, type, receiver);
	
	int params = GetMsgArgs();
	
	for(int i = 0; i < params; i++)
	{
		switch(GetMsgArgType(i))
		{
			case MsgArg_Byte, MsgArg_Char, MsgArg_Short, MsgArg_Long, MsgArg_Entity:
			{
				PrintToServer("param %d - %d", i, GetMsgArgInt(i));
			}
			case MsgArg_Angle, MsgArg_Coord:
			{
				PrintToServer("param %d - %f", i, GetMsgArgFloat(i));
			}
			case MsgArg_String:
			{
				char buffer[256];
				GetMsgArgString(i, buffer, sizeof buffer);
				PrintToServer("param %d - %s", i, buffer);
				if( i == 3) SetMsgArgString(i, "changed");
			}
		}
	}
}
