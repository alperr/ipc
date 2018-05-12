#include <stdint.h>

void nodejs_send(uint8_t* message , uint32_t message_size);
void nodejs_listen(void (*onmessage)(uint8_t *message, uint32_t message_size, int32_t writer));
