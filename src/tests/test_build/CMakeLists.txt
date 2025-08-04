# Минимальная версия CMake
cmake_minimum_required(VERSION 3.14)

# Название проекта
project(3DViewerTests LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Включение флагов для gcov
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

# Поиск пакета gtest
find_package(GTest REQUIRED)

# В test_build/CMakeLists.txt
add_subdirectory(../lib_build ${CMAKE_BINARY_DIR}/lib_build)

# Включение поддержки gcov
# if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
#     set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
# endif()

# Поиск пакета Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick3D 3DCore 3DRender 3DExtras)

# Добавление исполняемого файла для тестов
add_executable(3DViewerTests ../test.cpp)

# Подключение заголовочных файлов для тестов
target_include_directories(3DViewerTests PRIVATE
    ../../3DViewer/core
    ../../3DViewer/adapter
    ../../3DViewer/io
)

# Указываем путь к библиотеке 3DViewerBackend
link_directories(${CMAKE_SOURCE_DIR}/tests/lib_build)

# Подключение библиотек для тестов
target_link_libraries(3DViewerTests PRIVATE
    GTest::GTest
    GTest::Main
    3DViewerBackend
    Qt6::Core
    Qt6::Qml
    Qt6::Quick3D
    Qt6::3DCore
    Qt6::3DRender
    Qt6::3DExtras
)
