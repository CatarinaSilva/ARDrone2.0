/*
    video.c - video driver

    Copyright (C) 2011 Hugo Perquin - http://blog.perquin.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA 02110-1301 USA.
*/

#ifndef MATLAB_MEX_FILE

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h> 
#include <fcntl.h> 
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h> 
#include <linux/videodev2.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <signal.h>

#endif //MATLAB_MEX_FILE

#include "tcpVideoLinuxClientMultiThread.h"

#ifndef MATLAB_MEX_FILE

#define CLEAR(x) memset (&(x), 0, sizeof (x))
#define DEFAULT_BUFLEN 131072

#endif //MATLAB_MEX_FILE





/*************************************
		Auxiliar functions
*************************************/

#define DEFAULT_BUFLEN 131072
int connect_TCPclient(char * ip ,long port)
{						
#ifndef MATLAB_MEX_FILE
  void (*old_handler)(int);
  int n; 
  char *ptr, buffer[1024]; 

  struct sockaddr_in addr; 

	if((old_handler=signal(SIGPIPE,SIG_IGN))==SIG_ERR){ 
		printf("signal error\n");
		exit(1);
	} 
  int fd = socket(AF_INET,SOCK_STREAM,0);
  //fcntl(fd, F_SETFL, O_NONBLOCK);

  if(fd == -1){
	  printf("socket returned -1\n");
	  exit(1);
  } 
  memset((void*)&addr,(int)'\0',sizeof(addr));

  addr.sin_family=AF_INET;
  
  inet_aton(ip, &addr.sin_addr);

  addr.sin_port=htons(port);
  int err;
  int sndsize = DEFAULT_BUFLEN; 
  int sockbufsize = 0; int size = sizeof(int); 
  err = getsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char *)&sockbufsize, &size);
  //printf("Buffer size before setsockopt %d\n", sockbufsize);
  
  int iSock = 1;
  setsockopt( fd, IPPROTO_TCP, TCP_QUICKACK, (void *)&iSock, sizeof(iSock));
  
  err = setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char *)&sndsize, (int)sizeof(sndsize)); 
  
  err = setsockopt(fd, IPPROTO_TCP, TCP_MAXSEG, (char *)&sndsize, (int)sizeof(sndsize)); 
  
  n=connect(fd,(struct sockaddr*)&addr,sizeof(addr));
  if(n==-1) return -1;
  
  sockbufsize = 0; 
  err = getsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char *)&sockbufsize, &size);
 // printf("buffer%d\n", sockbufsize);
  
  sockbufsize = 0; 
  err = getsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char *)&sockbufsize, &size);
  //printf("Buffer size after connect %d\n", sockbufsize); 
  
  return fd;
  #endif //MATLAB_MEX_FILE
}



int TCP_send(int fd, char * message, int length){
	#ifndef MATLAB_MEX_FILE

	char * ptr = message;
	int nleft,nwritten, n;
	nwritten = 0;
	nleft=length; 
	while(nleft>0){
		if(nleft < 8192) n=write(fd, ptr, nleft); 
		else n=send(fd, ptr, 8192, 0); 
		if(n==-1){
			return -1;
		}//if(n<=0)exit(1);//error 
		nleft-=n; 
		ptr+=n;
		nwritten += n;
	 } 
	 return nwritten;
	#endif //MATLAB_MEX_FILE

}

	
#ifndef MATLAB_MEX_FILE
//return timestamp in seconds with microsecond resolution
double util_timestamp()
{
  struct timeval tv;
  gettimeofday(&tv, NULL); 
  return (double)tv.tv_sec+((double)tv.tv_usec)/1000000;
}

#endif //MATLAB_MEX_FILE








/*************************************
				VIDEO 1
*************************************/

int cycleFlag = 0;

