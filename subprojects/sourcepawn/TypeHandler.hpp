/*
 *  Copyright (C) 2018-2020 SPMod Development Team
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
        }
        else
        {
            id = m_handlers.size();
        }

        m_handlers[id] = data;
        return id;
    }

    T *get(std::size_t id)
    {
        try
        {
            return m_handlers.at(id);
        }
        catch (const std::out_of_range &e)
        {
            (void)e;
            return nullptr;
        }
    }

    std::size_t getKey(T *data)
    {
        for (const auto &[key, content] : m_handlers)
        {
            if (content == data)
                return key;
        }

        return static_cast<std::size_t>(-1);
    }

    void free(std::size_t id)
    {
        if (m_handlers.find(id) == m_handlers.end() || !m_handlers[id])
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

private:
    std::unordered_map<std::size_t, T *> m_handlers;
    std::queue<std::size_t> m_freeIds;
};
