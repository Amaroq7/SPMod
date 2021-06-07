/*
 *  Copyright (C) 2018-2021 SPMod Development Team
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

#include <queue>
#include <stdexcept>

template<typename T>
class TypeHandler
{
public:
    std::size_t create(T *data)
    {
        std::size_t id = 0;
        if (!m_freeIds.empty())
        {
            id = m_freeIds.front();
            m_freeIds.pop();
            m_handlers[id] = data;
        }
        else
        {
            id = m_handlers.size();
            m_handlers.push_back(data);
        }

        return id;
    }

    T *get(std::size_t id) const
    {
        try
        {
            return m_handlers.at(id);
        }
        catch (const std::out_of_range &e [[maybe_unused]])
        {
            return nullptr;
        }
    }

    std::size_t getKey(T *data) const
    {
        std::size_t id = 0;
        for (const auto content : m_handlers)
        {
            if (content == data)
                return id;

            id++;
        }

        return static_cast<std::size_t>(-1);
    }

    void free(std::size_t id)
    {
        if (id >= m_handlers.size() || !m_handlers[id])
            return;

        m_handlers[id] = nullptr;
        m_freeIds.push(id);
    }

    void clear()
    {
        m_handlers.clear();
        while (!m_freeIds.empty())
        {
            m_freeIds.pop();
        }
    }

    auto begin()
    {
        return m_handlers.begin();
    }

    auto end()
    {
        return m_handlers.end();
    }

    bool empty()
    {
        return true;
    }

private:
    std::vector<T *> m_handlers;
    std::queue<std::size_t> m_freeIds;
};
