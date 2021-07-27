/*
 * network client
 *
 * @build:  make examples
 * @server  bin/httpd -s restart -d
 * @usage:  bin/nc 127.0.0.1 8080
 *          > GET / HTTP/1.1
 *          > Connection: close
 *          > [Enter]
 *          > GET / HTTP/1.1
 *          > Connection: keep-alive
 *          > [Enter]
 */

#include "hloop.h"
#include "hbase.h"
#include "hsocket.h"
#include "hlog.h"

#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>


#define RECV_BUFSIZE    8192
static char recvbuf[RECV_BUFSIZE];

//缓冲区大小
//#define MAX_PATH 260

// 1:tcp 2:udp
int protocol = 1;
// for stdin
hio_t*      stdinio = NULL;
// for socket
hio_t*      sockio = NULL;

int verbose = 0;

void send_heartbeat(hio_t* io) {
    static char buf[] = "PING\r\n";
    // printf("send_heartbeat %s", buf);
    hio_write(io, buf, 6);
}

void on_recv(hio_t* io, void* buf, int readbytes) {
    //printf("on_recv fd=%d readbytes=%d\n", hio_fd(io), readbytes);
    if (verbose) {
        char localaddrstr[SOCKADDR_STRLEN] = {0};
        char peeraddrstr[SOCKADDR_STRLEN] = {0};
        printf("[%s] <=> [%s]\n",
            SOCKADDR_STR(hio_localaddr(io), localaddrstr),
            SOCKADDR_STR(hio_peeraddr(io), peeraddrstr));
    }
    printf("%.*s", readbytes, (char*)buf);

    // test hio_set_readbuf in hread_cb
#if 0
    static int total_readbytes = 0;
    total_readbytes += readbytes;
    if (total_readbytes >= RECV_BUFSIZE) {
        total_readbytes = 0;
    }
    hio_set_readbuf(io, recvbuf + total_readbytes, RECV_BUFSIZE - total_readbytes);
    printf("%.*s", total_readbytes, recvbuf);
#endif

    fflush(stdout);
}

void on_stdin(hio_t* io, void* buf, int readbytes) {
    //printf("on_stdin fd=%d readbytes=%d\n", hio_fd(io), readbytes);
    //printf("> %s\n", buf);

    char* str = (char*)buf;

    // test hio_read_start/hio_read_stop/hio_close/hloop_stop
#if 1
    if (strncmp(str, "start", 5) == 0) {
        printf("call hio_read_start\n");
        hio_read_start(sockio);
        return;
    }
    else if (strncmp(str, "stop", 4) == 0) {
        printf("call hio_read_stop\n");
        hio_read_stop(sockio);
        return;
    }
    else if (strncmp(str, "close", 5) == 0) {
        printf("call hio_close\n");
        hio_close(sockio);
        return;
    }
    else if (strncmp(str, "quit", 4) == 0) {
        printf("call hloop_stop\n");
        hloop_stop(hevent_loop(io));
        return;
    }
#endif

    // CR|LF => CRLF for test most protocols
    char eol = str[readbytes-1];
    if (eol == '\n' || eol == '\r') {
        if (readbytes > 1 && str[readbytes-2] == '\r' && eol == '\n') {
            // have been CRLF
        }
        else {
            ++readbytes;
            str[readbytes - 2] = '\r';
            str[readbytes - 1] = '\n';
        }
    }

    hio_write(sockio, buf, readbytes);
}

void on_close(hio_t* io) {
    //printf("on_close fd=%d error=%d\n", hio_fd(io), hio_error(io));
    hio_del(stdinio, HV_READ);
}

void on_connect(hio_t* io) {
    //printf("on_connect fd=%d\n", hio_fd(io));
    if (verbose) {
        char localaddrstr[SOCKADDR_STRLEN] = {0};
        char peeraddrstr[SOCKADDR_STRLEN] = {0};
        printf("connect connfd=%d [%s] => [%s]\n", hio_fd(io),
            SOCKADDR_STR(hio_localaddr(io), localaddrstr),
            SOCKADDR_STR(hio_peeraddr(io), peeraddrstr));
    }

    hio_read_start(io);
    // uncomment to test heartbeat
    // hio_set_heartbeat(sockio, 3000, send_heartbeat);
}

//建立目录
int create_dir(char *ptr)
{
	char *filename;
	char dir[MAX_PATH] = {0};
	filename = ptr;
	
	//打印当前工作目录
	getcwd(dir， sizeof(dir));
	printf("current working dirctory is : %s\n",dir);
	
	//在当前路径建立目录
	if(mkdir(filename, 0777) < 0)
	{
		printf("mkdir fail!\n");
		return errno;
	}
	else
	{
		printf("mkdir success!\n");
	}
	
	return 0;
}

//获取当前工作目录
char * get_run_dir(char *buf, int size)
{
	return getcwd(buf, size);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("\
Usage: nc [-ut] host port\n\
Options:\n\
  -t        Use tcp protocol (default)\n\
  -u        Use udp protocol\n\
Examples: nc 127.0.0.1 80\n\
          nc -u 127.0.0.1 80\n");
        return -10;
    }

    int index = 1;
    const char* protocolname;
    if (argv[1][0] == '-') {
        ++index;
        if (argv[1][1] == 't') {
            protocol = 1;
            protocolname = "tcp";
        }
        else if (argv[1][1] == 'u') {
            protocol = 2;
            protocolname = "udp";
        }
    }
    const char* host = argv[index++];
    int port = atoi(argv[index++]);
    if (verbose) {
        printf("%s %s %d\n", protocolname, host, port);
    }

    HV_MEMCHECK;

    hloop_t* loop = hloop_new(HLOOP_FLAG_QUIT_WHEN_NO_ACTIVE_EVENTS);

    // stdin use default readbuf
    stdinio = hread(loop, 0, NULL, 0, on_stdin);
    if (stdinio == NULL) {
        return -20;
    }

    // socket
    if (protocol == 1) {
        // tcp
        sockio = hloop_create_tcp_client(loop, host, port, on_connect);
    }
    else if (protocol == 2) {
        // udp
        sockio = hloop_create_udp_client(loop, host, port);
        hio_read(sockio);
    }
    if (sockio == NULL) {
		char run_dir[MAX_PATH] = {0};
		get_run_dir(run_dir, sizeof(run_dir));
		char logdir[MAX_PATH] = {0};
		snprintf(logdir, sizeof(logdir), "%s/logs", run_dir);
		
		create_dir(logdir);
		char logfile[MAX_PATH] = {0};
		char logname[MAX_PATH] = "nc_fail_record";
		snprintf(logfile, sizeof(logfile), "%s/logs/%s.log", run_dir, logname);
		
		//配置错误日志
		hlog_set_level(LOG_LEVEL_ERROR);
		hlog_set_remain_days(3);
		hlog_set_file(logfile);
		//logger_set_level(hlog, LOG_LEVEL_ERROR );
		//logger_set_remain_days(hlog, 3);
		//logger_set_file(hlog, "nc_fail_record");
		
		//打印错误日志
        hloge("Client failed to connect to server!");
		
		getchar();
        return -20;
    }
    // printf("sockfd=%d\n", hio_fd(sockio));
    hio_setcb_close(sockio, on_close);
    hio_setcb_read(sockio, on_recv);
    hio_set_readbuf(sockio, recvbuf, RECV_BUFSIZE);

    hloop_run(loop);
    hloop_free(&loop);

    return 0;
}
