/*  PyMod - Python Half-Life scripting engine
 *  Copyright (C) 2018  PyMod Development Team
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

#include <pluginMngr.hpp>

Plugin::Plugin(size_t id, const fs::path &path)
{
    std::fstream scriptFile(path, std::ios_base::in | std::ios_base::binary);
    auto fileName = path.stem().c_str();
    std::stringstream errorMsg;

    if (scriptFile.fail())
    {
        errorMsg << "[PyMod] Can't open " << path;
        throw std::runtime_error(errorMsg.str());
    }

    scriptFile.seekg(0, std::ios_base::end);
    auto size = scriptFile.tellg();
    scriptFile.seekg(0, std::ios_base::beg);

    auto dataBlock = std::make_unique<char[]>(size + 1LL);
    scriptFile.read(dataBlock.get(), size);

    PyObject *compiledScript = Py_CompileString(dataBlock.get(), fileName, Py_file_input);
    if (!compiledScript) {
        errorMsg << "[PyMod] Error compiling " << path;
        throw std::runtime_error(errorMsg.str());
    }

    m_internal = PyImport_ExecCodeModule(fileName, compiledScript);
    if (!m_internal) {
        errorMsg << "[PyMod] Can't import " << path;
        throw std::runtime_error(errorMsg.str());
    }
    Py_DECREF(compiledScript);

    GetPluginInfo(path.filename().string());

    m_id = id;
}

Plugin::~Plugin()
{
    if (Py_REFCNT(m_internal) > 1)
        Py_DECREF(m_internal);
}

void Plugin::GetPluginInfo(const std::string &scriptname)
{
    std::stringstream errorMsg;

    PyObject *pluginInfoClass = PyObject_GetAttrString(m_internal, "pluginInfo");
    if (!pluginInfoClass)
    {
        errorMsg << "[PyMod] Can't find pluginInfo class! (" << scriptname << ")";
        throw std::runtime_error(errorMsg.str());
    }

    auto infoObject = PyObject_CallObject(pluginInfoClass, nullptr);
    Py_DECREF(pluginInfoClass);

    auto gatherInfo = [&errorMsg, &scriptname, &infoObject](const char *property)
    {
        PyObject *propertyObject = PyObject_GetAttrString(infoObject, property);
        if (!propertyObject) {
            errorMsg << "[PyMod] Cannot access " << property << " property (" << scriptname << ")";
            throw std::runtime_error(errorMsg.str());
        }
        auto &&propertyValue = PyUnicode_AsUTF8(propertyObject);
        Py_DECREF(propertyObject);
        return propertyValue;
    };

    m_name = gatherInfo("name");
    m_version = gatherInfo("version");
    m_author = gatherInfo("author");
    m_url = gatherInfo("url");
    m_filename = std::move(scriptname);

    Py_DECREF(infoObject);
}

PluginMngr::PluginMngr(const fs::path &pathToScripts)
{
    m_scriptsPath = std::move(pathToScripts);
    loadPlugins();
}

size_t PluginMngr::loadPlugins()
{
    size_t index = 0;
    std::error_code errCode;
    auto directoryIter = fs::directory_iterator(m_scriptsPath, errCode);

    if (errCode)
    {
        std::stringstream msg;
        msg << "Error while loading plugins " << errCode.message() << '\n';
        SERVER_PRINT(msg.str().c_str());

        return index;
    }

    for (const auto &entry : directoryIter)
    {
        auto &&filePath = entry.path();
        if (filePath.extension().string() != ".py")
        {
            std::stringstream msg;
            msg << "[PyMod] Unrecognized file format: " << filePath << '\n';
            SERVER_PRINT(msg.str().c_str());

            continue;
        }
        std::shared_ptr<Plugin> plugin;
        try
        {
            plugin = std::make_shared<Plugin>(index, filePath);
        }
        catch (const std::exception &e)
        {
            std::stringstream msg;
            msg << "[PyMod] " << e.what() << '\n';
            SERVER_PRINT(msg.str().c_str());
            
            continue;
        }
        m_plugins.insert_or_assign(index, std::move(plugin));
        index++;
    }
    return index;
}
