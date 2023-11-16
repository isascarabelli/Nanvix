/*
 * Copyright(C) 2011-2016 Pedro H. Penna <pedrohenriquepenna@gmail.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

/**
* @brief Performs changes on max value of semaphore or return the current value.
*/

#include <nanvix/const.h>
#include <nanvix/hal.h>
#include <nanvix/klib.h>
#include <nanvix/pm.h>
#include <sys/sem.h>

PUBLIC int sys_semctl(int semid, int cmd, int val){

    struct semaphore *sem;

    //Searching for semid
    for (sem = FIRST_SEM; sem < LAST_SEM; sem++){
        if (sem->id == semid){
            if (cmd == GETVAL)
                return sem->curr_val;       //Return current value.
            if (cmd == SETVAL)
                semtab[semid].val = val;     //Set max value of semaphore.
            if (cmd == IPC_RMID && sem->flag == UNLOCKED){
                semtab[semid].id = -1;       //Delete semaphore id.
            } else {
                wakeup(curr_proc->chain);
                semtab[semid].flag = UNLOCKED;
                semtab[semid].id = -1;
                semtab[semid].val = 1;
                semtab[semid].curr_val = 1;
            }
        }
    }

    return 0;
    
}
