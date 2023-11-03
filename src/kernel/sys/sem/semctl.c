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

#include <nanvix/const.h>
#include <nanvix/hal.h>
#include <nanvix/klib.h>
#include <nanvix/pm.h>
#include <sys/sem.h>

PUBLIC void sys_semctl(int semid, int cmd, int val){

    struct semaphore *sem;

    for (sem = 0; sem < SEM_MAX; sem++){
        if (sem->id == semid){
            if (cmd == GETVAL)
                return sem->curr_val;
            if (cmd == SETVAL)
                sem->val = val;
            if (cmd == IPC_RMID){
                sem->id = -1;
            }
        }
    }
    
}
