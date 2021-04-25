cmake_minimum_required(VERSION 3.5)

project(SimpleAssembler LANGUAGES C)

add_executable(sat main.c stringop.c stringop.h command.c command.h)
