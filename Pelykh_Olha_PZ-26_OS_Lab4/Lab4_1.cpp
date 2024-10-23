#define _POSIX_SOURCE
#include<sys/types.h>
#include<unistd.h>
#include<iostream>
#include <stdio.h>
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
std::cout<<"child says fork return p="<<p<<std::endl;
exit(0);
}
else
{
std::cout<<"parent says fork return p="<<p<<std::endl;
}
return 0;
}
