/*
 * Copyright(C) 2011-2016 Pedro H. Penna   <pedrohenriquepenna@gmail.com>
 *              2015-2016 Davidson Francis <davidsondfgl@hotmail.com>
 *              2016-2016 Subhra S. Sarkar <rurtle.coder@gmail.com>
 *
 * This file is part of Nanvix.
 *
 * Nanvix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nanvix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#include <nanvix/config.h>
#include <nanvix/const.h>
#include <nanvix/dev.h>
#include <nanvix/fs.h>
#include <nanvix/hal.h>
#include <nanvix/mm.h>
#include <nanvix/pm.h>
#include <nanvix/klib.h>
#include <sys/stat.h>
#include <signal.h>
#include <limits.h>

/**
 * @brief Idle process page directory.
 */
EXTERN struct pde idle_pgdir[];

/**
 * @brief Idle process kernel stack.
 */
PUBLIC char idle_kstack[KSTACK_SIZE];

/**
 * @brief Process table.
 */
PUBLIC struct process proctab[PROC_MAX];

/**
 * @brief Current running process.
 */
PUBLIC struct process *curr_proc = IDLE;

/**
 * @brief Last running process.
 */
PUBLIC struct process *last_proc = IDLE;

/**
 * @brief Next available process ID.
 */
PUBLIC pid_t next_pid = 0;

/**
 * @brief Current number of processes in the system.
 */
PUBLIC unsigned nprocs = 0;

/**
 * @brief Initializes the process management system.
 */
PUBLIC void pm_init(void)
{
	int i;             /* Loop index.      */
	struct process *p; /* Working process. */

	/* Initialize the process table. */
	for (p = FIRST_PROC; p <= LAST_PROC; p++)
		p->flags = 0, p->state = PROC_DEAD;

	/* Handcraft init process. */
	IDLE->cr3 = (dword_t)idle_pgdir;
	IDLE->intlvl = 1;
	IDLE->flags = 0;
	IDLE->received = 0;
	IDLE->kstack = idle_kstack;
	IDLE->restorer = NULL;
	for (i = 0; i < NR_SIGNALS; i++)
		IDLE->handlers[i] = SIG_DFL;
	IDLE->irqlvl = INT_LVL_5;
	IDLE->pgdir = idle_pgdir;
	for (i = 0; i < NR_PREGIONS; i++)
		IDLE->pregs[i].reg = NULL;
	IDLE->size = 0;
	for (i = 0; i < OPEN_MAX; i++)
		IDLE->ofiles[i] = NULL;
	IDLE->close = 0;
	IDLE->umask = S_IXUSR | S_IWGRP | S_IXGRP | S_IWOTH | S_IXOTH;
	IDLE->tty = NULL_DEV;
	IDLE->status = 0;
	IDLE->nchildren = 0;
	IDLE->uid = SUPERUSER;
	IDLE->euid = SUPERUSER;
	IDLE->suid = SUPERUSER;
	IDLE->gid = SUPERGROUP;
	IDLE->egid = SUPERGROUP;
	IDLE->sgid = SUPERGROUP;
	IDLE->pid = next_pid++;
	IDLE->pgrp = IDLE;
	IDLE->father = NULL;
	kstrncpy(IDLE->name, "idle", NAME_MAX);
	IDLE->utime = 0;
	IDLE->ktime = 0;
	IDLE->cutime = 0;
	IDLE->cktime = 0;
	IDLE->state = PROC_RUNNING;
	IDLE->counter = PROC_QUANTUM;
	IDLE->priority = PRIO_USER;
	IDLE->nice = NZERO;
	IDLE->epriority = PRIO_USER + NZERO; //Define a prioridade efetiva do IDLE
	IDLE->alarm = 0;
	IDLE->next = NULL;
	IDLE->chain = NULL;

	nprocs++;

	enable_interrupts();
}

/*As três funções abaixo são desenvolvidas para a formatação
do texto, ao se printar na tela para o usuário. Elas convertem
inteiros para caracteres da melhor forma para imprimir na tela
utilizando a função kprintf.
*/
void rvr(char* s)
{
	int i, j;
	char c;

	for (i = 0, j = kstrlen(s)-1; i<j; i++, j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

void ips(int n, char* s)
{
	int i, sign;

	if ((sign = n) < 0)
		n = -n;

	i = 0;
	do
	{
		s[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);

	if (sign < 0)
		s[i++] = '-';

	s[i] = '\0';
	rvr(s);
}

void pValue(int value, char* s, int padding)
{
	int i,len,size;

	ips(value, s);
	len = padding - kstrlen(s);

	size = kstrlen(s);
	for(i=size; i<len+size; i++)
		*(s+i) = ' ';

	*(s+i) = '\0';
}

//----------------------------------------------------------------------

/*
Salva as informações da struct process do pid requerido,
em buf, e depois mostra na tela o resultado para o usuário.
*/
//pid_t --> typedef de signed
PUBLIC void do_get_process_info(pid_t pid, struct process_buf *buf){
	struct process *proc;
	int aux = 1; //salva a posição de proctable que aquele processo ocupa

	//Loop na tabela de processos
	for (proc = FIRST_PROC; proc <= LAST_PROC; proc++){
		//Checa se aquele processo possui o pid requerido
		if(proc->pid == pid){
			buf->pid = proctab[aux].pid;
			buf->state = proctab[aux].state;
			buf->priority = proctab[aux].priority;
			buf->utime = proctab[aux].utime;
			buf->ktime = proctab[aux].ktime;
		}
		
		aux++;
	}

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
		pValue(buf->pid, p, 6);

		/* Priority */
		pValue(buf->priority, priority, 11);

		/* Utime */
		pValue(buf->utime, utime, 8);

		/* Ktime */
		pValue(buf->ktime, ktime, 10);

		kprintf("%s %s %s %s %s", p,
			priority, utime, ktime, states[(int)buf->state] );
}
