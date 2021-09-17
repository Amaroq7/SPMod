/*
 *  Copyright (C) 2018-2020 SPMod Development Team
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

#include "StandardHeaders.hpp"
#include "IInterface.hpp"

#include <anubis/observer_ptr.hpp>

namespace SPMod
{
    class ITimer
    {
    public:
        enum class Return : std::uint8_t
        {
            Continue = 0,
            Stop
        };

    public:
        /**
         * @brief Timer callback
         *
         * @return  True to continue executing timer, false to remove.
         */
        using Callback = std::function<Return(nstd::observer_ptr<ITimer> timer)>;

    public:
        virtual ~ITimer() = default;

        /**
         * @brief Gets interval.
         *
         * @return        Timer interval.
         */
        [[nodiscard]] virtual float getInterval() const = 0;

        /**
         * @brief Checks if timer is paused.
         *
         * @return        True if paused, false otherwise.
         */
        [[nodiscard]] virtual bool isPaused() const = 0;

        /**
         * @brief Sets new interval.
         *
         * @param newInterval  New interval to set.
         */
        virtual void setInterval(float newInterval) = 0;

        /**
         * @brief Pauses or unpauses a timer.
         *
         * @param pause   True to pause timer, false to unpause it.
         */
        virtual void setPause(bool pause) = 0;
    };

    class ITimerMngr : public ISPModInterface
    {
    public:
        static constexpr std::uint16_t MAJOR_VERSION = 0;
        static constexpr std::uint16_t MINOR_VERSION = 0;

        static constexpr std::uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);

        /**
         * @brief Gets interface's name.
         *
         * @return              Interface's name.
         */
        [[nodiscard]] std::string_view getName() const override
        {
            return "ITimerMngr";
        }

        ~ITimerMngr() override = default;

        /**
         * @brief Gets interface's version.
         *
         * @note The first 16 most significant bits represent major version, the rest represent minor version.
         *
         * @return      Interface's version.
         */
        [[nodiscard]] std::uint32_t getVersion() const override
        {
            return VERSION;
        }

        /**
         * @brief Registers a new timer.
         *
         * @param interval    Time interval.
         * @param func        Callback function.
         * @param pause       True if timer should be paused after creation, false otherwise.
         *
         * @return            Created timer.
         */
        virtual nstd::observer_ptr<ITimer> createTimer(float interval, ITimer::Callback func, bool pause) = 0;

        /**
         * @brief Removes a timer.
         *
         * @note  Removing timer in the callback should be done by returning false.
         *
         * @param timer       Timer to delete.
         */
        virtual void removeTimer(nstd::observer_ptr<ITimer> timer) = 0;

        /**
         * @brief Executes a timer.
         *
         * @param timer       Timer to be executed.
         */
        virtual void execTimer(nstd::observer_ptr<ITimer> timer) = 0;
    };
} // namespace SPMod