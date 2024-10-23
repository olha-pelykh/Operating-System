#define _POSIX_SOURCE
#include<sys/types.h>
#include<unistd.h>
#include<iostream>
#include <stdio.h>

int main()
{
while(true)
{
fork();
}
return 0;
}

