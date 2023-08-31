/*
 * Implementação da chamada de kernel sys_get_process_info() que invoca a função de kernel do_get_process_info(), implementada em src/kernel/pm/pm.c
 */

#include <nanvix/const.h>
#include <nanvix/clock.h>
#include <nanvix/dev.h>
#include <nanvix/fs.h>
#include <nanvix/klib.h>
#include <nanvix/mm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <nanvix/pm.h>

PUBLIC int  sys_get_process_info( pid_t pid, struct process_buf *buf ){
	return (do_get_process_info( pid, buf ));
}

