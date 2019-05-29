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

#pragma once

#if defined SP_POSIX
    #define _vsnprintf vsnprintf
#endif

#include <utlvector.h>

namespace SPMod
{
    class IPluginMngr;
    class IProxiedNative;

    class IPlugin
    {
    public:
        /**
         * @brief Returns name of plugin.
         *
         * @return        Plugin name.
         */
        virtual const char *getName() const = 0;

        /**
         * @brief Returns version of plugin.
         *
         * @return        Plugin version.
         */
        virtual const char *getVersion() const = 0;

        /**
         * @brief Returns author of plugin.
         *
         * @return        Plugin author.
         */
        virtual const char *getAuthor() const = 0;

        /**
         * @brief Returns url address of plugin.
         *
         * @return        Plugin url address.
         */
        virtual const char *getUrl() const = 0;

        /**
         * @brief Returns identity of plugin.
         *
         * @note          It's the same as getFilename() except it doesn't contain file extension.
         *
         * @return        Plugin identity.
         */
        virtual const char *getIdentity() const = 0;

        /**
         * @brief Returns filename of plugin.
         *
         * @return        Plugin filename.
         */
        virtual const char *getFilename() const = 0;

        /**
         * @brief Gets plugin's plugin manager.
         * 
         * @return        Plugin manager.
         */
        virtual IPluginMngr *getPluginMngr() const = 0;

        /**
         * @brief Gets proxied param as int.
         * 
         * @param index   Param number.
         * 
         * @return        Param as int.
         */
        virtual int getProxiedParamAsInt(std::size_t index) const = 0;

        /**
         * @brief Gets proxied param as pointer to int.
         * 
         * @param index   Param number.
         * 
         * @return        Param as pointer to int.
         */
        virtual int *getProxiedParamAsIntAddr(std::size_t index) const = 0;

        /**
         * @brief Gets proxied param as float.
         * 
         * @param index   Param number.
         * 
         * @return        Param as float.
         */
        virtual float getProxiedParamAsFloat(std::size_t index) const = 0;

        /**
         * @brief Gets proxied param as pointer to float.
         * 
         * @param index   Param number.
         * 
         * @return        Param as pointer to float.
         */
        virtual float *getProxiedParamAsFloatAddr(std::size_t index) const = 0;

        /**
         * @brief Gets proxied param as string.
         * 
         * @param index   Param number.
         * 
         * @return        Param as string.
         */
        virtual char *getProxiedParamAsString(std::size_t index) const = 0;

        /**
         * @brief Gets proxied param as array.
         * 
         * @param index   Param number.
         * 
         * @return        Param as pointer to float.
         */
        virtual void *getProxiedParamAsArray(std::size_t index) const = 0;

    protected:
        virtual ~IPlugin() = default;
    };

    class IPluginMngr
    {
    public:
        /**
         * @brief Searches for plugin by specified identity.
         *
         * @param name    Identity to search for.
         *
         * @return        Plugin pointer, nullptr if not found.
         */
        virtual IPlugin *getPlugin(const char *name) = 0;

        /**
         * @brief Loads plugins.
         *
         * @noreturn
         */
        virtual void loadPlugins() = 0;

        /**
         * @brief Binds natives created by other plugins.
         *
         * @noreturn
         */
        virtual void bindPluginsNatives() = 0;

        /**
         * @brief Unloads plugins.
         *
         * @noreturn
         */
        virtual void unloadPlugins() = 0;

        /**
         * @brief Called on proxied native.
         * 
         * @param native  Proxied native.
         * @param plugin  Plugin which executed the native.
         *
         * @return        Native result.
         */
        virtual int proxyNativeCallback(const IProxiedNative *const native, const IPlugin *const plugin) = 0;

        /**
         * @brief Returns numbers of loaded plugins.
         *
         * @return        Number of loaded plugins.
         */
        virtual std::size_t getPluginsNum() const = 0;

        /**
         * @brief Returns plugins extension.
         *
         * @return        Plugins extension.
         */
        virtual const char *getPluginsExt() const = 0;

        /**
         * @brief Returns list of the plugins.
         *
         * @return        Plugins list.
         */
        virtual const CUtlVector<IPlugin *> &getPluginsList() const = 0;

    protected:
        virtual ~IPluginMngr() = default;
    };
}
