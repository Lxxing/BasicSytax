#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>


#define DEFAULT_PORT 7000
#define DEFAULT_BACKLOG 128


uv_loop_t *loop;
struct sockaddr_in addr;

uv_tcp_t *tcp_client;

typedef struct {
	uv_write_t req;
	uv_buf_t buf;
} write_req_t;

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

void echo_write(uv_write_t *req, int status) {
	if (status) {
		fprintf(stderr, "Write error %s\n", uv_strerror(status));
	}
	free(req);
}

void echo_read(uv_stream_t *ser, ssize_t nread, const uv_buf_t *buf) {
	if (nread > 0) {
		printf("cli %d:%s \n",nread,buf->base);

		char * send = (char*)malloc(255);
		write_req_t *req = (write_req_t*)malloc(sizeof(write_req_t));


		memset(send,0,sizeof(255));
		memset(req,0,sizeof(write_req_t));

		*(int*)(send) = 99;
		*(int*)(send+4) = 255 - 4 - 4;
		memcpy(send+8,"testlibuv",sizeof("testlibuv"));
		
		req->buf = uv_buf_init(send, 255);
		
		uv_write((uv_write_t*)req, ser, &req->buf, 1, echo_write);
		Sleep(3000);
		return;
	}
	if (nread < 0) {
		if (nread != UV_EOF)
			fprintf(stderr, "Read error %s\n", uv_err_name(nread));
	}

	free(buf->base);
}
void Muv_write_cb(uv_write_t *req, int status)
{
	if (status == -1)
	{
		return;
	}

	req->handle->data = req->data;

	uv_read_start(req->handle, alloc_buffer, echo_read);

}

void CTest(uv_connect_t* req, int status)
{
	printf("CTest");

	uv_buf_t buf = uv_buf_init("HELLO", 6);

	uv_write_t write_req;
	//write_req.data = req->data;
	int buf_count = 1;
	uv_write(&write_req, tcp_client, &buf, buf_count, Muv_write_cb);

}

int main() {
	
	void *buf = NULL;
	unsigned int len;

	loop = uv_default_loop();

	tcp_client  = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));

	uv_tcp_init(loop, tcp_client);

	uv_connect_t* connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));

	struct sockaddr_in dest;
	uv_ip4_addr("127.0.0.1", 7000, &dest);

	uv_tcp_connect(connect, tcp_client, (const struct sockaddr*)&dest, CTest);

	uv_run(loop, UV_RUN_DEFAULT);
	return 0;
}
