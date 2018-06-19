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

static cell_t core_cvarRegister(    SourcePawn::IPluginContext *ctx, 
                                    const cell_t *params)
{	
    enum { cvar_name = 1, cvar_value, cvar_flags };
	const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
	char *cvarName, *cvarValue;
	ctx->LocalToString(params[cvar_name], &cvarName);
	ctx->LocalToString(params[cvar_value], &cvarValue);
	

	auto plCvar = cvarMngr->registerOrFindCvar(cvarName, cvarValue, (ICvar::Flags)params[cvar_flags], true);
	if (!plCvar)
		return -1;
	
	return plCvar->getId();
}

static cell_t core_cvarGetName( SourcePawn::IPluginContext *ctx,
                                const cell_t *params)
{
    enum { cvar_id = 1, buffer, buffer_size };
	char *destBuffer;
	size_t bufferSize = params[buffer_size];

	cell_t cvarId = params[cvar_id];
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

	ctx->LocalToString(params[buffer], &destBuffer);

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
    enum { cvar_id = 1 };
	cell_t cvarId = params[cvar_id];
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
    enum { cvar_id = 1, buffer, buffer_size };
    char *destBuffer;
    size_t bufferSize = params[buffer_size];

	cell_t cvarId = params[cvar_id];
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

    ctx->LocalToString(params[buffer], &destBuffer);
	
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
    enum { cvar_id = 1 };
	cell_t cvarId = params[cvar_id];
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
    enum { cvar_id = 1 };
	cell_t cvarId = params[cvar_id];
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
    enum { cvar_id = 1, cvar_value };
	cell_t cvarId = params[cvar_id];
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
	cvar->setValue(sp_ctof(params[cvar_value]));

	return 1;
}

// native void cvarSetString(const char[] name, const char[] value)
static cell_t core_cvarSetString(SourcePawn::IPluginContext *ctx,
                                 const cell_t *params)
{
    enum { cvar_id = 1, cvar_value };
    char *cvarValue;
    ctx->LocalToString(params[cvar_value], &cvarValue);
	
	cell_t cvarId = params[cvar_id];
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
    enum { cvar_id = 1, cvar_value };
	cell_t cvarId = params[cvar_id];
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
	cvar->setValue(params[cvar_value]);

	return 1;
}

static cell_t core_cvarAddCallback( SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    enum { cvar_id = 1, cvar_callback };
	cell_t cvarId = params[cvar_id];
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
	auto ptr = ctx->GetFunctionById(params[cvar_callback]);
	if (ptr)
	{
		cvar->addPluginCallback(ptr);
	}

	return 1;
}

static cell_t core_cvarSetFlags( SourcePawn::IPluginContext *ctx,
                                 const cell_t *params)
{
    enum { cvar_id = 1, cvar_flags };
	cell_t cvarId = params[cvar_id];
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
	cvar->setFlags((ICvar::Flags)params[cvar_flags]);

	return 1;
}

static cell_t core_cvarFindCvar( SourcePawn::IPluginContext *ctx,
                                 const cell_t *params)
{
    enum { cvar_name = 1 };
	const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
	char *cvarName;
	ctx->LocalToString(params[cvar_name], &cvarName);

	auto plCvar = cvarMngr->registerOrFindCvar(cvarName, "0", (ICvar::Flags)params[3], false);

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
