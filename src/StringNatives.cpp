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

// int NumToString(int num, char[] buffer, int size)
static cell_t NumToString(SourcePawn::IPluginContext *ctx,
                          const cell_t *params)
{
    enum { arg_int = 1, arg_buffer, arg_size };

    auto numToConvert = params[arg_int];
    auto numConverted = std::to_string(numToConvert);
    ctx->StringToLocal(params[arg_buffer], params[arg_size], numConverted.c_str());

    return numConverted.length();
}

// int RealToString(float real, char[] buffer, int size)
static cell_t RealToString(SourcePawn::IPluginContext *ctx,
                           const cell_t *params)
{
    enum { arg_real = 1, arg_buffer, arg_size };

    auto realToConvert = sp_ctof(params[arg_real]);
    auto realConverted = std::to_string(realToConvert);
    ctx->StringToLocal(params[arg_buffer], params[arg_size], realConverted.c_str());

    return realConverted.length();
}

// int CopyString(char[] buffer, int size, const char[] source)
static cell_t CopyString(SourcePawn::IPluginContext *ctx,
                         const cell_t *params)
{
    enum { arg_buffer = 1, arg_size, arg_source };

    char *destArray, *stringToCopy;
    ctx->LocalToString(params[arg_buffer], &destArray);
    ctx->LocalToString(params[arg_source], &stringToCopy);

    return gSPGlobal->getUtilsCore()->strCopy(destArray, params[arg_size], stringToCopy);
}

sp_nativeinfo_t gStringNatives[] =
{
    {  "NumToString",            NumToString         },
    {  "RealToString",           RealToString        },
    {  "CopyString",             CopyString          },
    {  nullptr,                  nullptr             }
};
