# Snake.c
A implementation of snake written purely in C with Raylib. Written as a synthesis of a few weeks independantly learning C.

## Tech Stack
- nob.h
- raylib

## Learning Process
This was my first project in C where I did everything on my own merits. I made design and tool choices on my own, and wrote every line of code myself. Previous projects written in C that I did followed mostly some sort of guide or other resource, but this was my first go at something entirely my own. Its not an original idea for a project, but the design is entirely mine.

This project was also a way of teaching myself how working with external libraries works in C. I have two types of libraries in this project: a traditional large library in Raylib, and a header-file library in nob. This was my way of learning how libraries in C and C++ work, as im
mostly used to working with things like Rust which have built in package managers.

## Build Process
- clone the repository
- clone raylib repository into /raylib/
- build raylib as defined in raylib wiki.
- bootstrap nob with 'gcc -o nob nob.c'
- in repo root, run './nob' to build
- run with './snake'
