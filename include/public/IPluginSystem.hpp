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

#pragma once

namespace SPMod
{
    class IPlugin SPMOD_FINAL
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
         * @note          It's the same as getFilename() except it doesn't contain extension.
         *                For plugin named "example.smx" it returns "example".
         *
         * @return        Plugin identity.
         */
        virtual const char *getIndentity() const = 0;

        /**
         * @brief Returns filename of plugin.
         *
         * @return        Plugin filename.
         */
        virtual const char *getFileName() const = 0;

        /**
         * @brief Returns id of plugin.
         *
         * @return        Plugin id.
         */
        virtual size_t getId() const = 0;

        /**
         * @brief Returns SourcePawn runtime of plugin.
         *
         * @return        Plugin runtime.
         */
        virtual SourcePawn::IPluginRuntime *getRuntime() const = 0;

        /**
         * @brief Creates forward for plugin.
         *
         * @param name    Forward name.
         * @param owner   Module owner of forward.
         * @param params  Number of paramaters in forward.
         * @param ...     Type of parameters.
         *
         * @return        Forward pointer, nullptr if creation failed.
         */
        virtual IForward *createForward(const char *name,
                                        IModuleInterface *owner,
                                        size_t params,
                                        ...) const = 0;

        /**
         * @brief Creates forward for plugin.
         *
         * @param name    Forward name.
         * @param owner   Plugin owner of forward.
         * @param params  Number of paramaters in forward.
         * @param ...     Type of parameters.
         *
         * @return        Forward pointer, nullptr if creation failed.
         */
        virtual IForward *createForward(const char *name,
                                        IPlugin *owner,
                                        size_t params,
                                        ...) const = 0;

    protected:
        virtual ~IPlugin() {};
    };

    class IPluginMngr SPMOD_FINAL
    {
    public:

        /**
         * @brief Loads a plugin.
         *
         * @note          Loads a plugin present in "scripts" folder.
         *
         * @param name    Filename to load, must contain ".smx" extension.
         * @param error   Buffer to store an error message.
         * @param size    Size of the buffer to store the message.
         *
         * @return        Plugin pointer, nullptr if loading failed.
         */
        virtual IPlugin *loadPlugin(const char *name,
                                    char *error,
                                    size_t size) = 0;

        /**
         * @brief Searches for plugin by specified id.
         *
         * @param index   Index to search for.
         *
         * @return        Plugin pointer, nullptr if not found.
         */
        virtual IPlugin *getPlugin(size_t index) = 0;

        /**
         * @brief Searches for plugin by specified identity.
         *
         * @param name    Identity to search for.
         *
         * @return        Plugin pointer, nullptr if not found.
         */
        virtual IPlugin *getPlugin(const char *name) = 0;

        /**
         * @brief Searches for plugin by SourcePawn context.
         *
         * @param ctx     Find plugin with exact SourcePawn context.
         *
         * @return        Plugin pointer, nullptr if not found.
         */
        virtual IPlugin *getPlugin(SourcePawn::IPluginContext *ctx) = 0;

        /**
         * @brief Returns numbers of loaded plugins.
         *
         * @return        Number of loaded plugins.
         */
        virtual size_t getPluginsNum() const = 0;

    protected:
        virtual ~IPluginMngr() {};
    };
}
