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


/* Parameteres. */
int semid = -1;  /* Sempahore Identification. */
int op = -1; /* Option. */

/*
 * Prints program version and exits.
 */
static void version(void)
{
	printf("semop (Nanvix Coreutils) %d.%d\n\n", VERSION_MAJOR, VERSION_MINOR);
	printf("Copyright(C) 2011-2014 Pedro H. Penna\n");
	printf("This is free software under the ");
	printf("GNU General Public License Version 3.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\n\n");

	exit(EXIT_SUCCESS);
}

/*
 * Prints program usage and exits.
 */
static void usage(int status)
{
	printf("Usage: semop [semid] [op]\n\n");
	printf("Brief: Set different options for a semaphore.\n\n");
	printf("Options:\n");
	printf("  --help    Display this information and exit\n");
	printf("  --version Display program version and exit\n");

	exit(status);
}

/*
 * Gets number of the semaphore.
 */
static void getargs(int argc, char *const argv[])
{
	int i;     /* Loop index.       */
	char *arg; /* Working argument. */
	int aux;

	/* Get program arguments. */
	for (i = 1; i < argc; i++) {
		arg = argv[i];

		aux = atoi(arg);

		/* Display help information. */
		if (!strcmp(arg, "--help"))
			usage(EXIT_SUCCESS);

		/* Display program version. */
		else if (!strcmp(arg, "--version"))
			version();

		/* Get source/destination filenames. */
		else {
			/* Get semid */
			if (semid == -1){
				semid = aux;

                if (semid < 0 || semid > SEM_MAX){
                    fprintf(stderr, "semop: wrong id for semaphore. Please choose a number between 0 and %d.\n", SEM_MAX);
                    exit(EXIT_FAILURE);
                }
			}

			/* Get cmd */
			else if (op == -1){
				op = aux;

                // Op just cant be 0, because its not positive or negative
                if (op == 0){
                    fprintf(stderr, "semop: wrong command for semaphore. Please choose between 0, 1 and 3.\n");
                    exit(EXIT_FAILURE);
                }

			}
			/* Too many arguments. */
			else {
				fprintf(stderr, "semop: too many arguments\n");
				usage(EXIT_FAILURE);
			}
		}
	}

	/* Missing arguments. */
	if ((semid == -1) || (op == -1)) {
		fprintf(stderr, "semop: missing arguments\n");
		usage(EXIT_FAILURE);
	}
}


int main(int argc, char *const argv[])
{
	int ret; // return from the function semget

	// Reading Arguments
	getargs(argc, argv);

	ret = semop(semid, op);
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