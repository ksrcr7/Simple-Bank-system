cmake_minimum_required(VERSION 3.27)
project(Bank_account)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Bank_account
        test/main.cpp
        src/Account.cpp
        src/Person.cpp

)


target_include_directories(Bank_account
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
