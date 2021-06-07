/*
 *  Copyright (C) 2018-2021 SPMod Development Team
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

#include "StandardHeaders.hpp"
#include "IInterface.hpp"
#include "Common.hpp"
#include "IHookChains.hpp"

#include <metamodcpp_sdk/engine/Common.hpp>
#include <metamodcpp_sdk/engine/IEdict.hpp>

namespace SPMod
{
    constexpr std::size_t MAX_USER_MESSAGES = 256U;

    enum class MsgBlockType : std::uint8_t
    {
        Not = 0,
        Once,
        Set
    };

    class IMessage
    {
    public:
        using Handler = std::function<void(IHook<void, IMessage *> *chain, IMessage *message)>;
        using Param = std::variant<std::byte, char, std::int16_t, std::int32_t, std::string,
                                    Metamod::Engine::MsgEntity, Metamod::Engine::MsgAngle, Metamod::Engine::MsgCoord>;

    public:
        virtual ~IMessage() = default;

        virtual std::vector<Param> &getParams() = 0;
        virtual Metamod::Engine::MsgDest getDest() const = 0;
        virtual Metamod::Engine::MsgType getType() const = 0;
        virtual const float *getOrigin() const = 0;
        virtual Metamod::Engine::IEdict *getEdict() const = 0;

        virtual IHookInfo *registerHook(IMessage::Handler handler,
                                        HookPriority hookPriority = HookPriority::DEFAULT) = 0;
        virtual void unregisterHook(IHookInfo *hook) = 0;

        virtual MsgBlockType getBlockType() const = 0;
        virtual void setBlockType(MsgBlockType blockType) = 0;
    };

    class IMessageMngr : public ISPModInterface
    {
    public:
        virtual ~IMessageMngr() = default;

        static constexpr std::uint16_t MAJOR_VERSION = 0;
        static constexpr std::uint16_t MINOR_VERSION = 0;

        static constexpr std::uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);
        /**
         * @brief Gets interface's name.
         *
         * @return        Interface's name.
         */
        std::string_view getName() const override
        {
            return "IMessageMngr";
        }

        /**
         * @brief Gets interface's version.
         *
         * @note The first 16 most significant bits represent major version, the rest represent minor version.
         *
         * @return        Interface's version.
         */
        std::uint32_t getVersion() const override
        {
            return VERSION;
        }

        virtual IMessage *getMessage(Metamod::Engine::MsgType msgType) const = 0;
    };
} // namespace SPMod
