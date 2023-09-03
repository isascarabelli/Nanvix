/*
 Implementação da chamada de kernel sys_get_process_info() 
 que invoca a função de kernel do_get_process_info(), 
 implementada em src/kernel/pm/pm.c. Ela é invocada em 
 getp.c por NR_get_process, para realizar a passagem
 do espaço de usuário para o espaço de kernel.
 */

#include <nanvix/klib.h>
#include <nanvix/pm.h> 

PUBLIC void  sys_get_process_info( pid_t pid, struct process_buf *buf ){
	do_get_process_info( pid, buf );
}
