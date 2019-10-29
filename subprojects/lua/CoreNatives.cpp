/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "ExtMain.hpp"

// native void PrintToServer(text)
static int PrintToServer(lua_State *luaState)
{
    enum
    {
        text = -1
    };

    char bufferOutput[1024];
    size_t textLen;
    gSPGlobal->getUtils()->strCopy(bufferOutput, sizeof(bufferOutput), lua_tolstring(luaState, text, &textLen));

    if (textLen > 1024)
    {
        bufferOutput[1022] = '\n';
        bufferOutput[1023] = '\0';
    }
    else
    {
        bufferOutput[textLen++] = '\n';
        bufferOutput[textLen] = '\0';
    }

    gSPGlobal->getEngineFuncs()->serverPrint(bufferOutput);

    return 1;
}

LuaAdapterCFucntion gCoreNatives[] = {{"PrintToServer", PrintToServer}, {nullptr, nullptr}};
