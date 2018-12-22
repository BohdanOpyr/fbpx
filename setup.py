from distutils.core import setup, Extension
setup(name='fbpx', version='1.0',  \
      ext_modules=[Extension('fbpx', ['fbpx.c'])])
