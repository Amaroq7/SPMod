/*  SPMod - SourcePawn Scripting Engine for Half-Life
 *  Copyright (C) 2018  SPMod Development Team
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

#include <plugin.hpp>

Plugin::Plugin(size_t id, const std::string &identity, const fs::path &path)
{
    char errorSPMsg[256];
    std::stringstream errorMsg;
    auto *spAPIv2 = gSPGlobal->getSPEnvironment()->APIv2();
    auto *plugin = spAPIv2->LoadBinaryFromFile(path.string().c_str(), errorSPMsg, sizeof(errorSPMsg));

    if (!plugin)
    {
        errorMsg << "Can't load " << path.filename() << "! (" << errorSPMsg << ")";
        throw std::runtime_error(errorMsg.str());
    }

    uint32_t infoVarIndex;
    if (plugin->FindPubvarByName("pluginInfo", &infoVarIndex) != SP_ERROR_NONE)
    {
        errorMsg << "Can't find plugin info! (" << path.filename() << ")";
        throw std::runtime_error(errorMsg.str());
    }

    sp_pubvar_t *infoVar;
    plugin->GetPubvarByIndex(infoVarIndex, &infoVar);

    auto gatherInfo = [plugin, infoVar](uint32_t field)
    {
        char *infoField;
        plugin->GetDefaultContext()->LocalToString(*(infoVar->offs + field), &infoField);
        return infoField;
    };

    m_name = gatherInfo(Plugin::FIELD_NAME);
    m_version = gatherInfo(Plugin::FIELD_VERSION);
    m_author = gatherInfo(Plugin::FIELD_AUTHOR);
    m_url = gatherInfo(Plugin::FIELD_URL);

    m_filename = path.filename();
    m_id = id;
    m_identity = identity;
    m_runtime = plugin;

    auto iNativesNum = plugin->GetNativesNum();
    for (const auto &entry : gSPGlobal->getModulesList())
    {
        if (!entry.second->name || !entry.second->func)
            continue;

        if (!entry.second->name && !entry.second->func)
            break;

        for (uint32_t index = 0; index < iNativesNum; index++)
        {
            const sp_native_t *native = m_runtime->GetNative(index);

            if (native->status == SP_NATIVE_BOUND)
                continue;

            if (std::strcmp(native->name, entry.second->name))
                continue;

            plugin->UpdateNativeBinding(index, entry.second->func, 0, nullptr);
            break;
        }
    }

    cell_t result = 0;
    auto *initFunction = plugin->GetFunctionByName("pluginInit");

    if (initFunction)
        initFunction->Execute(&result);
}

Plugin::~Plugin()
{
    cell_t result;
    auto *endFunction = m_runtime->GetFunctionByName("pluginEnd");

    if (endFunction)
        endFunction->Execute(&result);
}
