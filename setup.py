from distutils.core import setup, Extension

module1 = Extension('deep_spectre',
                    sources = ['deep_spectre.c'])

setup (name = 'Deep Spectre',
       version = '1.0',
       description = 'Deep learning side channel privileged memory reader',
       ext_modules = [module1])
