cmake_minimum_required(VERSION 3.22)
project(FlightReservation CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 REQUIRED COMPONENTS Widgets)


file(GLOB CORE_SRC
"src/core/*.cpp"
)
file(GLOB UI_SRC
"src/ui/**/*.cpp"
)


add_executable(frs
app/main.cpp
${CORE_SRC}
${UI_SRC}
)


target_include_directories(frs PRIVATE include)


target_link_libraries(frs PRIVATE Qt6::Widgets)


# Set working directory to project root so data/ is discoverable during run from IDEs
set_target_properties(frs PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")