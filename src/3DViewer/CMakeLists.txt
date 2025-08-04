# Минимальная версия CMake
cmake_minimum_required(VERSION 3.14)

# Название проекта
project(3DViewer LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_PREFIX_PATH "/home/mintaarc/qt-everywhere-src-6.8.2/qtbase")

# Поиск пакета Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets 3DCore 3DRender 3DExtras Quick Quick3D)

# Добавление исполняемого файла
add_executable(3DViewer
    main.cpp
    core/model3d.h
    adapter/modelloader.h
    adapter/geometryadditions.cpp
    adapter/geometryprototype.h
    adapter/geometryprototype.cpp
    adapter/linesgeometry.h
    adapter/linesgeometry.cpp
    adapter/fasade.h
    adapter/fasade.cpp
    adapter/saver.h
    adapter/saver.cpp
    adapter/viewersettings.h
    io/objloader.h
)

#qt_add_resources(${PROJECT_NAME} "resources" PREFIX "/" FILES main.qml)
qt_add_resources(${PROJECT_NAME} "resources"
    PREFIX "/"
    FILES
        main.qml
        shaders/dashed_shader.frag
        shaders/vertex_shader.frag
)

# Подключение заголовочных файлов
target_include_directories(3DViewer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/adapter
    ${CMAKE_CURRENT_SOURCE_DIR}/io
)

# Подключение библиотек Qt
target_link_libraries(3DViewer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::3DCore
    Qt6::3DRender
    Qt6::3DExtras
    Qt6::Quick
    Qt6::Quick3D
)

# Автоматическая генерация moc-файлов (если используются сигналы и слоты)
set_target_properties(3DViewer PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
)