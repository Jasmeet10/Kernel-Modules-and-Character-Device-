#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include <sys/types.h>

#define BUFFER_LENGTH 20000

int main(){
	int fd;

	fd = open("/dev/process_list", O_RDONLY);      
	if (fd < 0){
		return errno;
	}
	printf("Program executing ... ");
	char user_buffer[BUFFER_LENGTH];
	ssize_t bytes_read = read(fd, user_buffer, BUFFER_LENGTH);
	printf("\n%s\n", user_buffer);	
	close(fd);
	return 0;
}