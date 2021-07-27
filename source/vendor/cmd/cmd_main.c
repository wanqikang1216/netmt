/*
 * zebra daemon main routine.
 * Copyright (C) 1997, 98 Kunihiro Ishiguro
 *
 * This file is part of GNU Zebra.
 *
 * GNU Zebra is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * GNU Zebra is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Zebra; see the file COPYING.  If not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.  
 */

#include <zebra.h>

#include "version.h"
#include "getopt.h"
#include "command.h"
#include "thread.h"
#include "filter.h"
#include "memory.h"
#include "prefix.h"
#include "log.h"
#include "public_c.h"

/* Master of threads. */
struct thread_master *g_cmdmaster;

/* process id. */
pid_t g_cmdold_pid;
pid_t g_cmdpid;

/* Route retain mode flag. */
int retain_mode = 0;

/* Don't delete kernel route. */
int keep_kernel_mode = 0;


/* Default configuration file path. */
char config_current[] = " ";
char config_default[] = " ";

/* Process ID saved for use by init system */
char *pid_file = PATH_ZEBRA_PID;



/* SIGHUP handler. */
void 
sighup (int sig)
{
  zlog_info ("SIGHUP received");

  /* Reload of config file. */
  ;
}

/* SIGINT handler. */
void
sigint (int sig)
{
  /* Decrared in rib.c */
  void rib_close ();

  zlog_info ("Terminating on signal");

#ifdef HAVE_IRDP
  irdp_finish();
#endif

  exit (0);
}

/* SIGUSR1 handler. */
void
sigusr1 (int sig)
{
  zlog_rotate (NULL);
}

/* Signale wrapper. */
RETSIGTYPE *
signal_set (int signo, void (*func)(int))
{
  int ret;
  struct sigaction sig;
  struct sigaction osig;

  sig.sa_handler = func;
  sigemptyset (&sig.sa_mask);
  sig.sa_flags = 0;
#ifdef SA_RESTART
  sig.sa_flags |= SA_RESTART;
#endif /* SA_RESTART */

  ret = sigaction (signo, &sig, &osig);

  if (ret < 0) 
    return (SIG_ERR);
  else
    return (osig.sa_handler);
}

/* Initialization of signal handles. */
void
signal_init ()
{
  signal_set (SIGHUP, sighup);
  signal_set (SIGINT, sigint);
  signal_set (SIGTERM, sigint);
  signal_set (SIGPIPE, SIG_IGN);
  signal_set (SIGUSR1, sigusr1);
}
//typedef int     BOOL;
extern int GetAbsPath(char* szFile, char* szOutPath, int nBufLen);
/* Main startup routine. */
int cmd_main (int argc, char **argv,CMD_INIT_OTHER pfun)
{
  char *p;
  char *vty_addr = NULL;
  int vty_port = 0;
  int daemon_mode = 0;
  char *progname;
  struct thread thread;
  void rib_weed_tables ();
  void zebra_vty_init ();
  char szIniAbsPath[256]="\0";
 
  /* Set umask before anything for security */
  umask (0027);
  printf("init cmd progname =%s\n",argv[0]);
  /* preserve my name */
  progname = ((p = strrchr (argv[0], '/')) ? ++p : argv[0]);

  zlog_default = openzlog (progname, ZLOG_STDOUT, ZLOG_ZEBRA,
			   LOG_CONS|LOG_NDELAY|LOG_PID, LOG_DAEMON);


  /* Make master thread emulator. */
  g_cmdmaster = thread_master_create ();

  /* Vty related initialize. */
  cmd_init (1);
  vty_init ();
  pfun();
  if(!GetAbsPath("../etc/cmd.confg", szIniAbsPath, sizeof(szIniAbsPath)))
  {
        printf("\nGet cfg-file failed! file='%s'\n", "../etc/cmd.confg");
        return 1;
  }

  /* Sort VTY commands. */
  sort_node ();
  //config_file = "/home/ccj/cmd/aa.config";
  /* Configuration file read*/
  vty_read_config (szIniAbsPath, config_current, config_default);
  memset( szIniAbsPath, 0, sizeof(szIniAbsPath));
  if(!GetAbsPath("../etc/all.ini", szIniAbsPath, sizeof(szIniAbsPath)))
  {
        printf("\nGet cfg-file failed! file='%s'\n", "../etc/all.ini");
        return 1;
  }
  if(!CGetValueInt(szIniAbsPath,"comm", "serverport", 2018, &vty_port,""))
  {
	  printf("comm:serverport error!");
  }


  /* Needed for BSD routing socket. */
  g_cmdold_pid = getpid ();

  /* Daemonize. */
  if (daemon_mode)
    daemon (0, 0);

  /* Output pid of zebra. */
  pid_output (pid_file);

  /* Needed for BSD routing socket. */
  g_cmdpid = getpid ();

  /* Make vty server socket. */
  vty_serv_sock (vty_addr,
		 vty_port ? vty_port : 2018, "./");

  while (thread_fetch (g_cmdmaster, &thread))
    thread_call (&thread);

  /* Not reached... */
  exit (0);
}
