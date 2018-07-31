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
        virtual const char *getFilename() const = 0;

        /**
         * @brief Returns id of plugin.
         *
         * @return        Plugin id.
         */
        virtual std::size_t getId() const = 0;

        /**
         * @brief Returns SourcePawn runtime of plugin.
         *
         * @return        Plugin runtime.
         */
        virtual SourcePawn::IPluginRuntime *getRuntime() const = 0;

    protected:
        virtual ~IPlugin() {};
    };

    class IPluginMngr SPMOD_FINAL : public ISPModInterface
    {
    public:
        static constexpr uint16_t MAJOR_VERSION = 0;
        static constexpr uint16_t MINOR_VERSION = 0;

        static constexpr uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);
        /**
         * @brief Gets interface's name.
         *
         * @return        Interface's name.
         */
        const char *getInterfaceName() const override
        {
            return "IPluginMngr";
        }

        /**
         * @brief Gets interface's version.
         *
         * @note The first 16 most significant bits represent major version, the rest represent minor version.
         *
         * @return        Interface's version.
         */
        uint32_t getInterfaceVersion() const override
        {
            return VERSION;
        }

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
                                    std::size_t size) = 0;

        /**
         * @brief Searches for plugin by specified id.
         *
         * @param index   Index to search for.
         *
         * @return        Plugin pointer, nullptr if not found.
         */
        virtual IPlugin *getPlugin(std::size_t index) = 0;

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

        /**
         * @brief Adds natives.
         *
         * @note Last entry has to be nullptr.
         *
         * @return        List of natives to add.
         */
        virtual bool addNatives(const sp_nativeinfo_t *natives) = 0;

    protected:
        virtual ~IPluginMngr() {};
    };
}
