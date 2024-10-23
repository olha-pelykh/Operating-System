#define _POSIX_SOURCE
#include<sys/types.h>
#include<unistd.h>
#include<iostream>
#include <stdio.h>
int main(int argc, char* argv[])
{
fork();
sleep(3);
std::cout<<"child"<<std::endl;
sleep(3);
std::cout<<"parent"<<std::endl;
return 0;
}
