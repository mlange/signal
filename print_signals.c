#define HPUX
#define _XOPEN_SOURCE_EXTENDED 1

#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include "main.h"

static void handler(int sig);

static char *sig_names[] =  {
#if defined AIX
	"0  SIG0",      "1  SIGHUP",    "2  SIGINT",     "3  SIGQUIT",   
	"4  SIGILL",    "5  SIGTRAP",   "6  SIGABRT",    "7  SIGEMT",    
	"8  SIGFPE",    "9  SIGKILL",   "10 SIGBUS",     "11 SIGSEGV", 
	"12 SIGSYS",    "13 SIGPIPE",   "14 SIGALRM",    "15 SIGTERM", 
	"16 SIGURG",    "17 SIGSTOP",   "18 SIGTSTP",    "19 SIGCONT",
	"20 SIGCHLD",   "21 SIGTTIN",   "22 SIGTTOU",    "23 SIGIO",     
	"24 SIGXCPU",   "25 SIGXFSZ",   "27 SIGMSG",     "28 SIGWINCH",  
	"29 SIGPWR",    "30 SIGUSR1",   "31 SIGUSR2",    "32 SIGPROF", 
	"33 SIGDANGER", "34 SIGVTALRM", "35 SIGMIGRATE", NULL
#elif defined SUNOS
	"0  SIGO",     "1  SIGHUP",  "2  SIGINT",    "3  SIGQUIT",   
	"4  SIGILL",   "5  SIGTRAP", "6  SIGABRT",   "7  SIGEMT",    
	"8  SIGFPE",   "9  SIGKILL", "10 SIGBUS",    "11 SIGSEGV", 
	"12 SIGSYS",   "13 SIGPIPE", "14 SIGALRM",   "15 SIGTERM",   
	"16 SIGUSR1",  "17 SIGUSR2", "18 SIGCHLD",   "19 SIGPWR",    
	"20 SIGWINCH", "21 SIGURG",  "22 SIGPOLL",   "23 SIGSTOP",   
	"24 SIGTSTP",  "25 SIGCONT", "26 SIGTTIN",   "27 SIGTTOU",   
	"28 SIGVTALRM", NULL
#elif defined HPUX
	"0  SIGO",      "1  SIGHUP",  "2  SIGINT",  "3  SIGQUIT",   
	"4  SIGILL",    "5  SIGTRAP", "6  SIGABRT", "7  SIGEMT",    
	"8  SIGFPE",    "9  SIGKILL", "10 SIGBUS",  "11 SIGSEGV", 
	"12 SIGSYS",    "13 SIGPIPE", "14 SIGALRM", "15 SIGTERM",   
	"16 SIGUSR1",   "17 SIGUSR2", "18 SIGCHLD", "19 SIGPWR",
	"20 SIGVTALRM", "21 SIGPROF", "22 SIGIO",   "23 SIGWINCH",  
	"24 SIGSTOP",   "25 SIGTSTP", "26 SIGCONT", "27 SIGTTIN",   
	"28 SIGTTOU",   "29 SIGURG",   NULL
#else
	NULL
#endif

};


static struct sigaction actions[(sizeof sig_names) / sizeof *sig_names];


void print_signals(void)  {

	int col,
	    sig;

	static struct sigaction handler_action = { handler, 0L, 0 };

	/* Empty our own action set. */
	sigemptyset(&handler_action.sa_mask);

	/* Cycle through every signal we have. */
	for (sig = 0, col = 1; sig_names[sig]; sig++)
	{
		/* Replace the current action with our own. */
		/* This will fail on SIGKILL and SIGSTOP.   */
		if (sigaction(sig, &handler_action, &actions[sig]) == 0)
		{
	 
			switch ((int)actions[sig].sa_handler)
			{
				case (int) SIG_DFL:
					fprintf(stderr, "%-13s - SIG_DFL         %s", 
					        sig_names[sig],
					        (col % 2 == 0) ? "\n" : "");
					break;

				case (int) SIG_IGN:
					fprintf(stderr, "%-13s - SIG_IGN  *      %s",
							  sig_names[sig],
								(col % 2 == 0) ? "\n" : "");
					break;

				default:
					fprintf(stderr, "%-13s - %-8p *      %s",
							  sig_names[sig], 
							  actions[sig].sa_handler,
					        (col % 2 == 0) ? "\n" : "");

			}  /* switch */

			/* Increment our column counter. */
			col++;

		}  /* if */

	}  /* for */

	/* Print a linefeed if necessary. */
	if (col % 2 == 0)
		fprintf(stderr, "\n");

	/* Restore the original signal action. */
	for(sig = 0; sig_names[sig]; sig++)
		sigaction(sig, &actions[sig], (struct sigaction *)0);

	fflush(stderr);

	return;
}


static void handler(int sig)  {

	static int in_handler;
   
	in_handler = 1;

	if (!in_handler)
	{

		fprintf(stderr, __FILE__ " - %d: caught %s\n", __LINE__, sig_names[sig]);
		fflush(stderr);

		/* Call the original signal handler if the original action     */
		/* wasn't SIG_DFL and an actual handler exists for the signal. */
		if ((actions[sig].sa_handler != actions[sig].sa_handler) && 
			 (actions[sig].sa_handler != (void (*)()) -1))
         (*actions[sig].sa_handler)(sig);

		in_handler = 0;

	}

	return;
}
