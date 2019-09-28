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
            return nullptr;
        }
    }

    std::size_t getKey(T *data)
    {
        for (const auto &handler : m_handlers)
        {
            if (handler.second == data)
                return handler.first;
        }

        return -1;
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
        m_freeIds.clear();
    }

private:
    std::unordered_map<std::size_t, T *> m_handlers;
    std::queue<std::size_t> m_freeIds;
};
