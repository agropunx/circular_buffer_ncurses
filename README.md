# C++ bidirectional Circular Buffer class 

Repository with a bidirectional circular buffer class written in C++;

It is also given a simple ncurses TUI for the circular buffer.
It compile and works but yet some flexibility
for buffer type (as per class) and read/write from/to external files need to be added.
Currently the buffer type in TUI.cpp is fixed to uint32_t, 
tedious but easy to change accessing the code. 

The repo contains the following files:
 
class header:
__cirBuff.h__

class test routine:
__test.cpp__
run with: g++ test.cpp -o test

simple TUI(ncurses) implementing cirBuff.h :
__TUI.cpp__
run with: g++ TUI.cpp -lmenu -lform -lncurses -o TUI

Created by giacomo on 5 sept 2020
