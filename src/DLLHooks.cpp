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

#include "SPGlobal.hpp"
#include "DLLHooks.hpp"
#include "AnubisInit.hpp"

#include <anubis/engine/IEdict.hpp>
#include <anubis/game/IHooks.hpp>

namespace SPMod
{
    void installDLLHooks()
    {
        nstd::observer_ptr<Anubis::Game::IHooks> hooks = gGame->getHooks();
        static nstd::observer_ptr<PlayerMngr> playerMngr = gSPGlobal->getPlayerManager();

        hooks->gameInit()->registerHook([](nstd::observer_ptr<Anubis::Game::IGameInitHook> hook) {

            gSPGlobal->loadExts();

            // Allow plugins to add their natives
            for (auto &interface : gSPGlobal->getAdaptersInterfaces())
            {
                interface.second->getPluginMngr()->loadPlugins();
            }

            for (auto &interface : gSPGlobal->getAdaptersInterfaces())
            {
                interface.second->getPluginMngr()->bindPluginsNatives();
            }

            nstd::observer_ptr<GameInitHookRegistry> hookchain = gSPGlobal->getHooks()->gameInit();

            hookchain->callChain([hook]() {
                hook->callNext();
            }, [hook]() {
                hook->callOriginal();
            });

            gEngine->registerSrvCommand("spmod", CommandMngr::SPModInfoCommand, Anubis::FuncCallType::Direct);
        }, Anubis::HookPriority::Default);

        hooks->clientConnect()->registerHook([](Anubis::Game::IClientConnectHook *hook,
                                                Anubis::Engine::IEdict *pEdict,
                                                std::string_view name,
                                                std::string_view address,
                                                std::string &reason) {
            static nstd::observer_ptr<ClientConnectHookRegistry> hookchain = gSPGlobal->getHooks()->clientConnect();

            return hookchain->callChain([hook](nstd::observer_ptr<Anubis::Engine::IEdict> pEdict,
                                    std::string_view name,
                                    std::string_view address,
                                    std::string &reason) {
                if (bool result = hook->callNext(pEdict, name, address, reason); !result)
                {
                    return false;
                }

                return playerMngr->ClientConnect(pEdict, name, address, reason);
            }, [hook](nstd::observer_ptr<Anubis::Engine::IEdict> pEdict,
                      std::string_view name,
                      std::string_view address,
                      std::string &reason) {
                if (bool result = hook->callOriginal(pEdict, name, address, reason); !result)
                {
                    return false;
                }

                return playerMngr->ClientConnect(pEdict, name, address, reason);
            }, pEdict, name, address, reason);
        }, Anubis::HookPriority::Default);

        hooks->clientCmd()->registerHook([](Metamod::Game::IClientCmdHook *hook,
                                            Metamod::Engine::IEdict *pEdict) {
            std::string_view strCmd = gEngine->cmdArgv(0, Metamod::FuncCallType::Direct);

            if (strCmd == "menuselect" && !gSPGlobal->getMenuManager()->ClientCommand(pEdict))
            {
                return;
            }

            if (!gSPGlobal->getCommandManager()->ClientCommand(pEdict, strCmd))
            {
                return;
            }

            static ClientCmdHookRegistry *hookchain = gSPGlobal->getHooks()->clientCmd();
            hookchain->callChain([hook](Metamod::Engine::IEdict *pEdict) {
                hook->callNext(pEdict);
            }, [hook](Metamod::Engine::IEdict *pEdict) {
                hook->callOriginal(pEdict);
            }, pEdict);
        });

        hooks->clientPutinServer()->registerHook([](Metamod::Game::IClientPutinServerHook *hook,
                                                    Metamod::Engine::IEdict *pEdict) {
            static ClientPutinServerHookRegistry *hookchain = gSPGlobal->getHooks()->clientPutinServer();
            hookchain->callChain([hook](Metamod::Engine::IEdict *pEdict) {
                hook->callNext(pEdict);
                gSPGlobal->getPlayerManager()->ClientPutInServer(pEdict);
            }, [hook](Metamod::Engine::IEdict *pEdict) {
                hook->callOriginal(pEdict);
                gSPGlobal->getPlayerManager()->ClientPutInServer(pEdict);
            }, pEdict);
        });

        hooks->clientInfoChanged()->registerHook([](Metamod::Game::IClientInfoChangedHook *hook,
                                                    Metamod::Engine::IEdict *pEdict, Metamod::Engine::InfoBuffer infoBuffer)
        {
            static ClientInfoChangedHookRegistry *hookchain = gSPGlobal->getHooks()->clientInfoChanged();
            hookchain->callChain([hook](Metamod::Engine::IEdict *pEdict, Metamod::Engine::InfoBuffer infoBuffer) {
                hook->callNext(pEdict, infoBuffer);
                playerMngr->ClientUserInfoChanged(pEdict, infoBuffer);
            }, [hook](Metamod::Engine::IEdict *pEdict, Metamod::Engine::InfoBuffer infoBuffer) {
                hook->callOriginal(pEdict, infoBuffer);
                playerMngr->ClientUserInfoChanged(pEdict, infoBuffer);
            }, pEdict, infoBuffer);
        });

        hooks->serverActivate()->registerHook([](Metamod::Game::IServerActivateHook *hook,
                                              std::uint32_t edictCount, std::uint32_t clientMax) {

            static ServerActivateHookRegistry *hookchain = gSPGlobal->getHooks()->serverActivate();
            hookchain->callChain([hook](std::uint32_t edictCount, std::uint32_t clientMax) {
                hook->callNext(edictCount, clientMax);
                playerMngr->ServerActivate();
            }, [hook](std::uint32_t edictCount, std::uint32_t clientMax) {
                hook->callOriginal(edictCount, clientMax);
                playerMngr->ServerActivate();
            }, edictCount, clientMax);
        });

        hooks->serverDeactivate()->registerHook([](Metamod::Game::IServerDeactivateHook *hook) {
            static ServerDeactivateHookRegistry *hookchain = gSPGlobal->getHooks()->serverDeactivate();
            hookchain->callChain([hook]() {
                hook->callNext();
                playerMngr->ServerDeactivate();
            }, [hook]() {
                hook->callOriginal();
                playerMngr->ServerDeactivate();
            });
        });

        hooks->startFrame()->registerHook([](Metamod::Game::IStartFrameHook *hook) {
            static StartFrameHookRegistry *hookchain = gSPGlobal->getHooks()->startFrame();
            hookchain->callChain([hook]() {
                hook->callNext();
            }, [hook]() {
                hook->callOriginal();
            });

            playerMngr->StartFrame();
            float gpGlobalsTime = gEngine->getTime();

            if (TimerMngr::m_nextExecution <= gpGlobalsTime)
            {
                TimerMngr::m_nextExecution = gpGlobalsTime + 0.1f;
                gSPGlobal->getTimerManager()->execTimers(gpGlobalsTime);
            }
        });

        hooks->gameShutdown()->registerHook([](Metamod::Game::IGameShutdownHook *hook) {

            GameShutdownHookRegistry *hookchain = gSPGlobal->getHooks()->gameShutdown();

            hookchain->callChain([hook]() {
                hook->callNext();
            }, [hook]() {
                hook->callOriginal();
            });

            gSPGlobal->getTimerManager()->clearTimers();
            gSPGlobal->getCommandManager()->clearCommands();
            gSPGlobal->getMenuManager()->clearMenus();

            gEngine->removeCmd("spmod");
        });
    }
}
