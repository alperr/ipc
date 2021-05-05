#include <stdint.h>

void ipc_send(uint8_t* message , uint32_t size);
void ipc_listen(int32_t argc, int8_t* argv[], void (*onmsg)(uint8_t* msg, uint32_t size));
