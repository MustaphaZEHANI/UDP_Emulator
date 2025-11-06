
This Folder Contain :

-Emulator_UDP source file : emulator.c  emulator_parser.c  emulator_parser.h
-Client source file   : client.c
-Client nc command : nc -u localhost 2020 
-Log file : Emulator_Test.Log (contained Emulator_UDP traces )

We are using port "2020" to ensure connection between client and server.

------------------
How to Compile
------------------
- To compile the Emulator you can use this command :

gcc emulator.c emulator_parser.c -o UDP

- To compile the Client Emulator :

gcc client.c -o client

---------------------------
How to Activate Debug Mode
---------------------------
To activate Debug trace we need to Set  "1" to this macro "debug"

Emulator_UDP: [emulator_parser.h]   #define debug   1

Client :      [client.c]            #define debug 1






