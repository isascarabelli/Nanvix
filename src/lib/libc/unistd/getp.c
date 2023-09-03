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

#include <unistd.h>
#include <errno.h>
#include <nanvix/pm.h>
#include <nanvix/syscall.h>

/*
 * Get process from pid
 */
ssize_t getp(pid_t pid, struct process_buf *buf)
{
	ssize_t ret;

	__asm__ volatile (
		"int $0x80"
		: "=a" (ret)
		: "0" (NR_get_process),
		  "b" (pid),
		  "c" (buf)
	);

	/* Error. */
	if (ret < 0)
	{
		errno = -ret;
		return (-1);
	}

	return ((ssize_t)ret);
}