void video1InitGrabSend(int * workPort, int port, int num_buffers, int runInitCycle)
{
#ifndef MATLAB_MEX_FILE
	
	workPort[0] = port;

	vid1.device = (char*)"/dev/video1";
	vid1.w=1280;
	vid1.h=720;
	vid1.n_buffers = num_buffers;
	
	img1 = (img_struct*)malloc(sizeof(img_struct));
	img1->w=vid1.w;
	img1->h=vid1.h;
	img1->buf = (unsigned char*)malloc(vid1.h*vid1.w*2);
		
    struct v4l2_capability cap;
    struct v4l2_format fmt;
    unsigned int i;
    enum v4l2_buf_type type;

    extern int errno;

	vid1.seq=0;
	vid1.trigger=0;
	if(vid1.n_buffers==0) vid1.n_buffers=num_buffers;
	
	vid1.fd = open(vid1.device, O_RDWR | O_NONBLOCK, 0);

    if (ioctl(vid1.fd, VIDIOC_QUERYCAP, &cap) < 0) {
		printf("ioctl() VIDIOC_QUERYCAP failed.\n");
    }

//     printf("2 driver = %s, card = %s, version = %d, capabilities = 0x%x\n", cap.driver, cap.card, cap.version, cap.capabilities);

    CLEAR(fmt);
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = vid1.w;	
	fmt.fmt.pix.height = vid1.h;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
    if (ioctl(vid1.fd, VIDIOC_S_FMT, &fmt) < 0) {
		printf("ioctl() VIDIOC_S_FMT failed.\n");
		exit(EXIT_FAILURE);
    }

    struct v4l2_requestbuffers req;

    CLEAR(req);
    req.count = vid1.n_buffers;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
	
    if (ioctl(vid1.fd, VIDIOC_REQBUFS, &req) < 0) {
		// Catch errors slovak194
		printf("ioctl() VIDIOC_REQBUFS failed.\n");
		exit(EXIT_FAILURE);
    }

    vid1.buffers = (buffer_struct*)calloc(vid1.n_buffers, sizeof(buffer_struct));

    for (i = 0; i < vid1.n_buffers; ++i) {
		struct v4l2_buffer buf;

		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		if (ioctl(vid1.fd, VIDIOC_QUERYBUF, &buf) < 0) {
			printf("ioctl() VIDIOC_QUERYBUF failed.\n");
		}

		vid1.buffers[i].length = buf.length;
		printf("buffer%d.length=%d\n",i,buf.length);
		vid1.buffers[i].buf = mmap(NULL, buf.length, PROT_READ|PROT_WRITE, MAP_SHARED, vid1.fd, buf.m.offset);

		if (MAP_FAILED == vid1.buffers[i].buf) {
			printf ("mmap() failed.\n");
		}
	}

	for (i = 0; i < vid1.n_buffers; ++i) {
		struct v4l2_buffer buf;

		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		if (ioctl(vid1.fd, VIDIOC_QBUF, &buf) < 0) {
			printf("errno = %d\n", errno);
			printf("ioctl() VIDIOC_QBUF failed.\n");
			exit(EXIT_FAILURE); 
		}
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(vid1.fd, VIDIOC_STREAMON, &type)< 0) {
		printf("ioctl() VIDIOC_STREAMON failed.\n");  
    }
	if(runInitCycle) cycleFlag = 0;
	else cycleFlag = 1;
	
#endif //MATLAB_MEX_FILE
}

#ifndef MATLAB_MEX_FILE
void video_GrabImage1() 
{
	fd_set fds;
	struct timeval tv;
	int r;

	FD_ZERO(&fds);
	FD_SET(vid1.fd, &fds);

	tv.tv_sec = 2;
	tv.tv_usec = 0;
	r = select(vid1.fd + 1, &fds, NULL, NULL, &tv);

	if (-1 == r) {
		if (EINTR == errno)
		printf("select err\n");
	}

	if (0 == r) {
		fprintf(stderr, "select timeout\n");
		exit(EXIT_FAILURE);
	}
	
	struct v4l2_buffer buf;
	CLEAR(buf);
	
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	
	if (ioctl(vid1.fd, VIDIOC_DQBUF, &buf) < 0) {

		printf("ioctl() VIDIOC_DQBUF failed.\n");
		exit(EXIT_FAILURE);
	}

	assert(buf.index < vid1.n_buffers);

	vid1.seq++;	
	vid1.img = img1;
	vid1.img->timestamp = util_timestamp();
	vid1.img->seq = vid1.seq;
 	
	memcpy(vid1.img->buf, vid1.buffers[buf.index].buf, vid1.w*vid1.h*2);
	
	if (ioctl(vid1.fd, VIDIOC_QBUF, &buf) < 0) {
			printf("ioctl() VIDIOC_QBUF failed.\n");
	}
}
#endif //MATLAB_MEX_FILE


