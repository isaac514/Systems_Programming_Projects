Isaac Cooper
04/11/2024
CS_3411
Project_5

1. Individual programs complied successfully with gcc -Wall -pedantic -g -o step(n) step(n).c and prior to with submission with (make all)

    Notes: 

2. Step1.c 
    Step1.c works correctly the socket is set up and is allowed to assign a random port 
    the port number is then outputted to stdout

3. Step2.c
    Step2.c works correctly a client can successfully connects to the server with the use of telnet.
    Upon connection telnet reports "Connected to 127.0.0.1 (hostname)" and the server prints to stdout,
    "Accepted Connection From: (s_in.sin_addr.s_addr , s_in->sin_port)". When the server is terminated telnet
    reports "Connection closed by foreign host".


4. Step3.c 
    Step3.c works correctly. Select() implemented such that any input written to stdin is reflected back to stdout 
    and when there is input on the client side such as an attempted connection the program accepts the client with a 
    call to accepted() and prints the client information. 

5. Step4.c
    Step4.c compelete. added an integer array to store the status of each client connected to the server 
    0 => Not Connected 1 => Connected 2 => Listing 


6. Step5.c
    Step5.c complete. (log #:) is displayed to the connected client. The connection status vector, file position vector and client file descriptor
    vector are set accordingly. Note that this step is only set up to work on one client. 

7. Step6.c
    Step6.c complete. Upon reading (0) from the connected client the status vector is set accordingly and the (TEST client closed) is written to stdout
    Note that select no longer monitors stdin because it is used in further steps. Note that the read() system call hangs on connected client with no input
    however because this step only monitors one file descriptor this lack of functionally is ignored. To account for select hanging in later steps the connected clients
    will be added to the readset and monitored for input to prevent read() from hanging. 


8. Step7.c
    Step7.c complete. The server can connected to mulitple clients. Major changes to this step from the previous step include the following: 
    Prior to the call to select the file descriptors for the connected clients are added to the read set. A function is used to find the largest file 
    descriptor in the client descriptor vector to set up the select call properly. All vectors are updated such that if a new client connects to the server
    we will search through the status array to find the first available index for insertion. Indexes are keep consistant for all vectors and their corresponding descriptor.
    All client connections are monitored and upon reading 0 their status is updated to not connected and (TEST client disconnect) is written to stdout. Note that
    read no longer hangs. 


9. Step8.c
    Step8.c complete. The server can read the commands list, log, and \n. When iterating over the connected clients the program checks for both
    connected clients and listing clients. Additionally line (100) has been updated to account for the listing clients remaining in the read set. For all commands
    if the user input (\n) the program will recognized this as a invalid command unless it proceeds and list command. 


10. Step9.c
    Step9.c complete. The log command has been implemenented and tested with mulitple clients, server is able to read up 124 bytes of data from multiple clients
    the (log) command is written to the server logfile. The log file is opened once in towards the beginning of the program so that O_TRUNC does not deleted any 
    contents of the file, except when it is run at first. 

11. logger.c
    logger.c complete. The list command has been implemenented so that file contents can be listed back to the client until there is no more data to be written. The file position
    vector is updated accordingly so that when the user hits enter and the client is in the listing state then next chunk of data can be written. Upon intial call to (list) the currency indictor 
    for the logfile is reset, which is nessary as after calls to (log) it will be at the end of the file. In a similar manner, if a user attempts to log more data after calls to list the currency
    indictor must be moved to the the end of the logfile. Program has been tested with populating the logfile with more than 128 bytes.  

