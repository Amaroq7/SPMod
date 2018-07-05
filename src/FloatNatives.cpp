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

static cell_t __float_ctor(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                           const cell_t *params)
{
    enum { arg_int = 1 };

    return sp_ftoc(static_cast<float>(params[arg_int]));
}

static cell_t __float_add(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                          const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return sp_ftoc(sp_ctof(params[arg_float1]) + sp_ctof(params[arg_float2]));
}

static cell_t __float_sub(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                          const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return sp_ftoc(sp_ctof(params[arg_float1]) - sp_ctof(params[arg_float2]));
}

static cell_t __float_mul(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                          const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return sp_ftoc(sp_ctof(params[arg_float1]) * sp_ctof(params[arg_float2]));
}

static cell_t __float_div(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                          const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return sp_ftoc(sp_ctof(params[arg_float1]) / sp_ctof(params[arg_float2]));
}

static cell_t __float_gt(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                         const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return static_cast<cell_t>(sp_ctof(params[arg_float1]) > sp_ctof(params[arg_float2]));
}

static cell_t __float_ge(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                         const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return static_cast<cell_t>(sp_ctof(params[arg_float1]) >= sp_ctof(params[arg_float2]));
}

static cell_t __float_lt(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                         const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return static_cast<cell_t>(sp_ctof(params[arg_float1]) < sp_ctof(params[arg_float2]));
}

static cell_t __float_le(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                         const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return static_cast<cell_t>(sp_ctof(params[arg_float1]) <= sp_ctof(params[arg_float2]));
}

static cell_t __float_eq(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                         const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return static_cast<cell_t>(sp_ctof(params[arg_float1]) == sp_ctof(params[arg_float2]));
}

static cell_t __float_ne(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                         const cell_t *params)
{
    enum { arg_float1 = 1, arg_float2 };

    return static_cast<cell_t>(sp_ctof(params[arg_float1]) != sp_ctof(params[arg_float2]));
}

static cell_t __float_not(SourcePawn::IPluginContext *ctx [[maybe_unused]],
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

    return sp_ftoc(std::ceil(sp_ctof(params[arg_float1])));
}

static cell_t RoundToFloor(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                           const cell_t *params)
{
    enum { arg_float1 = 1 };

    return sp_ftoc(std::floor(sp_ctof(params[arg_float1])));
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

    return sp_ftoc(value);
}

sp_nativeinfo_t gFloatNatives[] =
{
    { "__float_ctor",   __float_ctor   },
    { "__float_add",    __float_add    },
    { "__float_sub",    __float_sub    },
    { "__float_mul",    __float_mul    },
    { "__float_div",    __float_div    },
    { "__float_gt",     __float_gt     },
    { "__float_ge",     __float_ge     },
    { "__float_lt",     __float_lt     },
    { "__float_le",     __float_le     },
    { "__float_eq",     __float_eq     },
    { "__float_ne",     __float_ne     },
    { "__float_not",    __float_not    },
    { "FloatAbs",       FloatAbs       },
    { "RoundToCeil",    RoundToCeil    },
    { "RoundToFloor",   RoundToFloor   },
    { "RoundToNearest", RoundToNearest },
    { "RoundToZero",    RoundToZero    },
    { nullptr,          nullptr        }
};