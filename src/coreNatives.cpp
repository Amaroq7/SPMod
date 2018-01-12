/*  SPMod - SourcePawn Scripting Engine for Half-Life
 *  Copyright (C) 2018  SPMod Development Team
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

#include "spmod.hpp"

// native void printToConsole(const char[] text)
static cell_t core_printToConsole(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    char *stringToPrint;
    ctx->LocalToString(params[1], &stringToPrint);

    SERVER_PRINT(stringToPrint);

    return 1;
}

// native int precacheModel(const char[] model)
static cell_t core_precacheModel(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    char *modelToPrecache;
    ctx->LocalToString(params[1], &modelToPrecache);

    return PRECACHE_MODEL(STRING(ALLOC_STRING(modelToPrecache)));
}

// native int precacheSound(const char[] sound)
static cell_t core_precacheSound(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    char *soundToPrecache;
    ctx->LocalToString(params[1], &soundToPrecache);

    return PRECACHE_SOUND(STRING(ALLOC_STRING(soundToPrecache)));
}

// native int precacheGeneric(const char[] generic)
static cell_t core_precacheGeneric(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    char *genericToPrecache;
    ctx->LocalToString(params[1], &genericToPrecache);

    return PRECACHE_GENERIC(STRING(ALLOC_STRING(genericToPrecache)));
}

// native int numToString(int num, char[] converted_num, int size)
static cell_t core_NumToString(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    auto numToConvert = params[1];
    auto numConverted = std::to_string(numToConvert);
    ctx->StringToLocal(params[2], params[3], numConverted.c_str());

    return numConverted.length();
}

static cell_t core_CopyString(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    char *destArray, *stringToCopy;
    ctx->LocalToString(params[1], &destArray);
    ctx->LocalToString(params[3], &stringToCopy);

    std::strncpy(destArray, stringToCopy, params[2]);

    return 1;
}

sp_nativeinfo_t gCoreNatives[] =
{
    { "printToConsole",     core_printToConsole     },
    { "precacheModel",      core_precacheModel      },
    { "precacheSound",      core_precacheSound      },
    { "precacheGeneric",    core_precacheGeneric    },
    { "numToString",        core_NumToString        },
    { "copyString",         core_CopyString         },
    { nullptr,              nullptr                 }
};
