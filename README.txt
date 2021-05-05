ipc                              nodejs <-> C interprocess communication library
--------------------------------------------------------------------------------

This library enables your nodejs programs to exchange messages with 
C programs by using unix sockets.

In order to use this library, you should first compile your C program and create
an executable, then by using the file path of that executable, nodejs program
can spawn that process and connect to it like this

	IPC = require("ipc.js")
	IPC.connect("path-of-your-c-executable")

messages can be sent from nodejs to C

	IPC.send("sample message")

messages that sent from C can be consumed like this

	IPC.onmessage = function(msg){ console.log(msg) }

main function of your C program should call **ipc_listen** to start a blocking 
event loop that listens the messages coming across the unix socket, 
you must also pass the argc and argv parameters of main function to ipc_listen 
alongside with a function pointer **onmsg** that handles the messages sent from
nodejs

	int32_t main(int32_t argc, int8_t* argv[])
	{
		ipc_listen(argc, argv, onmsg);
		return 0;
	}

message handler takes 2 self-explanatory parameters **msg** and **size**
you can also use the function **ipc_send** to send messages from C to nodejs

	void onmsg(uint8_t* msg, uint32_t size)
	{
		printf("received a message with size %d\n", size);
		ipc_send({65, 66, 67, 68}, 4);
	}



To sum up this library has 3 library files and an one example folder

1. ipc.c	C ipc library implementation 
2. ipc.h	C ipc library header file
3. ipc.js	nodejs module for ipc library

