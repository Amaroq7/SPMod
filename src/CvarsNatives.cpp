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

static cell_t core_cvarRegister(SourcePawn::IPluginContext *ctx, const cell_t *params)
{	
	const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
	char *cvarName, *cvarValue;
	ctx->LocalToString(params[1], &cvarName);
	ctx->LocalToString(params[2], &cvarValue);
	

	auto plCvar = cvarMngr->registerOrFindCvar(cvarName, cvarValue, (ICvar::Flags)params[3], true);
	if (!plCvar)
		return -1;
	
	return plCvar->getId();
}

static cell_t core_cvarGetName(SourcePawn::IPluginContext *ctx,
	const cell_t *params)
{
	char *destBuffer;
	size_t bufferSize = params[3];

	cell_t cvarId = params[1];
	if (cvarId  < 0)
	{
		ctx->ReportError("Invalid cvar pointer!");
		return 0;
	}
	const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
	auto cvar = cvarMngr->findCvar(cvarId);
	if (!cvar)
	{
		ctx->ReportError("Cvar not found!");
		return 0;
	}

	ctx->LocalToString(params[2], &destBuffer);

#if defined __STDC_LIB_EXT1__ || defined SP_MSVC
#if defined SP_MSVC
	strncpy_s(destBuffer, bufferSize, cvar->getName(), _TRUNCATE);
#else
	strncpy_s(destBuffer, bufferSize, cvar->asString().c_str(), bufferSize - 1);
#endif
#else
	std::strncpy(destBuffer, cvar->asString().c_str(), bufferSize);
	destBuffer[bufferSize - 1] = '\0';
#endif

	return 1;
}


static cell_t core_cvarGetFloat(SourcePawn::IPluginContext *ctx,
                                const cell_t *params)
{
	cell_t cvarId = params[1];
	if (cvarId < 0)
	{
		ctx->ReportError("Invalid cvar pointer!");
		return 0;
	}
	const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
	auto cvar = cvarMngr->findCvar(cvarId);
	if (!cvar)
	{
		ctx->ReportError("Cvar not found!");
		return 0;
	}
	
	return sp_ftoc(cvar->asFloat());
}

static cell_t core_cvarGetString(SourcePawn::IPluginContext *ctx,
                                 const cell_t *params)
{
    char *destBuffer;
    size_t bufferSize = params[3];

	cell_t cvarId = params[1];
	if (cvarId  < 0)
	{
		ctx->ReportError("Invalid cvar pointer!");
		return 0;
	}
	const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
	auto cvar = cvarMngr->findCvar(cvarId);
	if (!cvar)
	{
		ctx->ReportError("Cvar not found!");
		return 0;
	}

    ctx->LocalToString(params[2], &destBuffer);
	
#if defined __STDC_LIB_EXT1__ || defined SP_MSVC
    #if defined SP_MSVC
    strncpy_s(destBuffer, bufferSize, cvar->asString().c_str(), _TRUNCATE);
    #else
    strncpy_s(destBuffer, bufferSize, cvar->asString().c_str(), bufferSize - 1);
    #endif
#else
    std::strncpy(destBuffer, cvar->asString().c_str(), bufferSize);
    destBuffer[bufferSize - 1] = '\0';
#endif

    return 1;
}

static cell_t core_cvarGetInt(SourcePawn::IPluginContext *ctx,
                              const cell_t *params)
{
	cell_t cvarId = params[1];
	if (cvarId < 0)
	{
		ctx->ReportError("Invalid cvar pointer!");
		return 0;
	}
	const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
	auto cvar = cvarMngr->findCvar(cvarId);
	if (!cvar)
	{
		ctx->ReportError("Cvar not found!");
		return 0;
	}

	return cvar->asInt();
}

static cell_t core_cvarGetFlags(SourcePawn::IPluginContext *ctx,
                                const cell_t *params)
{
	cell_t cvarId = params[1];
	if (cvarId  < 0)
	{
		ctx->ReportError("Invalid cvar pointer!");
		return 0;
	}
	const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
	auto cvar = cvarMngr->findCvar(cvarId);
	if (!cvar)
	{
		ctx->ReportError("Cvar not found!");
		return 0;
	}

	return cvar->getFlags();
}

