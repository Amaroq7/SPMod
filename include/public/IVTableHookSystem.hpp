/*
 *  Copyright (C) 2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

namespace SPMod
{
    class IVTableHook
    {
    public:
        enum class Return : std::uint8_t
        {
            Ignored = 0,
            Handled,
            Override,
            Supercede
        };
        using Callback = std::function<Return(IVTableHook *const hook, bool post)>;
        using Param = std::variant<std::int32_t,
                                   std::uint32_t,
                                   std::int16_t,
                                   float,
                                   float *,
                                   Engine::IEntVars *,
                                   Engine::IEdict *,
                                   Engine::ITraceResult *,
                                   const char *>;

        virtual ~IVTableHook() = default;

        virtual const std::optional<Param> &getReturn() const = 0;
        virtual const std::optional<Param> &getOriginalReturn() const = 0;
        virtual std::vector<Param> &getParams() = 0;

        virtual void setReturn(Param retVal) = 0;
    };

    class IVTableHookManager : public ISPModInterface
    {
    public:
        enum class vFuncType : std::uint16_t
        {
            Spawn = 0,
            TakeDamage,
            TraceAttack
        };

        static constexpr std::uint16_t MAJOR_VERSION = 0;
        static constexpr std::uint16_t MINOR_VERSION = 0;

        static constexpr std::uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);

        /**
         * @brief Gets interface's name.
         *
         * @return      Interface's name.
         */
        std::string_view getName() const override
        {
            return "IVTableHookManager";
        }

        /**
         * @brief Gets interface's version.
         *
         * @note The first 16 most significant bits represent major version, the rest represent minor version.
         *
         * @return      Interface's version.
         */
        std::uint32_t getVersion() const override
        {
            return VERSION;
        }

        virtual ~IVTableHookManager() = default;

        virtual IVTableHook *registerHook(vFuncType vFunc, std::string_view classname) = 0;
        virtual void addCallback(IVTableHook::Callback callback) = 0;
    };
} // namespace SPMod