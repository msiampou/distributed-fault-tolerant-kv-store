#!/bin/bash
g++ -g3 broker.cpp ../../src/io/parser/parser.cpp ../../src/io/command_handler/command_handler.cpp -o kvBroker