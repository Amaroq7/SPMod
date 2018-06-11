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

// native int numToString(int num, char[] buffer, int size)
static cell_t numToString(SourcePawn::IPluginContext *ctx,
                          const cell_t *params)
{
    auto numToConvert = params[1];
    auto numConverted = std::to_string(numToConvert);
    ctx->StringToLocal(params[2], params[3], numConverted.c_str());

    return numConverted.length();
}

// native int realToString(float real, char[] buffer, int size)
static cell_t realToString(SourcePawn::IPluginContext *ctx,
                           const cell_t *params)
{
    auto realToConvert = sp_ctof(params[1]);
    auto realConverted = std::to_string(realToConvert);
    ctx->StringToLocal(params[2], params[3], realConverted.c_str());

    return realConverted.length();
}

// native int copyString(char[] buffer, int size, const char[] source)
static cell_t copyString(SourcePawn::IPluginContext *ctx,
                         const cell_t *params)
{
    char *destArray, *stringToCopy;
    size_t arraySize = params[2];

    ctx->LocalToString(params[1], &destArray);
    ctx->LocalToString(params[3], &stringToCopy);

    size_t stringSize = strlen(stringToCopy);

    bool isStringBigger = stringSize >= arraySize;

#if defined __STDC_LIB_EXT1__ || defined SP_MSVC
    #if defined SP_MSVC
    strncpy_s(destArray, arraySize, stringToCopy, _TRUNCATE);
    #else
    strncpy_s(destArray, arraySize, stringToCopy, arraySize - 1);
    #endif
#else
    std::strncpy(destArray, stringToCopy, arraySize);

    if (isStringBigger)
        destArray[arraySize - 1] = '\0';
#endif

    return isStringBigger ? arraySize - 1 : stringSize;
}

sp_nativeinfo_t gStringNatives[] =
{
    {  "numToString",            numToString         },
    {  "realToString",           realToString        },
    {  "copyString",             copyString          },
    {  nullptr,                  nullptr             }
};