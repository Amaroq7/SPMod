/*
*
*    This program is free software; you can redistribute it and/or modify it
*    under the terms of the GNU General Public License as published by the
*    Free Software Foundation; either version 2 of the License, or (at
*    your option) any later version.
*
*    This program is distributed in the hope that it will be useful, but
*    WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software Foundation,
*    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*    In addition, as a special exception, the author gives permission to
*    link the code of this program with the Half-Life Game Engine ("HL
*    Engine") and Modified Game Libraries ("MODs") developed by Valve,
*    L.L.C ("Valve").  You must obey the GNU General Public License in all
*    respects for all of the code used other than the HL Engine and MODs
*    from Valve.  If you modify this file, you may extend this exception
*    to your version of the file, but you are not obligated to do so.  If
*    you do not wish to do so, delete this exception statement from your
*    version.
*
*/

#pragma once

#include <IHookChains.hpp>
#include <forward_list>
#include <stdexcept>
#include <functional>
#include <memory>

namespace SPMod
{
    template<typename t_ret, typename... t_args>
    class HookInfo : public IHookInfo
    {
    public:
        HookInfo(HookFunc<t_ret, t_args...> func, HookPriority priority) : m_hookFunc(func), m_priority(priority), m_state(State::Enabled) {}
        ~HookInfo() override = default;

        void setState(State state) override
        {
            m_state = state;
        }

        HookPriority getPriority() const override
        {
            return m_priority;
        }

        const HookFunc<t_ret, t_args...> &getHookFn() const
        {
            return m_hookFunc;
        }

        bool isEnabled() const
        {
            return m_state == State::Enabled;
        }

    private:
        HookFunc<t_ret, t_args...> m_hookFunc;
        HookPriority m_priority;
        State m_state;
    };

    // Implementation for chains in modules
    template<typename t_ret, typename... t_args>
    class Hook final : public IHook<t_ret, t_args...>
    {
    public:
        Hook(typename std::forward_list<std::unique_ptr<HookInfo<t_ret, t_args...>>>::iterator iter,
             const std::forward_list<std::unique_ptr<HookInfo<t_ret, t_args...>>> &hooks,
             OriginalFunc<t_ret, t_args...> orig)
            : m_iter(iter), m_hooks(hooks), m_origFunc(orig)
        {}

        Hook(typename std::forward_list<std::unique_ptr<HookInfo<t_ret, t_args...>>>::iterator iter,
             const std::forward_list<std::unique_ptr<HookInfo<t_ret, t_args...>>> &hooks,
             OriginalFunc<t_ret, t_args...> orig,
             OriginalFunc<t_ret, t_args...> last)
            : m_iter(iter), m_hooks(hooks), m_origFunc(orig), m_lastFn(last)
        {}

        ~Hook() override = default;

        t_ret callNext(t_args... args) override
        {
            if (m_iter != m_hooks.end())
            {
                const auto &currentHook = *m_iter;

                do
                {
                    ++m_iter;
                } while (m_iter != m_hooks.end() && !(*m_iter)->isEnabled());

                Hook nextChain(m_iter, m_hooks, m_origFunc, m_lastFn);
                return std::invoke(currentHook->getHookFn(), &nextChain, std::forward<t_args>(args)...);
            }

            if (m_lastFn)
            {
                return std::invoke(m_lastFn, std::forward<t_args>(args)...);
            }

            return callOriginal(std::forward<t_args>(args)...);
        }

        t_ret callOriginal(t_args... args) const override
        {
            return std::invoke(m_origFunc, std::forward<t_args>(args)...);
        }

    protected:
        typename std::forward_list<std::unique_ptr<HookInfo<t_ret, t_args...>>>::iterator m_iter;
        const std::forward_list<std::unique_ptr<HookInfo<t_ret, t_args...>>> &m_hooks;
        OriginalFunc<t_ret, t_args...> m_origFunc;
        OriginalFunc<t_ret, t_args...> m_lastFn;
    };

    template<typename t_ret, typename... t_args>
    class HookRegistry : public IHookRegistry<t_ret, t_args...>
    {
    public:
        ~HookRegistry() override = default;

        t_ret callChain(OriginalFunc<t_ret, t_args...> origFunc, t_args... args)
        {
            if (hasHooks())
            {
                auto iter = m_hooks.begin();
                while (iter != m_hooks.end() && !(*iter)->isEnabled())
                {
                    ++iter;
                }

                Hook<t_ret, t_args...> chain(iter, m_hooks, origFunc);
                return chain.callNext(std::forward<t_args>(args)...);
            }

            return origFunc(std::forward<t_args>(args)...);
        }

        t_ret callChain(OriginalFunc<t_ret, t_args...> lastFunc, OriginalFunc<t_ret, t_args...> origFunc, t_args... args)
        {
            if (hasHooks())
            {
                auto iter = m_hooks.begin();
                while (iter != m_hooks.end() && !(*iter)->isEnabled())
                {
                    ++iter;
                }

                Hook<t_ret, t_args...> chain(iter, m_hooks, origFunc, lastFunc);
                return chain.callNext(std::forward<t_args>(args)...);
            }

            return lastFunc(std::forward<t_args>(args)...);
        }

        bool hasHooks() const
        {
            return !m_hooks.empty();
        }

        HookInfo<t_ret, t_args...> *registerHook(HookFunc<t_ret, t_args...> hook, HookPriority priority) override
        {
            if (!hook)
            {
                // TODO: Error message
                return nullptr;
            }

            auto hookInfo = std::make_unique<HookInfo<t_ret, t_args...>>(hook, priority);

            if (!hasHooks())
            {
                return m_hooks.emplace_front(std::move(hookInfo)).get();
            }

            typename std::forward_list<std::unique_ptr<HookInfo<t_ret, t_args...>>>::iterator prevIter = m_hooks.before_begin();
            for (auto it = m_hooks.begin(); it != m_hooks.end(); ++it)
            {
                if ((*it)->getPriority() < priority)
                {
                    return (*m_hooks.emplace_after(prevIter, std::move(hookInfo))).get();
                }

                prevIter = it;
            }

            return (*m_hooks.emplace_after(prevIter, std::move(hookInfo))).get();
        }

        void unregisterHook(IHookInfo *hookInfo) override
        {
            m_hooks.remove_if([hookInfo](const std::unique_ptr<HookInfo<t_ret, t_args...>> &hook) {
              return hookInfo == hook.get();
            });
        }

    private:
        std::forward_list<std::unique_ptr<HookInfo<t_ret, t_args...>>> m_hooks;
    };
}