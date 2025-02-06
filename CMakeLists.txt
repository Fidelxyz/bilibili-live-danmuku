cmake_minimum_required(VERSION 3.16)
project(bilibili-live-danmuku
        VERSION 0.1.1
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

# Compiler options
if (MSVC)
    add_compile_options(
            /W4
            $<$<CONFIG:DEBUG>:/fsanitize=address>)
    add_link_options(
            /W4
            $<$<CONFIG:DEBUG>:/fsanitize=address>)
else ()
    add_compile_options(
            -Wall -Wextra
            $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>
            $<$<CONFIG:DEBUG>:-fsanitize-address-use-after-scope>
    )
    add_link_options(
            -Wall -Wextra
            $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>
            $<$<CONFIG:DEBUG>:-fsanitize-address-use-after-scope>
    )
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(QT_NO_DEBUG_OUTPUT)
endif ()

# Find Qt libraries
find_package(Qt6 REQUIRED COMPONENTS Core Gui Network WebSockets Widgets LinguistTools)
qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(unofficial-brotli CONFIG REQUIRED)

# Source files
set(SOURCES
        src/modules/danmu_display/danmu_config.cpp
        src/modules/danmu_display/danmu_display.cpp
        src/modules/danmu_display/danmu_loader.cpp
        src/modules/danmu_display/danmu_panel.cpp
        src/modules/danmu_display/danmu_window.cpp
        src/modules/live_room/live_room.cpp
        src/modules/live_room/protocol.cpp
        src/utils/byte_convert.cpp
        src/utils/decompress.cpp
        src/utils/network.cpp
        src/danmuku.cpp
        src/main.cpp
        src/module.cpp
)

# Header files
set(HEADERS
        src/modules/danmu_display/danmu_config.h
        src/modules/danmu_display/danmu_display.h
        src/modules/danmu_display/danmu_loader.h
        src/modules/danmu_display/danmu_panel.h
        src/modules/danmu_display/danmu_window.h
        src/modules/live_room/live_room.h
        src/modules/live_room/protocol.h
        src/utils/byte_convert.h
        src/utils/decompress.h
        src/utils/network.h
        src/danmuku.h
        src/global.h
        src/module.h
)

# UI files
set(UI_FILES
        src/modules/danmu_display/ui/danmu_panel.ui
        src/modules/danmu_display/ui/danmu_window.ui
)

# Resource files
set(RESOURCES
)

# Translation files
set(TRANSLATIONS
        i18n/zh_CN.ts
)

# Add executable
qt_add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UI_FILES} ${RESOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Network
        Qt6::WebSockets
        Qt6::Widgets
        unofficial::brotli::brotlidec
)

if (MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
            LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif ()

# Add translations
qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES zh_CN)
qt_add_translations(${PROJECT_NAME})

# Install rules
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> DESTINATION bin)
install(PROGRAMS "$<TARGET_FILE:Qt6::QWindowsIntegrationPlugin>" DESTINATION bin/platforms)
install(PROGRAMS "$<TARGET_FILE:Qt6::QModernWindowsStylePlugin>" DESTINATION bin/styles)
