#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
	int cpid = fork();
	if(!cpid){
	//inside child
		exec("./bench.o");
	}
	else if(cpid != 0){
	// inside parent
		int fd = open(
	}
	return 0;
}
