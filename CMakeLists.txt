cmake_minimum_required(VERSION 3.5)
project(VideoListApp)

# Find required Qt packages
#set(CMAKE_PREFIX_PATH "/path/to/Qt/5.x.x")  # Replace with your Qt installation directory
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(Qt5WinExtras)

set(Qt5_USE_STATIC_LIBS ON)
set(Qt5_USE_STATIC_RUNTIME ON)

# Set the source files for the project
set(SOURCES
    main.cpp
)

# Set the header files for the project
set(HEADERS
    videolistapp.h
)

# Add executable target
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE include/)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON)


# Link against Qt libraries
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::WinExtras)