#ifndef MATLAB_MEX_FILE
void video_GrabImage1_initCycle() 
{
	fd_set fds;
	struct timeval tv;
	int r;
	int count=0;

	FD_ZERO(&fds);
	FD_SET(vid1.fd, &fds);
	tv.tv_sec = 2;
	tv.tv_usec = 0;
	
	while(count<200){
		
		r = select(vid1.fd + 1, &fds, NULL, NULL, &tv);

		if (-1 == r) {
			break;
		}
		if (0 == r) {
			break;
		}
		
		struct v4l2_buffer buf;
		CLEAR(buf);
		
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		
		if (ioctl(vid1.fd, VIDIOC_DQBUF, &buf) < 0) {

			printf("ioctl() VIDIOC_DQBUF failed.\n");
			exit(EXIT_FAILURE);
		}

		assert(buf.index < vid1.n_buffers);

		vid1.seq++;	
		vid1.img = img1;
		vid1.img->timestamp = util_timestamp();
		vid1.img->seq = vid1.seq;
		
		memcpy(vid1.img->buf, vid1.buffers[buf.index].buf, vid1.w*vid1.h*2);
		
		if (ioctl(vid1.fd, VIDIOC_QBUF, &buf) < 0) {
				printf("ioctl() VIDIOC_QBUF failed.\n");
		}
		
		tv.tv_sec = 0;
		tv.tv_usec = 100;
		count++;
	}
	
}
#endif //MATLAB_MEX_FILE


void video1CloseGrabSend(void)
{
#ifndef MATLAB_MEX_FILE

	int i;
    for (i = 0; i < vid1.n_buffers; ++i) {
    	if (-1 == munmap(vid1.buffers[i].buf, vid1.buffers[i].length)) printf("munmap() failed.\n");
    }
    close(vid1.fd); 
	
#endif //MATLAB_MEX_FILE
}


double video1GrabSend(int * workPort)
{	
#ifndef MATLAB_MEX_FILE
	double nbytes = 0;
	
	if( cycleFlag == 0 ){
		video_GrabImage1_initCycle();
		cycleFlag == 1;
	}else{
		video_GrabImage1();
	}
		
	int fd = connect_TCPclient("192.168.1.2", workPort[0]);
	if(fd!=-1){
		double nbytes = TCP_send(fd,(char *) vid1.img->buf, 1843200);
		close(fd);
	} 
	
	return nbytes;

#endif //MATLAB_MEX_FILE
}
	
	


	
	
	
	
	
	
	
	
/*************************************
				VIDEO 2
*************************************/

int cycleFlag2 = 0;

void video2InitGrabSend(int * workPort, int port, int num_buffers, int runInitCycle)
{
#ifndef MATLAB_MEX_FILE
	
	workPort[0] = port;
	
	vid2.device = (char*)"/dev/video2";
	vid2.w=320;
	vid2.h=240;
	vid2.n_buffers = num_buffers;
	
	img2 = (img_struct*)malloc(sizeof(img_struct));
	img2->w=vid2.w;
	img2->h=vid2.h;
	img2->buf = (unsigned char*)malloc(vid2.h*vid2.w*2);
		
    struct v4l2_capability cap;
    struct v4l2_format fmt;
    unsigned int i;
    enum v4l2_buf_type type;

    extern int errno;

	vid2.seq=0;
	vid2.trigger=0;
	if(vid2.n_buffers==0) vid2.n_buffers=num_buffers;
	
	vid2.fd = open(vid2.device, O_RDWR | O_NONBLOCK, 0);

    if (ioctl(vid2.fd, VIDIOC_QUERYCAP, &cap) < 0) {
		printf("ioctl() VIDIOC_QUERYCAP failed.\n");
    }

//     printf("2 driver = %s, card = %s, version = %d, capabilities = 0x%x\n", cap.driver, cap.card, cap.version, cap.capabilities);

    CLEAR(fmt);
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = vid2.w;	
	fmt.fmt.pix.height = vid2.h;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
    if (ioctl(vid2.fd, VIDIOC_S_FMT, &fmt) < 0) {
		printf("ioctl() VIDIOC_S_FMT failed.\n");
		exit(EXIT_FAILURE);
    }

    struct v4l2_requestbuffers req;

    CLEAR(req);
    req.count = vid2.n_buffers;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
	
    if (ioctl(vid2.fd, VIDIOC_REQBUFS, &req) < 0) {
		// Catch errors slovak194
		printf("ioctl() VIDIOC_REQBUFS failed.\n");
		exit(EXIT_FAILURE);
    }

    vid2.buffers = (buffer_struct*)calloc(vid2.n_buffers, sizeof(buffer_struct));

    for (i = 0; i < vid2.n_buffers; ++i) {
		struct v4l2_buffer buf;

		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		if (ioctl(vid2.fd, VIDIOC_QUERYBUF, &buf) < 0) {
			printf("ioctl() VIDIOC_QUERYBUF failed.\n");
		}

		vid2.buffers[i].length = buf.length;
		printf("buffer%d.length=%d\n",i,buf.length);
		vid2.buffers[i].buf = mmap(NULL, buf.length, PROT_READ|PROT_WRITE, MAP_SHARED, vid2.fd, buf.m.offset);

		if (MAP_FAILED == vid2.buffers[i].buf) {
			printf ("mmap() failed.\n");
		}
	}

	for (i = 0; i < vid2.n_buffers; ++i) {
		struct v4l2_buffer buf;

		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		if (ioctl(vid2.fd, VIDIOC_QBUF, &buf) < 0) {
			printf("errno = %d\n", errno);
			printf("ioctl() VIDIOC_QBUF failed.\n");
			exit(EXIT_FAILURE); 
		}
    }

	
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(vid2.fd, VIDIOC_STREAMON, &type)< 0) {
		printf("ioctl() VIDIOC_STREAMON failed.\n");  
    }
	
	if(runInitCycle) cycleFlag2 = 0;
	else cycleFlag2 = 1;

