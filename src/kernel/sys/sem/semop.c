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
* @brief Down and Up operations on semaphore.
*/

#include <nanvix/const.h>
#include <nanvix/hal.h>
#include <nanvix/klib.h>
#include <nanvix/pm.h>
#include <sys/sem.h>

//Current value decrement or sleeping process
PUBLIC void down_sem(struct semaphore *sem, int semid){

    if (sem->curr_val > 0)
        semtab[semid].curr_val--;
    else {
        sleep(curr_proc->chain, PRIO_USER);
        semtab[semid].flag = LOCKED;
    }

}

//Current value increment and sleeping process
PUBLIC void up_sem(struct semaphore *sem, int semid){

    if (sem->flag == LOCKED){
        wakeup(curr_proc->chain);
        semtab[semid].flag = UNLOCKED;
        semtab[semid].curr_val++;
    } else if (sem->curr_val > 0 && sem->curr_val < sem->val){
        semtab[semid].curr_val++;
    }

}

PUBLIC int sys_semop(int semid, int op){

    struct semaphore *sem;

    //Searching for semid
    for (sem = FIRST_SEM; sem < LAST_SEM; sem++){
        if (sem->id == semid){
            if (op < 0) {
                down_sem(sem, semid);
            } else if (op > 0) {
                up_sem(sem, semid);
            }
        }
    }

    return 0;

}
