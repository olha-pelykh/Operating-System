#define _POSIX_SOURCE
#include<sys/types.h>
#include<unistd.h>
#include<iostream>
#include <stdio.h>
#include<sys/wait.h>

int main(int argc, char* argv[])
{
int n;
std::cout<<"enter number of process"<<std::endl;
std::cin>>n;
pid_t *p =new pid_t[n];
int *status=new int[n];
pid_t *w =new pid_t[n];
p[0]=fork();
if(p[0]<0)
{
perror("error fork");
exit(EXIT_FAILURE);
}
if(p[0]>0)
{
std::cout<<"parent before"<<std::endl;
do{
if ((w[0]=waitpid(p[0],&status[0],WNOHANG))==0)
{
std::cout<<"child is still running"<<std::endl;
}
else
{
if(WIFEXITED(status[0]))
std::cout<<"child exited with status"<<
WEXITSTATUS(status[0])<<std::endl;
}
} while(w[0]==0);
std::cout<<"parent after"<<std::endl;
}
if (p[0]==0)
{
std::cout<<"child "<<std::endl;
for(long i=0, j=0; i<1000000; i++)
for(long k=0;k<1000;k++)
j+=i;
}
return 0;
}
