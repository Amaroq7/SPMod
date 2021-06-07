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

#include "NativeCallback.hpp"
#include "PluginSystem.hpp"

namespace SPExt
{
    NativeCallback::NativeCallback(std::string_view name, SourcePawn::IPluginFunction *pluginFunction)
        : m_name(name), m_pluginFunction(pluginFunction)
    {
    }

    NativeCallback::NativeCallback(SPMod::IProxiedNative *native) : m_name(native->getName()), m_proxiedNative(native) {}

    void NativeCallback::AddRef()
    {
        m_refCount++;
    }

    void NativeCallback::Release()
    {
        if (--m_refCount <= 0)
        {
            delete this;
        }
    }

    cell_t NativeCallback::Invoke(SourcePawn::IPluginContext *ctx, const cell_t *params)
    {
        if (params[0] > SP_MAX_CALL_ARGUMENTS)
        {
            ctx->ReportError("Too many parameters passed to native %d (max: %d)", params[0], SP_MAX_CALL_ARGUMENTS);
            return 0;
        }

        m_ctx.emplace(ctx);
        m_params.emplace(params);
        NativeCallback::currentPluginNative.emplace(this);

        SPExt::Plugin *executor;
        ctx->GetKey(Plugin::KEY_PLUGIN, reinterpret_cast<void **>(&executor));

        cell_t result = 0;
        if (m_pluginFunction && m_pluginFunction->IsRunnable())
        {
            m_pluginFunction->PushCell(static_cast<cell_t>(executor->getId()));
            m_pluginFunction->Execute(&result);
        }
        else
        {
            result = m_proxiedNative->InvokeSPModNative(executor);
        }

        NativeCallback::currentPluginNative.pop();
        m_ctx.pop();
        m_params.pop();

        return result;
    }

    std::int32_t NativeCallback::getParamAsInt(std::size_t index) const
    {
        try
        {
            if (m_proxiedNative)
            {
                return m_proxiedNative->getParamAsInt(index);
            }
            return m_params.top()[index];
        }
        catch (const std::out_of_range &e [[maybe_unused]])
        {
            return 0;
        }
    }

    std::int32_t *NativeCallback::getParamAsIntAddr(std::size_t index) const
    {
        try
        {
            if (m_proxiedNative)
            {
                return m_proxiedNative->getParamAsIntAddr(index);
            }

            cell_t *result;
            cell_t address = m_params.top()[index];
            m_ctx.top()->LocalToPhysAddr(address, &result);
            return result;
        }
        catch (const std::out_of_range &e [[maybe_unused]])
        {
            return nullptr;
        }
    }

    float NativeCallback::getParamAsFloat(std::size_t index) const
    {
        try
        {
            if (m_proxiedNative)
            {
                return m_proxiedNative->getParamAsFloat(index);
            }
            return sp_ctof(m_params.top()[index]);
        }
        catch (const std::out_of_range &e [[maybe_unused]])
        {
            return 0;
        }
    }

    float *NativeCallback::getParamAsFloatAddr(std::size_t index) const
    {
        try
        {
            if (m_proxiedNative)
            {
                return m_proxiedNative->getParamAsFloatAddr(index);
            }

            cell_t *result;
            cell_t address = m_params.top()[index];
            m_ctx.top()->LocalToPhysAddr(address, &result);

            return reinterpret_cast<float *>(result);
        }
        catch (const std::out_of_range &e [[maybe_unused]])
        {
            return nullptr;
        }
    }

    char *NativeCallback::getParamAsString(std::size_t index) const
    {
        try
        {
            if (m_proxiedNative)
            {
                return m_proxiedNative->getParamAsString(index);
            }

            char *result;
            cell_t address = m_params.top()[index];
            m_ctx.top()->LocalToString(address, &result);

            return result;
        }
        catch (const std::out_of_range &e [[maybe_unused]])
        {
            return nullptr;
        }
    }

    void *NativeCallback::getParamAsArray(std::size_t index) const
    {
        try
        {
            if (m_proxiedNative)
            {
                return m_proxiedNative->getParamAsArray(index);
            }

            cell_t *result;
            cell_t address = m_params.top()[index];
            m_ctx.top()->LocalToPhysAddr(address, &result);

            return result;
        }
        catch (const std::out_of_range &e [[maybe_unused]])
        {
            return nullptr;
        }
    }

    std::string_view NativeCallback::getName() const
    {
        return m_name;
    }

    std::int32_t NativeCallback::InvokeSPModNative(SPMod::IPlugin *plugin)
    {
        cell_t result = 0;
        if (m_pluginFunction && m_pluginFunction->IsRunnable())
        {
            NativeCallback::currentPluginNative.emplace(this);
            m_pluginFunction->PushCell(static_cast<cell_t>(plugin->getId()));
            m_pluginFunction->Execute(&result);
            NativeCallback::currentPluginNative.pop();
        }
        return result;
    }
}