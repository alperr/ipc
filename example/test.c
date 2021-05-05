#include <stdio.h>
#include "./ipc.h"

void onmsg(uint8_t* msg, uint32_t size)
{
	printf("nodejs has sent a message with size %d\n", size);
	//char buf[] = {65, 66, 67, 68}
	//ipc_send(buf, 4);
}

int main(int32_t argc, int8_t* argv[])
{
	ipc_listen(argc, argv, onmsg); // starts a blocking event loop
	return 0;
}