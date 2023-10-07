/*
 * Copyright(C) 2011-2016 Pedro H. Penna   <pedrohenriquepenna@gmail.com>
 *              2015-2016 Davidson Francis <davidsondfgl@hotmail.com>
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

#include <nanvix/clock.h>
#include <nanvix/const.h>
#include <nanvix/hal.h>
#include <nanvix/pm.h>
#include <signal.h>

/**
 * @brief Schedules a process to execution.
 *
 * @param proc Process to be scheduled.
 */
PUBLIC void sched(struct process *proc)
{
	proc->state = PROC_READY;
	proc->counter = 0;
}

/**
 * @brief Stops the current running process.
 */
PUBLIC void stop(void)
{
	curr_proc->state = PROC_STOPPED;
	sndsig(curr_proc->father, SIGCHLD);
	yield();
}

/**
 * @brief Resumes a process.
 *
 * @param proc Process to be resumed.
 *
 * @note The process must stopped to be resumed.
 */
PUBLIC void resume(struct process *proc)
{
	/* Resume only if process has stopped. */
	if (proc->state == PROC_STOPPED)
		sched(proc);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
//Round-Robin natural do Nanvix

PUBLIC void yield(void)
{
	struct process *p;    //Working process.
	struct process *next; // Next process to run.

	// Re-schedule process for execution.
	if (curr_proc->state == PROC_RUNNING)
		sched(curr_proc);

	// Remember this process.
	last_proc = curr_proc;

	//Check alarm.
	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		// Skip invalid processes.
		if (!IS_VALID(p))
			continue;

		// Alarm has expired.
		if ((p->alarm) && (p->alarm < ticks))
			p->alarm = 0, sndsig(p, SIGALRM);
	}

	// Choose a process to run next.
	next = IDLE;
	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		// Skip non-ready process.
		if (p->state != PROC_READY)
			continue;

		
		// Process with higher
		//waiting time found.
		 
/*		if (p->counter > next->counter)
		{
			next->counter++;
			next = p;
		}

		
		 //Increment waiting
		 //time of process.
		 
/*		else
			p->counter++;
	}

	//Switch to next process.
	next->priority = PRIO_USER;
	next->state = PROC_RUNNING;
	next->counter = PROC_QUANTUM;
	if (curr_proc != next)
		switch_to(next);
	}
*/

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
	Política de escalonamento utilizando prioridade efetiva
	(prioridade definida pelo usuário dada pela função nice,
	prioridade definida pelo sistema e tempo de espera na fila),
	a cada término de quantum, volta para checar se
	existe algum processo com prioridade maior,
	caso não, executa por mais um quantum.
	Processos com a mesma prioridade, serão revezados entre si
	a cada quantum.
*/

/**
 * @brief Yields the processor. Priority scheduling
 */
PUBLIC void yield(void)
{
	struct process *p;	 // Working process.
	struct process *next; // Next process to run.

	// Re-schedule process for execution.
	if (curr_proc->state == PROC_RUNNING)
		sched(curr_proc);

	// Remember this process.
	last_proc = curr_proc;

	// Check alarm.
	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		// Skip invalid processes.
		if (!IS_VALID(p))
			continue;

		// Alarm has expired.
		if ((p->alarm) && (p->alarm < ticks))
			p->alarm = 0, sndsig(p, SIGALRM);
	}

	// Choose a process to run next.
	next = IDLE;
	next->epriority = curr_proc->epriority;
	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		// Skip non-ready process.
		if (p->state != PROC_READY)
			continue;

		if (p->epriority >= next->epriority && p != curr_proc)
		{ // processo com prioridade maior ou igual que o processo atual, não sendo o processo atual

			next->epriority++;
			next->counter++;
			next = p;
		}

		// Increment waiting time of process and effective priority.
		else
		{
			p->epriority++;
			p->counter++;
		}
	}

	// Aqui, next é o processo com maior prioridade efetiva .

	// Switch to next process.
	next->priority = PRIO_USER;
	next->epriority = next->nice + next->priority; // Reseta a prioridade efetiva sem o counter que indica o tempo de fila e atualiza o valor com o novo valor de priority.
	next->state = PROC_RUNNING;
	next->counter = PROC_QUANTUM;
	if (curr_proc != next)
		switch_to(next); // Caso não haja um processo com a prioridade maior ou igual ao processo atual, mais um quantum será executado.
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
	Política de escalonamento utilizando prioridade efetiva,
	a cada término de quantum, volta para checar se
	existe algum processo com prioridade maior,
	caso não, executa por mais um quantum. Diferente do
	algoritmo de fila de prioridades aqui também implementado,
	esse algoritmo se preocupa apenas com prioridade maiores
	do que a do processo já executando.
	Processos com a mesma prioridade,
	vão ficar mais um tempo na fila de espera,
	aumentando sua prioridade efetiva, sendo o próximo
	a ser executado.
*/
/*
PUBLIC void yield(void)
{
	struct process *p;    //Working process.
	struct process *next; // Next process to run.

	// Re-schedule process for execution.
	if (curr_proc->state == PROC_RUNNING)
		sched(curr_proc);

	// Remember this process.
	last_proc = curr_proc;

	//Check alarm.
	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		// Skip invalid processes.
		if (!IS_VALID(p))
			continue;

		// Alarm has expired.
		if ((p->alarm) && (p->alarm < ticks))
			p->alarm = 0, sndsig(p, SIGALRM);
	}

	// Choose a process to run next.
	next = IDLE;
	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		// Skip non-ready process.
		if (p->state != PROC_READY)
			continue;

		//Process with higher effective priority found.
		if (p->epriority > next->epriority)
		{
			next->epriority++;
			next->counter++;
			next = p;
		}

		// Increment waiting time and effective priority of process.
		else
			p->epriority++;
			p->counter++;
	}

	// Switch to next process.
	next->priority = PRIO_USER;
	next->epriority = next->priority + next->nice;
	next->state = PROC_RUNNING;
	next->counter = PROC_QUANTUM;
	if (curr_proc != next)
		switch_to(next);
}

*/
