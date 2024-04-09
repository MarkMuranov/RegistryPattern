cmake_minimum_required(VERSION 3.28)
project(CommandProcessor)

set(CMAKE_CXX_STANDARD 17)

add_executable(CommandProcessor main.cpp
        ChainOfResponsibilities.h
        Tokenizer.h
        Rick.h
        Handlers.h
)
