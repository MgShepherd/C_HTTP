# C_HTTP

A Custom HTTP Implementation written in C

## Background

This project is built to help me learn the specifics of network programming and also practice some C.
As a result, there may be design decisions in here that would not always be the best approach, but that is just
part of the learning process.

The Aim of this project is to create a library which can be used for other people to create HTTP Servers.

## Structure

There are two main folders of the project.
- lib - This is where the actual implementation for the HTTP library lives
        This is the part you will be interested in if wanting to C_HTTP for your own projects
- sandbox - This is an actual HTTP Server that has been implemented in C_HTTP as part of development
            This can be used as an example to assist with implementing your own projects

## Building the Project from Source

This project uses the make build system (refer to the Makefile at the root of project). To build both the
sandbox and lib, just run `make` from the root of the project.
