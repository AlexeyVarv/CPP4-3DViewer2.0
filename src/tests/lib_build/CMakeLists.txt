# Минимальная версия CMake
cmake_minimum_required(VERSION 3.14)

# Название проекта
project(3DViewerBackend LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Поиск пакета Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick3D 3DCore 3DRender 3DExtras)

# Создание статической библиотеки для бэкенда
add_library(3DViewerBackend STATIC
    ../../3DViewer/core/model3d.h
    ../../3DViewer/adapter/modelloader.h
    ../../3DViewer/adapter/geometryadditions.cpp
    ../../3DViewer/adapter/geometryprototype.h
    ../../3DViewer/adapter/geometryprototype.cpp
    ../../3DViewer/adapter/linesgeometry.h
    ../../3DViewer/adapter/linesgeometry.cpp
    ../../3DViewer/adapter/fasade.h
    ../../3DViewer/adapter/fasade.cpp
    ../../3DViewer/io/objloader.h
)

# Подключение заголовочных файлов для бэкенда
target_include_directories(3DViewerBackend PUBLIC
    ../../3DViewer/core
    ../../3DViewer/adapter
    ../../3DViewer/io
)

# Подключение библиотек Qt для бэкенда
target_link_libraries(3DViewerBackend PRIVATE
    Qt6::Core
    Qt6::Qml
    Qt6::Quick3D
    Qt6::3DCore
    Qt6::3DRender
    Qt6::3DExtras
)


# # Минимальная версия CMake
# cmake_minimum_required(VERSION 3.14)

# # Название проекта
# project(3DViewerBackend LANGUAGES CXX)

# # Поиск пакета Qt6
# find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick3D 3DCore 3DRender 3DExtras)

# # Создание статической библиотеки для бэкенда
# add_library(3DViewerBackend STATIC
#     ../../3DViewer/core/model3d.h
#     ../../3DViewer/adapter/modelloader.h
#     ../../3DViewer/adapter/geometryadditions.cpp
#     ../../3DViewer/adapter/geometryprototype.h
#     ../../3DViewer/adapter/geometryprototype.cpp
#     ../../3DViewer/adapter/linesgeometry.h
#     ../../3DViewer/adapter/linesgeometry.cpp
#     ../../3DViewer/adapter/fasade.h
#     ../../3DViewer/adapter/fasade.cpp
#     ../../3DViewer/io/objloader.h
# )

# # Подключение заголовочных файлов для бэкенда
# target_include_directories(3DViewerBackend PUBLIC
#     ${CMAKE_CURRENT_SOURCE_DIR}/../../3DViewer/core
#     ${CMAKE_CURRENT_SOURCE_DIR}/../../3DViewer/adapter
#     ${CMAKE_CURRENT_SOURCE_DIR}/../../3DViewer/io
# )

# # Подключение библиотек Qt для бэкенда
# target_link_libraries(3DViewerBackend PRIVATE
#     Qt6::Core
#     Qt6::Qml
#     Qt6::Quick3D
#     Qt6::3DCore
#     Qt6::3DRender
#     Qt6::3DExtras
# )
