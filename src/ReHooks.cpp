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

#include "ReHooks.hpp"

#include "SPGlobal.hpp"
#include "MetaInit.hpp"

#include <metamod/engine/IHooks.hpp>
#include <metamod/engine/IGameClient.hpp>

void installReHooks()
{
    using namespace SPMod;
    static Metamod::Engine::IHooks *hooks = gMetaAPI->getEngine()->getHooks();
    static PlayerMngr *playerMngr = gSPGlobal->getPlayerManager();

    hooks->svDropClient()->registerHook(
        [](Metamod::Engine::ISVDropClientHook *hook,
           Metamod::Engine::IGameClient *cl, bool crash, std::string_view string) {

        static DropClientHookRegistry *spHook = gSPGlobal->getHooks()->dropClient();

        spHook->callChain([hook, cl](Metamod::Engine::IEdict *pEdict, bool crash, std::string_view string) {
            hook->callNext(cl, crash, string);
            playerMngr->ClientDrop(pEdict, crash, string);
        }, [hook, cl](Metamod::Engine::IEdict *pEdict, bool crash, std::string_view string) {
            hook->callOriginal(cl, crash, string);
            playerMngr->ClientDrop(pEdict, crash, string);
        }, cl->getEdict(), crash, string);
    });
}