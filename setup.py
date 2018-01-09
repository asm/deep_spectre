from distutils.core import setup, Extension

module1 = Extension('spectre_tf',
                    sources = ['spectre_tf.c'])

setup (name = 'Spectre Tensorflow',
       version = '1.0',
       description = '',
       ext_modules = [module1])
