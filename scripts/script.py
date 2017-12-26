import core

class pluginInfo(object):
    def __init__(self):
        self.name = 'Test plugin'
        self.version = '1.0.0'
        self.author = 'PyMod Dev Team'
        self.url = 'https://github.com/Amaroq7/PyMod'

def pluginInit():
    core.printToConsole('I am alive!')