#endif //MATLAB_MEX_FILE
}

#ifndef MATLAB_MEX_FILE
void video_GrabImage2() 
{	
	fd_set fds;
	struct timeval tv;
	int r;

	FD_ZERO(&fds);
	FD_SET(vid2.fd, &fds);

	tv.tv_sec = 2;
	tv.tv_usec = 0;
	r = select(vid2.fd + 1, &fds, NULL, NULL, &tv);

	if (-1 == r) {
		if (EINTR == errno)
		printf("select err\n");
	}

	if (0 == r) {
		fprintf(stderr, "select timeout\n");
		exit(EXIT_FAILURE);
	}
	
	struct v4l2_buffer buf;
	CLEAR(buf);
	
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	
	if (ioctl(vid2.fd, VIDIOC_DQBUF, &buf) < 0) {

		printf("ioctl() VIDIOC_DQBUF failed.\n");
		exit(EXIT_FAILURE);
	}

	assert(buf.index < vid2.n_buffers);

	vid2.seq++;	
	vid2.img = img2;
	vid2.img->timestamp = util_timestamp();
	vid2.img->seq = vid2.seq;
 	
	memcpy(vid2.img->buf, vid2.buffers[buf.index].buf, vid2.w*vid2.h*2);
	
	if (ioctl(vid2.fd, VIDIOC_QBUF, &buf) < 0) {
			printf("ioctl() VIDIOC_QBUF failed.\n");
	}
}
#endif //MATLAB_MEX_FILE



#ifndef MATLAB_MEX_FILE
void video_GrabImage2_initCycle() 
{
	fd_set fds;
	struct timeval tv;
	int r;
	int count=0;

	FD_ZERO(&fds);
	FD_SET(vid2.fd, &fds);
	tv.tv_sec = 2;
	tv.tv_usec = 0;
	
	while(count<200){
		
		r = select(vid2.fd + 1, &fds, NULL, NULL, &tv);

		if (-1 == r) {
			break;
		}
		if (0 == r) {
			break;
		}
		
		struct v4l2_buffer buf;
		CLEAR(buf);
		
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		
		if (ioctl(vid2.fd, VIDIOC_DQBUF, &buf) < 0) {

			printf("ioctl() VIDIOC_DQBUF failed.\n");
			exit(EXIT_FAILURE);
		}

		assert(buf.index < vid2.n_buffers);
		
		vid2.seq++;	
		vid2.img = img2;
		vid2.img->timestamp = util_timestamp();
		vid2.img->seq = vid2.seq;
	
		memcpy(vid2.img->buf, vid2.buffers[buf.index].buf, vid2.w*vid2.h*2);
		
		if (ioctl(vid2.fd, VIDIOC_QBUF, &buf) < 0) {
				printf("ioctl() VIDIOC_QBUF failed.\n");
		}
		
		tv.tv_sec = 0;
		tv.tv_usec = 100;
		count++;
	}
	
}
#endif //MATLAB_MEX_FILE



void video2CloseGrabSend(void)
{
#ifndef MATLAB_MEX_FILE

	int i;
    for (i = 0; i < vid2.n_buffers; ++i) {
    	if (-1 == munmap(vid2.buffers[i].buf, vid2.buffers[i].length)) printf("munmap() failed.\n");
    }
    close(vid2.fd);
	
	
#endif //MATLAB_MEX_FILE
}


double video2GrabSend(int * workPort)
{	
#ifndef MATLAB_MEX_FILE
	double nbytes = 0;
	
	if( cycleFlag2 == 0 ){
		video_GrabImage2_initCycle();
		cycleFlag2 == 1;
	}else{
		video_GrabImage2();
	}

	int fd = connect_TCPclient("192.168.1.2", workPort[0]);
	if(fd!=-1){
		double nbytes = TCP_send(fd,(char *) vid2.img->buf, 153600);
		close(fd);
	} 
	
	return nbytes;

#endif //MATLAB_MEX_FILE
}
	
	





