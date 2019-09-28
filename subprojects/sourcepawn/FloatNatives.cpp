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

#include "spmod.hpp"

static cell_t FloatCtor(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                        const cell_t *params)
{
    enum { arg_int = 1 };

    return sp_ftoc(static_cast<float>(params[arg_int]));
}

static cell_t FloatAdd(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                       const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return sp_ftoc(sp_ctof(params[arg_float1]) + sp_ctof(params[arg_float2]));
}

static cell_t FloatSub(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                       const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return sp_ftoc(sp_ctof(params[arg_float1]) - sp_ctof(params[arg_float2]));
}

static cell_t FloatMul(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                       const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return sp_ftoc(sp_ctof(params[arg_float1]) * sp_ctof(params[arg_float2]));
}

static cell_t FloatDiv(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                       const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return sp_ftoc(sp_ctof(params[arg_float1]) / sp_ctof(params[arg_float2]));
}

static cell_t FloatGt(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                      const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return static_cast<cell_t>(sp_ctof(params[arg_float1]) > sp_ctof(params[arg_float2]));
}

static cell_t FloatGe(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                      const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return static_cast<cell_t>(sp_ctof(params[arg_float1]) >= sp_ctof(params[arg_float2]));
}

static cell_t FloatLt(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                      const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return static_cast<cell_t>(sp_ctof(params[arg_float1]) < sp_ctof(params[arg_float2]));
}

static cell_t FloatLe(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                      const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return static_cast<cell_t>(sp_ctof(params[arg_float1]) <= sp_ctof(params[arg_float2]));
}

static cell_t FloatEq(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                      const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return static_cast<cell_t>(sp_ctof(params[arg_float1]) == sp_ctof(params[arg_float2]));
}

static cell_t FloatNe(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                      const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return static_cast<cell_t>(sp_ctof(params[arg_float1]) != sp_ctof(params[arg_float2]));
}

static cell_t FloatNot(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                       const cell_t *params)
{
    enum { arg_float1 = 1 };

    return (sp_ctof(params[arg_float1]) ? 0 : 1);
}

static cell_t FloatAbs(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                       const cell_t *params)
{
    enum { arg_float1 = 1 };

    return sp_ftoc(std::abs(sp_ctof(params[arg_float1])));
}

static cell_t RoundToCeil(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                          const cell_t *params)
{
    enum { arg_float1 = 1 };

    return static_cast<cell_t>(std::ceil(sp_ctof(params[arg_float1])));
}

static cell_t RoundToFloor(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                           const cell_t *params)
{
    enum { arg_float1 = 1 };

    return static_cast<cell_t>(std::floor(sp_ctof(params[arg_float1])));
}

static cell_t RoundToNearest(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                             const cell_t *params)
{
    enum { arg_float1 = 1 };

    return static_cast<cell_t>(sp_ctof(params[arg_float1]));
}

static cell_t RoundToZero(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                          const cell_t *params)
{
    enum { arg_float1 = 1 };

    float value = sp_ctof(params[arg_float1]);

    if (value >= 0.0f)
        value = std::floor(value);
    else
        value = std::ceil(value);

    return static_cast<cell_t>(value);
}

sp_nativeinfo_t gFloatNatives[] =
{
    { "__float_ctor",   FloatCtor      },
    { "__float_add",    FloatAdd       },
    { "__float_sub",    FloatSub       },
    { "__float_mul",    FloatMul       },
    { "__float_div",    FloatDiv       },
    { "__float_gt",     FloatGt        },
    { "__float_ge",     FloatGe        },
    { "__float_lt",     FloatLt        },
    { "__float_le",     FloatLe        },
    { "__float_eq",     FloatEq        },
    { "__float_ne",     FloatNe        },
    { "__float_not",    FloatNot       },
    { "FloatAbs",       FloatAbs       },
    { "RoundToCeil",    RoundToCeil    },
    { "RoundToFloor",   RoundToFloor   },
    { "RoundToNearest", RoundToNearest },
    { "RoundToZero",    RoundToZero    },
    { nullptr,          nullptr        }
};