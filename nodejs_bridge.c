#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>

const uint32_t BUFFER_SIZE = 1024;
const char *READER_SOCKET_PATH = "/tmp/nodejs_c_bridge.sock";
const char *WRITER_SOCKET_PATH = "/tmp/c_nodejs_bridge.sock";
uint32_t writer_socket;

int32_t create_writer()
{
	struct sockaddr_un addr;
	int fd, rc;

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		perror("socket error");
		exit(-1);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, WRITER_SOCKET_PATH, sizeof(addr.sun_path) - 1);

	if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		perror("connect error");
		exit(-1);
	}

	printf("c process is connected to nodejs\n");
	return fd;
}

void nodejs_listen(void (*onmessage)(uint8_t *message, uint32_t message_size))
{
	struct sockaddr_un addr;
	uint8_t *buffer = malloc(BUFFER_SIZE);

	int fd, cl, rc;

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		perror("socket error");
		exit(-1);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, READER_SOCKET_PATH, sizeof(addr.sun_path) - 1);
	unlink(READER_SOCKET_PATH);

	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		perror("bind error");
		exit(-1);
	}

	if (listen(fd, 5) == -1)
	{
		perror("listen error");
		exit(-1);
	}

	printf("c process is listening %s \n", READER_SOCKET_PATH);

	writer_socket = create_writer();
	while (1)
	{
		if ((cl = accept(fd, NULL, NULL)) == -1)
		{
			perror("accept error");
			continue;
		}

		while ((rc = read(cl, buffer, BUFFER_SIZE)) > 0)
		{
			onmessage(buffer, rc);
		}

		if (rc == -1)
		{
			perror("read");
			exit(-1);
		}
		else if (rc == 0)
		{
			// printf("EOF\n");
			close(cl);
		}
	}
}

void nodejs_send(uint8_t *message, uint32_t message_size)
{
	write(writer_socket, message, message_size);
}
