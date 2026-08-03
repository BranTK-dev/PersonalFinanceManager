cmake_minimum_required(VERSION 3.16)
project(PersonalFinanceManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/Transaction.cpp
    src/FinanceManager.cpp
    src/TransactionDialog.cpp
)

set(HEADERS
    include/MainWindow.h
    include/Transaction.h
    include/FinanceManager.h
    include/TransactionDialog.h
)

add_executable(PersonalFinanceManager
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(PersonalFinanceManager PRIVATE include)

target_link_libraries(PersonalFinanceManager PRIVATE Qt6::Widgets)
