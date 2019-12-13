#include <stdio.h>

#include <iostream>

#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include <sys/types.h>
#include <sys/signalfd.h>

#include "CWindowManager.hpp"



//---------------------------------------------------------------------------------------------------------------
int signal_setup(void)
{
	int fd = -1;
	sigset_t mask;
	
	// signal mask
	::sigemptyset(&mask);
	::sigaddset(&mask, SIGPIPE);
	::sigaddset(&mask, SIGTERM);
	
	::pthread_sigmask(SIG_BLOCK, &mask, NULL);
	
	//setup signal fd
	::sigemptyset(&mask);
	::sigaddset(&mask, SIGTERM);
	
	// create signal fd
	fd = ::signalfd(-1, &mask, SFD_CLOEXEC | SFD_NONBLOCK);
	
	return fd;
}
//---------------------------------------------------------------------------------------------------------------


CWindowManager *g_pwin;
//---------------------------------------------------------------------------------------------------------------
int main (int argc, char *argv[]) 
{
	int sigfd = -1, ret = -1;
	struct pollfd pfds[1];
	struct signalfd_siginfo fdsi;
	
	sigfd = signal_setup();
	
	// setup poll
	::memset(pfds,0,sizeof(pfds));
	pfds[0].fd = sigfd;
	pfds[0].events = POLLIN;
	pfds[0].revents = 0;
	
	g_pwin = new (std::nothrow) CWindowManager();
	
	g_pwin->WindowManagerInitialize();
	
	do
	{
		ret = poll(pfds, sizeof(pfds) / sizeof(struct pollfd), -1);
		
		if (ret == 1)
		{
			ret = read(pfds[0].fd, &fdsi, sizeof(struct signalfd_siginfo));
			if (ret == sizeof(struct signalfd_siginfo))
			{
				if ( fdsi.ssi_signo == SIGTERM)
				{
					break;
				}
			}
		}
		else 
		{
			//EINTR,EFAULT
			//EINVAL,ENOMEM
			if ((errno == EINVAL) || (errno == ENOMEM))
			{
				struct timespec wait;
				wait.tv_sec  = 0;
				wait.tv_nsec = 100 * 1000 * 1000;
				
				::nanosleep(&wait, NULL);
			}
		}
	}
	while(1);
	
	
	ilm_destroy();
	
	return 0;
}
