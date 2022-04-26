#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <stdlib.h> 
#include <unistd.h>
#include<sys/ioctl.h>
 #include<signal.h>

//struct used by ioctl to pass arguemnts
typedef struct {

        int pid;

}pinfo;

#define CLEAR_DIRTY _IOW ('p',1,pinfo *)
#define TRACK_DIRTY _IOW ('p',2,pinfo *)
#define COW _IOW ('p',3,pinfo *)

pinfo p;

void zz(int pid, int clear) {
	
	p.pid = pid;
	int fd;
	fd = open("/dev/pgdv", O_RDWR);
	//write(fd, &pid, 4);
	if(clear == 1)
	ioctl(fd, CLEAR_DIRTY, &p);
	if(clear == 0)
        ioctl(fd, TRACK_DIRTY, &p);
	if(clear == 2)
	ioctl(fd, COW, &p);	
}
int pid;
void sig_handler(int signum){

  //Return type of the handler function should be void
  printf("\nInside handler function\n");
  zz(pid,1);
  
}

void sig_handler2(int signum){

  //Return type of the handler function should be void
  printf("\nInside handler2 function\n");
  zz(pid,0);
  
}

void sig_handler3(int signum){

  //Return type of the handler function should be void
  printf("\nInside handler3 function\n");
  zz(pid,2);
  
}



int main(){
	
	signal(SIGINT,sig_handler); // Register signal handler	
        signal(SIGUSR1,sig_handler2); // Register signal handler  
	signal(SIGUSR2,sig_handler3); //for cow undoing
	pid = getpid();
	

//	while(1);
	printf("bye\n?\n");



	int a[1024][1024];
	int i, j;

	for(j=0; j<1024; ++j)
		for(i=0; i<1024; ++i)
			a[i][j] = 2*i;
	raise(SIGINT);

        for(j=0; j<1024; ++j)
                for(i=0; i<1024; ++i)
                        a[i][j] = 2*i;

	raise(SIGUSR1);



	return 0;
}
