#define _POSIX_SOURCE
#include<sys/types.h>
#include<unistd.h>
#include<iostream>
#include <stdio.h>
#include<sys/wait.h>
int main(int argc, char* argv[])
{
pid_t p;
p = fork();
if (p<0)
{
perror("fork");
return EXIT_FAILURE;
}
else if (p==0)
{
std::cout<<"child say fork return p="<<p<<std::endl;
exit(EXIT_SUCCESS);
}
else
{
wait(NULL);
std::cout<<"parent say fork return p="<<p<<std::endl;
}
exit(EXIT_SUCCESS);
}
