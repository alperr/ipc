#include <stdio.h>
#include "nodejs_bridge.h"

void onmessage(uint8_t *message, uint32_t message_size)
{
	printf("nodejs has sent a message with size %d\n", message_size);
	//char buff[] = {65, 66, 67, 68}
	//nodejs_send(buf, 4);
}

int main()
{
	nodejs_listen(onmessage); // this call starts a blocking event loop
	return 0;
}