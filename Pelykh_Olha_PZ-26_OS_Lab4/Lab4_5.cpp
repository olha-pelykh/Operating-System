#define _POSIX_SOURCE
#include<sys/types.h>
#include<unistd.h>
#include<iostream>
#include <stdio.h>
#include<sys/wait.h>
int main(int argc, char* argv[])
{
pid_t p, w;
int status;
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
do
{
w=wait(&status);
if (w==-1)
{
perror("wait");
exit(EXIT_FAILURE);
}
if (WIFEXITED(status))
{
std::cout<<"parent say fork return p="<<p<<std::endl;
std::cout<<"exited "<<WEXITSTATUS(status)<<std::endl;
}
else if (WIFSIGNALED(status))
{
std::cout<<"killed by "<<WTERMSIG(status)<<std::endl;
}
else if (WIFSTOPPED (status))
{
std::cout<<"stopped by
"<<WSTOPSIG(status)<<std::endl;
}
else if (WIFCONTINUED(status))
{
std::cout<<"continued "<<std::endl;
}
} while (!WEXITSTATUS(status)&& !WIFSIGNALED(status));
}
exit(EXIT_SUCCESS);
}
