/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
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

#include "spmod.hpp"

// int Player.GetName(char[] buffer, int size)
static cell_t GetName(SourcePawn::IPluginContext *ctx,
                      const cell_t *params)
{
    enum { arg_id = 1, arg_buffer, arg_size };

    std::shared_ptr<Player> plr = gSPGlobal->getPlayerManagerCore()->getPlayerCore(params[arg_id]);

    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    char *buffer;
    ctx->LocalToString(params[arg_buffer], &buffer);

    return gSPGlobal->getUtilsCore()->strCopyCore(buffer, params[arg_size], plr->getNameCore());
}

// int Player.GetIP(char[] buffer, int size, bool port = false)
static cell_t GetIP(SourcePawn::IPluginContext *ctx,
                    const cell_t *params)
{
    enum { arg_id = 1, arg_buffer, arg_size, arg_port };

    std::shared_ptr<Player> plr = gSPGlobal->getPlayerManagerCore()->getPlayerCore(params[arg_id]);

    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    char *buffer;
    ctx->LocalToString(params[arg_buffer], &buffer);

    std::string IPAddress(plr->getIPAddressCore());

    if (!params[arg_port])
    {
        auto pos = IPAddress.find(':');
        if (pos != std::string::npos)
            IPAddress[pos] = '\0';
    }

    return gSPGlobal->getUtilsCore()->strCopyCore(buffer, params[arg_size], IPAddress);
}

// int Player.GetSteamID(char[] buffer, int size)
static cell_t GetSteamID(SourcePawn::IPluginContext *ctx,
                         const cell_t *params)
{
    enum { arg_id = 1, arg_buffer, arg_size };

    std::shared_ptr<Player> plr = gSPGlobal->getPlayerManagerCore()->getPlayerCore(params[arg_id]);

    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    char *buffer;
    ctx->LocalToString(params[arg_buffer], &buffer);

    return gSPGlobal->getUtilsCore()->strCopyCore(buffer, params[arg_size], plr->getSteamIDCore());
}

// int Player.Index.get()
static cell_t GetIndex(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                       const cell_t *params)
{
    enum { arg_id = 1 };

    std::shared_ptr<Player> plr = gSPGlobal->getPlayerManagerCore()->getPlayerCore(params[arg_id]);

    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    return plr->getIndex();
}

// int Player.UserID.get()
static cell_t GetUserID(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                        const cell_t *params)
{
    enum { arg_id = 1 };

    std::shared_ptr<Player> plr = gSPGlobal->getPlayerManagerCore()->getPlayerCore(params[arg_id]);

    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    return plr->getUserId();
}

// bool Player.Alive.get()
static cell_t AliveGet(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                       const cell_t *params)
{
    enum { arg_id = 1 };

    std::shared_ptr<Player> plr = gSPGlobal->getPlayerManagerCore()->getPlayerCore(params[arg_id]);

    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    return plr->isAlive();
}

// bool Player.Connected.get()
static cell_t ConnectedGet(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                           const cell_t *params)
{
    enum { arg_id = 1 };

    std::shared_ptr<Player> plr = gSPGlobal->getPlayerManagerCore()->getPlayerCore(params[arg_id]);

    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    return plr->isConnected();
}

// bool Player.Fake.get()
static cell_t FakeGet(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                      const cell_t *params)
{
    enum { arg_id = 1 };

    std::shared_ptr<Player> plr = gSPGlobal->getPlayerManagerCore()->getPlayerCore(params[arg_id]);

    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    return plr->isFake();
}

// bool Player.HLTV.get()
static cell_t HLTVGet(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                      const cell_t *params)
{
    enum { arg_id = 1 };

    std::shared_ptr<Player> plr = gSPGlobal->getPlayerManagerCore()->getPlayerCore(params[arg_id]);

    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    return plr->isHLTV();
}

// bool Player.InGame.get()
static cell_t InGame(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                     const cell_t *params)
{
    enum { arg_id = 1 };

    std::shared_ptr<Player> plr = gSPGlobal->getPlayerManagerCore()->getPlayerCore(params[arg_id]);

    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    return plr->isInGame();
}

sp_nativeinfo_t gPlayerNatives[] =
{
    { "Player.GetName",         GetName      },
    { "Player.GetIP",           GetIP        },
    { "Player.GetSteamID",      GetSteamID   },
    { "Player.Index.get",       GetIndex     },
    { "Player.UserID.get",      GetUserID    },
    { "Player.Alive.get",       AliveGet     },
    { "Player.Connected.get",   ConnectedGet },
    { "Player.Fake.get",        FakeGet      },
    { "Player.HLTV.get",        HLTVGet      },
    { "Player.InGame.get",      InGame       },
    { nullptr,                  nullptr      }
};
