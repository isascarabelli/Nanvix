/*
 Implementação da chamada de kernel sys_get_process_info() 
 que invoca a função de kernel do_get_process_info(), 
 implementada em src/kernel/pm/pm.c e mostra na tela para o usuário
os dados daquele processo contido em process_buf.
 */

#include <nanvix/klib.h>
#include <nanvix/pm.h>

//Formatação
void prepareValue(int value, char* s, int padding)
{
	int i,len,size;

	itoa (value, s, 10);
	len = padding - kstrlen(s);

	size = kstrlen(s);
	for(i=size; i<len+size; i++)
		*(s+i) = ' ';

	*(s+i) = '\0';
}

PUBLIC void  sys_get_process_info( pid_t pid, struct process_buf *buf ){
	do_get_process_info( pid, buf );

	//Parâmetros abaixo usados apenas para formatação na hora de imprimir para o usuário
	char p[26];
	char priority[26];
	char utime   [26];
	char ktime   [26];

	const char *states[7];
	states[0] = "DEAD";
	states[1] = "ZOMBIE";
	states[2] = "RUNNING";
	states[3] = "READY";
	states[4] = "WAITING";
	states[5] = "SLEEPING";
	states[6] = "STOPPED";

	kprintf("------------------------------- Process Status"
			" -------------------------------\n"
		    "PID    PRIORITY "
		    "   UTIME   KTIME     STATUS");

		/* Pid */
		prepareValue(buf->pid, p, 6);

		/* Priority */
		prepareValue(buf->priority, priority, 11);

		/* Utime */
		prepareValue(buf->utime, utime, 8);

		/* Ktime */
		prepareValue(buf->ktime, ktime, 10);

		kprintf("%s%s%s%s%s%s%s%s", pid,
			priority, utime, ktime, states[(int)buf->state] );
}

