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

namespace SPMod
{
    constexpr std::size_t MAX_USER_MESSAGES = 256U;

    enum class MsgParamType
    {
        Byte = 1,
        Char,
        Short,
        Long,
        Angle,
        Coord,
        String,
        Entity,
    };

    enum class BlockType
    {
        Not,
        Once,
        Set
    };

    class IEdict;

    class IMessage
    {
    public:
        virtual ~IMessage() = default;

        virtual std::size_t getParams() const = 0;

        virtual MsgParamType getParamType(std::size_t index) const = 0;

        virtual int getParamInt(std::size_t index) const = 0;
        virtual float getParamFloat(std::size_t index) const = 0;
        virtual std::string_view getParamString(std::size_t index) const = 0;

        virtual void setParamInt(std::size_t index, int value) = 0;
        virtual void setParamFloat(std::size_t index, float value) = 0;
        virtual void setParamString(std::size_t index, std::string_view string) = 0;

        virtual int getDest() const = 0;
        virtual int getType() const = 0;
        virtual const float *getOrigin() const = 0;
        virtual IEdict *getEdict() const = 0;
    };

    using MessageHandler = std::function<IForward::ReturnValue(IMessage *const message, std::any cbData)>;

    class IMessageHook
    {
    public:
        virtual ~IMessageHook() = default;

        virtual void enable() = 0;
        virtual void disable() = 0;
        virtual bool isActive() const = 0;
        virtual int getMsgType() const = 0;
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

        virtual IMessageHook *registerHook(int msgType, MessageHandler handler, std::any cbData, bool post) = 0;
        virtual void unregisterHook(IMessageHook *hook) = 0;

        virtual BlockType getMessageBlock(int msgType) const = 0;
        virtual void setMessageBlock(int msgType, BlockType blockType) = 0;

        virtual IMessage *getMessage() const = 0;
        virtual bool inHook() const = 0;
    };
} // namespace SPMod
