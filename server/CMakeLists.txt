CMAKE_MINIMUM_REQUIRED (VERSION 2.8.10)

# The library has to be dynamically linked to the header
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/includes)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/gdbus)
INCLUDE_DIRECTORIES(/usr/include/glib-2.0)
INCLUDE_DIRECTORIES(/usr/lib/glib-2.0/include)
INCLUDE_DIRECTORIES(/usr/include/gio-unix-2.0)
INCLUDE_DIRECTORIES(/usr/lib/x86_64-linux-gnu/glib-2.0/include/)

add_executable(server server.c ${PROJECT_SOURCE_DIR}/gdbus/gdbusdemo_gen.c)
target_link_libraries(server  pthread gio-2.0 gobject-2.0 gmodule-2.0 gthread-2.0 rt glib-2.0)
