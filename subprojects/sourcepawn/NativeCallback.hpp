/*
 *  Copyright (C) 2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <INativeProxy.hpp>

#include "SourcePawnAPI.hpp"

namespace SPExt
{
    class NativeCallback final : public SourcePawn::INativeCallback, public SPMod::IProxiedNative
    {
    public:
        NativeCallback(std::string_view name, SourcePawn::IPluginFunction *pluginFunction);
        explicit NativeCallback(SPMod::IProxiedNative *native);
        void AddRef() final;
        void Release() final;
        cell_t Invoke(SourcePawn::IPluginContext *ctx, const cell_t *params) final;
        std::int32_t InvokeSPModNative(SPMod::IPlugin *plugin) final;

        std::string_view getName() const final;
        std::int32_t getParamAsInt(std::size_t index) const final;
        std::int32_t *getParamAsIntAddr(std::size_t index) const final;
        float getParamAsFloat(std::size_t index) const final;
        float *getParamAsFloatAddr(std::size_t index) const final;
        char *getParamAsString(std::size_t index) const final;
        void *getParamAsArray(std::size_t index) const final;

        static inline std::stack<SPExt::NativeCallback *> currentPluginNative = {};

    private:
        std::string m_name;
        SourcePawn::IPluginFunction *m_pluginFunction = nullptr;
        SPMod::IProxiedNative *m_proxiedNative = nullptr;
        std::stack<SourcePawn::IPluginContext *> m_ctx;
        std::stack<const cell_t *> m_params;
        std::uint32_t m_refCount = 0;
    };
}