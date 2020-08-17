/*
	binary protection
	mohammad hossein asghari
*/
#define _GNU_SOURCE
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <sys/prctl.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <linux/elf.h>
#include <enLoader.h>

int main(int argc, char** argv)
{
	if(argc < 5)
	{
		puts("[-] please type arguments:\n\t./enLoader <-e (encrypte) |-d (decrypte) > <program_file>\n");
		exit(0);
	}

	int file_size = get_file_size(argv[1]);
	char *program_dec = (char *) calloc(file_size, sizeof(char));
	char *key = (char *) calloc(256, sizeof(char));
	get_decrypte_key(key, argv[2], argv[3]);
	get_decrypte_file(program_dec, argv[1], file_size, key);
	
	int fd = memfd_create("", MFD_CLOEXEC);
	char *program_exec;
	write(fd, program_dec, file_size);
	asprintf(&program_exec, "/proc/self/fd/%i", fd);
	execl(program_exec, "arg1", "arg2", "arg3", NULL);
	puts("[-] error while executing decrypted program");
	return 0;
}
