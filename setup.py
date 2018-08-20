# -*- encoding: utf-8 -*-

try:
    from setuptools import setup, Extension
except ImportError:
    from distutils.core import setup, Extension

setup(
    name='pymodinv',
    version='0.2.1',
    description='Fast modular inverse for python integers',
    author=u'Erik Aronesty',
    author_email='erik@q32.com',
    url='https://github.com/earonesty/pymodinv',
    license='MIT',
    ext_modules=[Extension('pymodinv', ['pymodinv.cpp'])],
)
