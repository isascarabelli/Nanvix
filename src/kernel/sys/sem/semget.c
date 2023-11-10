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
* @brief Creates a semaphore or returns the key.
*/

#include <nanvix/const.h>
#include <nanvix/hal.h>
#include <nanvix/klib.h>
#include <nanvix/pm.h>
#include <sys/sem.h>
#include <stdio.h>

PUBLIC int sys_semget(int key){

    struct semaphore *sem;

    //Searching for key
    for (sem = FIRST_SEM; sem < LAST_SEM; sem++){
        if (sem->id == key){
            return key;
        }
    }

    semtab[key].id = key;          /**< Creating semaphore. */
    semtab[key].val = SETVAL;      /**< Set semaphore with max value (mutex). */
    semtab[key].curr_val = SETVAL; /**< Semaphore with current value 1 (mutex). */
    return key;

}