static cell_t core_cvarSetFloat(SourcePawn::IPluginContext *ctx,
                                const cell_t *params)
{
	cell_t cvarId = params[1];
	if (cvarId < 0)
	{
		ctx->ReportError("Invalid cvar pointer!");
		return 0;
	}
	const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
	auto cvar = cvarMngr->findCvar(cvarId);
	if (!cvar)
	{
		ctx->ReportError("Cvar not found!");
		return 0;
	}
	cvar->setValue(sp_ctof(params[2]));

	return 1;
}

// native void cvarSetString(const char[] name, const char[] value)
static cell_t core_cvarSetString(SourcePawn::IPluginContext *ctx,
                                 const cell_t *params)
{
    char *cvarValue;
    ctx->LocalToString(params[2], &cvarValue);
	
	cell_t cvarId = params[1];
	if (cvarId < 0)
	{
		ctx->ReportError("Invalid cvar pointer!");
		return 0;
	}
	const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
	auto cvar = cvarMngr->findCvar(cvarId);
	if (!cvar)
	{
		ctx->ReportError("Cvar not found!");
		return 0;
	}
	cvar->setValue(cvarValue);

    return 1;
}

static cell_t core_cvarSetInt(SourcePawn::IPluginContext *ctx,
                              const cell_t *params)
{
	cell_t cvarId = params[1];
	if (cvarId  < 0)
	{
		ctx->ReportError("Invalid cvar pointer!");
		return 0;
	}
	const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
	auto cvar = cvarMngr->findCvar(cvarId);
	if (!cvar)
	{
		ctx->ReportError("Cvar not found!");
		return 0;
	}
	cvar->setValue(params[2]);

	return 1;
}

static cell_t core_cvarAddCallback(SourcePawn::IPluginContext *ctx,
	const cell_t *params)
{
	cell_t cvarId = params[1];
	if (cvarId  < 0)
	{
		ctx->ReportError("Invalid cvar pointer!");
		return 0;
	}
	const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
	auto cvar = cvarMngr->findCvar(cvarId);
	if (!cvar)
	{
		ctx->ReportError("Cvar not found!");
		return 0;
	}
	auto ptr = ctx->GetFunctionById(params[2]);
	if (ptr)
	{
		cvar->addPluginCallback(ptr);
	}

	return 1;
}

static cell_t core_cvarSetFlags(SourcePawn::IPluginContext *ctx,
                                const cell_t *params)
{
	cell_t cvarId = params[1];
	if (cvarId  < 0)
	{
		ctx->ReportError("Invalid cvar pointer!");
		return 0;
	}
	const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
	auto cvar = cvarMngr->findCvar(cvarId);
	if (!cvar)
	{
		ctx->ReportError("Cvar not found!");
		return 0;
	}
	cvar->setFlags((ICvar::Flags)params[2]);

	return 1;
}

static cell_t core_cvarFindCvar(SourcePawn::IPluginContext *ctx,
                                const cell_t *params)
{
	const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
	char *cvarName, *cvarValue;
	ctx->LocalToString(params[1], &cvarName);
	ctx->LocalToString(params[2], &cvarValue);

	auto plCvar = cvarMngr->registerOrFindCvar(cvarName, cvarValue, (ICvar::Flags)params[3], false);

	if (!plCvar)
		return -1;

	return plCvar->getId();
}

sp_nativeinfo_t gCvarsNatives[] =
{
	{ "Cvar.Cvar",				core_cvarRegister },
	{ "Cvar.GetName",			core_cvarGetName },
	{ "Cvar.GetFloat",      	core_cvarGetFloat },
	{ "Cvar.GetString",			core_cvarGetString },
	{ "Cvar.GetInt",			core_cvarGetInt },
	{ "Cvar.GetFlags",			core_cvarGetFlags },
	{ "Cvar.SetFloat",			core_cvarSetFloat },
	{ "Cvar.SetString",     	core_cvarSetString },
	{ "Cvar.SetInt",			core_cvarSetInt },
	{ "Cvar.SetFlags",			core_cvarSetFlags },
	{ "Cvar.AddHookOnChange",	core_cvarAddCallback },
	{ "FindCvar",           	core_cvarFindCvar },
	{ nullptr, nullptr }
};
