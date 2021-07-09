Compilation:
     gcc main.c


Functionality:
1. echo:
   1. Implemented in echo.h
   2. Basic echo function, simply prints given string after removing extra spaces/tabs
2.   cd:
   1. Implemented in cd.h
   2. Basic cd function, with the following flags: '.', '..', '~'
   3. Handles error of invalid directory
3.   pwd:
   1. Implemented in pwd.h
   2. Basic pwd function
4.   quit:
   1. Implemented in exit.h
   2. Exits the shell program
5.   ls:
   1. Implemented in ls.h
   2. Basic ls function, with the following flags: '.', '..', '~', '-a', '-l', ‘-la’, --al’
   3. Handles error of invalid directory
6.   pinfo:
   1. Implemented in pinfo.h
   2. Hardcoded to get the values of state (third in the status file) and memory (first in the statm file)
   3. Handles error of invalid pid
7.   history
   1. Implemented in history.h
   2. When not given a number, it prints the last n<=10 commands
   3. Only 20 commands are stored at any given point in time
8.   setenv
   1. Implemented in setenv.h
   2. Basic setenv function, as specified
9.   unsetenv
   1. Implemented in setenv.h
   2. Basic unsetenv function, as specified

         Side Note: Functions are called in 'readnparse.h'. 'builtinheaders.h' contains all the header files defined by me. ‘main.c’ only calls the looping function defined in ‘readnparse.h’.


        Side Note 2: Background functions can be run using a ‘&’ after non-inbuilt functions. When the background function is terminated, the shell prints ‘%s with pid %d exited normally’ (%s = command name, %d = pid) on the terminal.
