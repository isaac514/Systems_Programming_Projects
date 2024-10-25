Isaac Cooper
04/26/24
CS_3411
Project_4

1. Individual programs complied successfully with gcc -Wall -pedantic -g -o step(n) step(n).c

    Notes: Steps using exec() were tested with a sample program and the unix command ls -la 

2. Step1.c 
Step1.c works correctly, file is opened and remaining arguments are written to the file

3. Step2.c
Step2.c works correctly the program creates a child process and execs the target specified by argv[1]

4. Step3.c 
Step3.c works correctly, the program parses arguments and sets up an argument array. Program successfully forks
into a child processs and runs execve(). Call to execve() works properly with unix programs and user programs
given that user program are passed a full path to the program

5. Step4.c
Step4.c works correctly, the program uses the select function to moinitor the file descriptor for stdin.
If no input is passed into the stdin within the time set by (timeout.tv_sec) the program prints "Tick" otherwise
any input is reflected back out to stdout

6. Step5.c
Step5.c works correctly, the program uses the select function to monitor the file descriptor for stdin and
the named pipe that was created. When data becomes availibe from either stdin or the named pipe it is written to stdout

7. Step6.c
Step6.c works correctly, the program monitors the read ends of the unamed pipes that were created and the outputs of 
stdout and stdin are mapped to the outfile and the error file

8. prose.c
Prose.c works correctly, tested with ./prose test ls -la he program monitors the read ends of the unamed pipes that were created and the outputs of 
stdout and stdin are mapped to the outfile and the error file. As the child process runs the execuable the pipes are monitored for input. On input 
the parent process reads the character data from each pipe and translated it if need be. Output from the executable are written to both the consule 
and the error and output file. 


