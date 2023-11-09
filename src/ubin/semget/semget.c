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

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <nanvix/pm.h>
#include <unistd.h>
#include <sys/sem.h>

/* Software versioning. */
#define VERSION_MAJOR 1 /* Major version. */
#define VERSION_MINOR 0 /* Minor version. */

/* Default Process name. */
#define PROCESS_DEFAULT 0

/*
 * Program arguments.
 */

int key;

/*
 * Prints program version and exits.
 */
static void version(void)
{
	printf("semget (Nanvix Coreutils) %d.%d\n\n", VERSION_MAJOR, VERSION_MINOR);
	printf("Copyright(C) 2011-2014 Pedro H. Penna\n");
	printf("This is free software under the ");
	printf("GNU General Public License Version 3.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\n\n");

	exit(EXIT_SUCCESS);
}

/*
 * Prints program usage and exits.
 */
static void usage(void)
{
	printf("Usage: semget [options] <id>\n\n");
	printf("Brief: Creates a semaphore. If already exists return semaphore id.\n\n");
	printf("Options:\n");
	printf("  --help             Display this information and exit\n");
	printf("  --version          Display program version and exit\n");

	exit(EXIT_SUCCESS);
}

/*
 * Gets number of the semaphore.
 */
static void getargs(int argc, char *const argv[])
{
	int i;	   /* Loop index.         */
	char *arg;  /* Current argument.   */

	/* Read command line arguments. */
	for (i = 1; i < argc; i++)
	{
		arg = argv[i];

		/* Parse command line argument. */
		if (!strcmp(arg, "--help"))
		{
			usage();
		}
		else if (!strcmp(arg, "--version"))
		{
			version();
		}
		else
		{
			key = atoi(arg);
		}
	}

	/* Invalid semaphore id. */
	if (key < 0 || key > SEM_MAX)
	{
		fprintf(stderr, "semget: wrong id for semaphore. Please choose a number between 0 and %d\n", SEM_MAX);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char *const argv[])
{
	int ret; // return from the function semget

	// Reading Arguments
	getargs(argc, argv);
	ret = semget(key);
	if (ret >= 0)
	{
		printf("%d\n", ret);
		return ret;
	}
	else
	{
		return -1;
	}
}