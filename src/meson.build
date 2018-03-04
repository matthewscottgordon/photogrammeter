boost_dep = dependency('boost')
oiio_dep = meson.get_compiler('cpp').find_library('OpenImageIO')
executable('photogrammeter', 'main.cpp', dependencies:[boost_dep, oiio_dep])
