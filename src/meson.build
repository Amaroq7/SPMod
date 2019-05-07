add_project_arguments('-DSPMOD_CORE', language : [ 'c', 'cpp' ])

if build_machine.system() == 'linux'
    add_project_link_arguments('-ldl', language : [ 'c', 'cpp'])
endif

sourceFiles = files('h_export.cpp',
                    'engine_api.cpp',
                    'dllapi.cpp',
                    'meta_api.cpp',
                    'SPGlobal.cpp',
                    'RehldsApi.cpp',
                    'CmdSystem.cpp',
                    'ForwardSystem.cpp',
                    'PlayerSystem.cpp',
                    'LoggingSystem.cpp',
                    'CvarSystem.cpp',
                    'TimerSystem.cpp',
                    'MenuSystem.cpp',
                    'ExtensionSystem.cpp',
                    'SrvCommand.cpp',
                    'ValveInterface.cpp',
                    'UtilsSystem.cpp',
                    'NativeProxy.cpp')

shared_library('spmod_mm', sourceFiles, include_directories : includeDirs)
