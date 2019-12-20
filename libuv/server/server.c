#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>
#include <queue.h>

#define DEFAULT_PORT 7000
#define DEFAULT_BACKLOG 128

uv_loop_t *loop;
struct sockaddr_in addr;

struct user_s {
	int age;
	char* name;
	QUEUE node;
};

typedef struct {
	uv_write_t req;
	uv_buf_t buf;
	char * context;
	int len;
	int testnum;
	size_t text_len;
	int nread;
} write_req_t;

void free_write_req(uv_write_t *req) {
	write_req_t *wr = (write_req_t*)req;
	//free(wr->buf.base);
	free(wr);
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) 
{
	write_req_t *mydata = (write_req_t *)handle->data;
	printf("alloc_buffer testnum:%d \n",mydata->testnum);
	if (0 == mydata->testnum)
	{
		mydata->context = malloc(6);
		mydata->text_len = 6;
		*buf = uv_buf_init(mydata->context,mydata->text_len);
		return;
	}
	else if (88 == mydata->testnum)
	{
		mydata->context = &mydata->text_len;
		mydata->text_len = sizeof(int);
		*buf = uv_buf_init(mydata->context,mydata->text_len);
		return;
	}
	else if (89 == mydata->testnum)
	{
		mydata->context = &mydata->text_len;
		mydata->text_len = sizeof(int);
		*buf = uv_buf_init(mydata->context, mydata->text_len);
		return;
	}
	else if (90 == mydata->testnum)
	{
		mydata->context = malloc(mydata->text_len);
		*buf = uv_buf_init(mydata->context, mydata->text_len);
		return;
	}
		

	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

void on_close(uv_handle_t* handle) {
	free(handle);
}

void echo_write(uv_write_t *req, int status) {
	if (status) {
		fprintf(stderr, "Write error %s\n", uv_strerror(status));
	}
	free_write_req(req);
}

void echo_read(uv_stream_t *client, ssize_t nread, uv_buf_t *buf) 
{
	write_req_t *mydata = (write_req_t *)client->data;
	printf("echo_read testnum:%d \n",mydata->testnum);

	if (nread > 0) {
		printf("read %d:%s \n",nread,buf->base);
		mydata->nread += nread;
		if (mydata->testnum == 0)
		{
			//read hello
			free(mydata->context);
			mydata->testnum = 88;
			mydata->context = NULL;
			mydata->nread = 0;

		}
		else if(mydata->testnum == 88)
		{
			//read version
			int version = *(int*)(mydata->context);
			printf("version:%d \n",version);
			mydata->testnum = 89;
			mydata->context = NULL;
			mydata->nread = 0;
		}else if(mydata->testnum == 89)
		{
			//read len
			mydata->text_len = *(int*)mydata->context;
			printf("text_len = %d \n",mydata->text_len);
			mydata->testnum = 90;
			mydata->context = NULL;
			mydata->nread = 0;
			*buf = uv_buf_init(mydata->context,mydata->text_len);
			return;
		}
		else if(mydata->testnum == 90)
		{
			//read body
			printf("body data:%s \n", mydata->context);
			mydata->testnum = 88;
			mydata->nread = 0;
			*buf = uv_buf_init(mydata->context,mydata->text_len);
			return;
		}
		write_req_t *req = (write_req_t*)malloc(sizeof(write_req_t));
		req->buf = uv_buf_init("Word", 5);
		uv_write((uv_write_t*)req, client, &req->buf, 1, echo_write);
		return;
	}
	if (nread < 0) {
		if (nread != UV_EOF)
			fprintf(stderr, "Read error %s\n", uv_err_name(nread));
		uv_close((uv_handle_t*)client, on_close);
	}
	if (nread == 0) {
		printf("read zero.");
	}
}

void on_new_connection(uv_stream_t *server, int status) {
	if (status < 0) {
		fprintf(stderr, "New connection error %s\n", uv_strerror(status));
		return;
	}

	write_req_t *mydata = (write_req_t*)malloc(sizeof(write_req_t));

	uv_tcp_t *client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));

	memset(mydata,0,sizeof(write_req_t));
	memset(client,0,sizeof(uv_tcp_t));

	mydata->testnum = 0;
	client->data = mydata;
	
	
	uv_tcp_init(loop, client);
	
	if (uv_accept(server, (uv_stream_t*)client) == 0) {
		uv_read_start((uv_stream_t*)client, alloc_buffer, echo_read);
	}
	else {
		uv_close((uv_handle_t*)client, on_close);
	}
}


int main() {

	static QUEUE* q;
	static QUEUE queue;

	struct user_s* user;
	struct user_s john;
	struct user_s henry;
	struct user_s *willy = malloc(sizeof(struct user_s));

	john.name = "john";
	john.age = 44;
	henry.name = "henry";
	henry.age = 32;
	willy->name = "lx";
	willy->age = 30;

	QUEUE_INIT(&queue);
	QUEUE_INIT(&john.node);
	QUEUE_INIT(&henry.node);
	QUEUE_INIT(&willy->node);

	QUEUE_INSERT_TAIL(&queue, &john.node);
	QUEUE_INSERT_TAIL(&queue, &henry.node);
	QUEUE_INSERT_TAIL(&queue, &willy->node);

	q = QUEUE_HEAD(&queue);

	user = QUEUE_DATA(q, struct user_s, node);

	printf("Received first inserted user: %s who is %d.\n",
		user->name, user->age);

	QUEUE_REMOVE(q);

	QUEUE_FOREACH(q, &queue) {
		user = QUEUE_DATA(q, struct user_s, node);

		printf("Received rest inserted users: %s who is %d.\n",
			user->name, user->age);
	}
	
	loop = uv_default_loop();

	uv_tcp_t server;
	uv_tcp_init(loop, &server);

	uv_ip4_addr("127.0.0.1", DEFAULT_PORT, &addr);

	uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
	int r = uv_listen((uv_stream_t*)&server, DEFAULT_BACKLOG, on_new_connection);
	if (r) {
		fprintf(stderr, "Listen error %s\n", uv_strerror(r));
		return 1;
	}
	return uv_run(loop, UV_RUN_DEFAULT);
}
