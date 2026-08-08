cmake_minimum_required(VERSION 3.16)
project(PersonalFinanceManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/Transaction.cpp
    src/FinanceManager.cpp
    src/TransactionDialog.cpp
    src/FileManager.cpp
    src/StatisticsDialog.cpp
)

set(HEADERS
    include/MainWindow.h
    include/Transaction.h
    include/FinanceManager.h
    include/TransactionDialog.h
    include/FileManager.h
    include/StatisticsDialog.h
)

set(RESOURCES
    resources/resources.qrc
)

add_executable(PersonalFinanceManager
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

# The .rc file sets the icon on the .exe itself and in the Windows taskbar.
# Only added on Windows, other platforms don't use .rc files.
if(WIN32)
    target_sources(PersonalFinanceManager PRIVATE resources/app_icon.rc)
endif()

target_include_directories(PersonalFinanceManager PRIVATE include)

target_link_libraries(PersonalFinanceManager PRIVATE Qt6::Widgets Qt6::Charts)
