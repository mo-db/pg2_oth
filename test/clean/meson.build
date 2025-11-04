project('embed_lua', 'c', 'cpp',
  default_options : ['c_std=c11', 'cpp_std=c++23', 'buildtype=debug'])

inc = include_directories('.', 'luajit_fin/include')

# luaJIT
cc = meson.get_compiler('c')
luajit_lib = cc.find_library(
  'luajit-5.1.2',
  dirs : ['luajit_fin/lib']
)

luajit_dep = declare_dependency(
  dependencies : [luajit_lib],
  # include_directories : include_directories('luajit_fin/include/luajit-2.1/')
  include_directories : inc
)

sources = ['test.cpp']

executable('app', sources, include_directories : inc, 
          dependencies : [luajit_dep])
