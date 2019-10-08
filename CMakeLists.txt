set(NAME windows_terminal)

add_library(${NAME} STATIC
    bar.cpp
    bar.hpp
    box.cpp
    box.hpp
    cyclicBuffer.hpp
    label.cpp
    label.hpp
    number.cpp
    number.hpp
    slider.cpp
    slider.hpp
    text.cpp
    text.hpp
    VT100.cpp
    VT100.hpp
    Window.cpp
    Window.hpp
    window_manager.cpp
    window_manager.hpp
)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})