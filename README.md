# Snake.c
A implementation of snake written purely in C with Raylib. Written as a synthesis of a few weeks independantly learning C.

## Tech Stack
- nob.h
- raylib

## Learning Process
This was my first project in C where I did everything on my own merits. I made design and tool choices on my own, and wrote every line of code myself. Previous projects written in C that I did followed mostly some sort of guide or other resource, but this was my first go at something entirely my own. Its not an original idea for a project, but the design is entirely mine.

This project was also a way of teaching myself how working with external libraries works in C. I have two types of libraries in this project: a traditional large library in Raylib, and a header-file library in nob. This was my way of learning how libraries in C and C++ work, as I'm
mostly used to working with things like Rust which have built in package managers. 

Overall this project was actually pretty smooth sailing. I think the biggest hiccup I ran into was how to efficiently model the snake's body as it grew, and I achieved that by modeling the body like a deque, with the deque having it's own collision separate from the snake's head. How I decided to make the snake move and everything also lead into the snake just naturally wrapping around on the screen edges without any need to adjust or bug fix. My design choices just worked out when I moved from a single moving block to a full body.

Going forward I might use this as a platform to develop machine learning programs to play the game optimally, since I know how this game works on the back end and I can get a machine learning algo to hook directly into it instead of having to read whats on screen and making decisions like that. 

## Build Process
- clone the repository
- clone raylib repository into /raylib/
- build raylib as defined in raylib wiki.
- bootstrap nob with `gcc -o nob nob.c`
- in repo root, run `./nob` to build
- run with `./snake`
