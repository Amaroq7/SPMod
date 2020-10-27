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

#include "ExtMain.hpp"

// int Player.GetName(char[] buffer, int size)
static cell_t GetName(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_id = 1,
        arg_buffer,
        arg_size
    };

    SPMod::IPlayer *plr = gSPGlobal->getPlayerManager()->getPlayer(params[arg_id]);
    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    char *buffer;
    ctx->LocalToString(params[arg_buffer], &buffer);

    return gSPGlobal->getUtils()->strCopy(buffer, params[arg_size], plr->getName());
}

// int Player.GetIP(char[] buffer, int size, bool port = false)
static cell_t GetIP(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_id = 1,
        arg_buffer,
        arg_size,
        arg_port
    };

    SPMod::IPlayer *plr = gSPGlobal->getPlayerManager()->getPlayer(params[arg_id]);
    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    char *buffer;
    ctx->LocalToString(params[arg_buffer], &buffer);

    std::string IPAddress(plr->getIPAddress());
    if (!params[arg_port])
    {
        auto pos = IPAddress.find(':');
        if (pos != std::string::npos)
            IPAddress[pos] = '\0';
    }

    return gSPGlobal->getUtils()->strCopy(buffer, params[arg_size], IPAddress.c_str());
}

// int Player.GetSteamID(char[] buffer, int size)
static cell_t GetSteamID(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_id = 1,
        arg_buffer,
        arg_size
    };

    SPMod::IPlayer *plr = gSPGlobal->getPlayerManager()->getPlayer(params[arg_id]);
    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    char *buffer;
    ctx->LocalToString(params[arg_buffer], &buffer);

    return gSPGlobal->getUtils()->strCopy(buffer, params[arg_size], plr->getSteamID());
}

// int Player.Index.get()
static cell_t GetIndex(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_id = 1
    };

    SPMod::IPlayer *plr = gSPGlobal->getPlayerManager()->getPlayer(params[arg_id]);
    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    return plr->basePlayer()->edict()->getIndex();
}

// int Player.UserID.get()
static cell_t GetUserID(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_id = 1
    };

    SPMod::IPlayer *plr = gSPGlobal->getPlayerManager()->getPlayer(params[arg_id]);
    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    return plr->getUserId();
}

// bool Player.Alive.get()
static cell_t AliveGet(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_id = 1
    };

    SPMod::IPlayer *plr = gSPGlobal->getPlayerManager()->getPlayer(params[arg_id]);
    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    return plr->isAlive();
}

// bool Player.Connected.get()
static cell_t ConnectedGet(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_id = 1
    };

    SPMod::IPlayer *plr = gSPGlobal->getPlayerManager()->getPlayer(params[arg_id]);
    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    return plr->isConnected();
}

// bool Player.Fake.get()
static cell_t FakeGet(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_id = 1
    };

    SPMod::IPlayer *plr = gSPGlobal->getPlayerManager()->getPlayer(params[arg_id]);
    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    return plr->isFake();
}

// bool Player.HLTV.get()
static cell_t HLTVGet(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_id = 1
    };

    SPMod::IPlayer *plr = gSPGlobal->getPlayerManager()->getPlayer(params[arg_id]);
    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    return plr->isHLTV();
}

// bool Player.InGame.get()
static cell_t InGame(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_id = 1
    };

    SPMod::IPlayer *plr = gSPGlobal->getPlayerManager()->getPlayer(params[arg_id]);
    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    return plr->isInGame();
}

// int Player.Health.get()
static cell_t HealthGet(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_id = 1
    };

    SPMod::IPlayer *plr = gSPGlobal->getPlayerManager()->getPlayer(params[arg_id]);
    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    return 0; // static_cast<cell_t>(plr->getHealth());
}

// void Player.Health.set(int health)
static cell_t HealthSet(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_id = 1,
        arg_health
    };

    SPMod::IPlayer *plr = gSPGlobal->getPlayerManager()->getPlayer(params[arg_id]);
    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    // plr->setHealth(static_cast<float>(params[arg_health]));
    return 0; // 1;
}

// void Player.Health.set(int health)
static cell_t TakeDamage(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_id = 1,
        arg_attacker,
        arg_inflictor,
        arg_damage,
        arg_dmgbits
    };

    SPMod::IPlayer *plr = gSPGlobal->getPlayerManager()->getPlayer(params[arg_id]);
    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }
    auto attacker = gSPEngine->getEdict(params[arg_attacker])->getEntVars();
    auto inflictor = gSPEngine->getEdict(params[arg_inflictor])->getEntVars();

    return plr->basePlayer()->takeDamage(attacker, inflictor, sp_ctof(params[arg_damage]), params[arg_dmgbits]);
}

// native void SendMsg(TextMsgDest msgDest, const char[] text, any ...)
static cell_t SendMsg(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_id = 1,
        arg_type,
        arg_msg
    };

    SPMod::IPlayer *plr = gSPGlobal->getPlayerManager()->getPlayer(params[arg_id]);
    if (!plr)
    {
        ctx->ReportError("Non player index (%i)", params[arg_id]);
        return 0;
    }

    char *spFormatString;
    char bufferOutput[192];

    ctx->LocalToString(params[arg_msg], &spFormatString);
    std::size_t res = formatString(bufferOutput, sizeof(bufferOutput) - 1, spFormatString, ctx, params, 4);

    bufferOutput[res] = '\0';

    plr->sendMsg(static_cast<SPMod::TextMsgDest>(params[arg_type]), bufferOutput);

    return 1;
}

sp_nativeinfo_t gPlayerNatives[] = {{"Player.GetName", GetName},
                                    {"Player.GetIP", GetIP},
                                    {"Player.GetSteamID", GetSteamID},
                                    {"Player.Index.get", GetIndex},
                                    {"Player.UserID.get", GetUserID},
                                    {"Player.Alive.get", AliveGet},
                                    {"Player.Connected.get", ConnectedGet},
                                    {"Player.Fake.get", FakeGet},
                                    {"Player.HLTV.get", HLTVGet},
                                    {"Player.InGame.get", InGame},
                                    {"Player.Health.get", HealthGet},
                                    {"Player.Health.set", HealthSet},
                                    {"Player.TakeDamage", TakeDamage},
                                    {"Player.SendMsg", SendMsg},
                                    {nullptr, nullptr}};
