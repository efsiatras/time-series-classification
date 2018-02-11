/*************************************************************************
Implementation File : time_series_classification.c
Author - Date       : Efstathios Siatras - 09/01/2017
Purpose             : Time-series classification
**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "functions.h"

int main(int argc, char *argv[]) {
	clock_t start, end;
	double cpu_time_used;

	double **train, **test;
	int *train_class, *test_class;
	double temp_dtw, near_dtw = 1.0/0.0;
	int c, near_series, error_counter = 0;
	int train_counter, train_n, i, j, test_counter, test_n, k, l;

	if (argc == 1) { 															/* If there are no other arguments */
		c = INT_MAX; 													   /* Set restriction to infinity ; No restriction */
	}

	else if (argc == 2) { 															  /* If there is one more argument */
		c = atoi(argv[1]); 												    /* Set the numeral argument as the restriction */
	}

	else {																        /* If there are too many arguments */
		fprintf(stderr, "Too many arguments");															  /* Error */
		return 3;
	}

	scanf("%d %d", &train_counter, &train_n); 								    /* Get the number and the length of training set's time-series */

	train = malloc((train_counter+1) * sizeof(double *)); 						  /* Create a two-dimensional array to save the training set's time-series */
	if (train == NULL) {
		fprintf(stderr, "Memory error\n");

		return 1;
	}

	for (j = 0 ; j < train_counter+1 ; j++) {
		train[j] = malloc((train_n+1) * sizeof(double));
		if (train[j] == NULL) {
			fprintf(stderr, "Memory error\n");

			return 1;
		}
	}

	train_class = malloc((train_counter+1) * sizeof(int)); 						      /* Create an array to save the classes of training set's time-series */

	for (j = 1 ; j <= train_counter ; j++) { 											     /* Fill up the array with the classes */
		scanf(" %d", &train_class[j]);

		for (i = 1 ; i <= train_n ; i++) 									 /* Fill up the two-dimensional array with the time-series */
			scanf(" %lf", &train[j][i]);
	}

	scanf(" %d %d", &test_counter, &test_n); 									/* Get the number and the length of test set's time-series */

	test = malloc((test_counter+1) * sizeof(double *)); 						      /* Create a two-dimensional array to save the test set's time-series */
	if (test == NULL) {
		fprintf(stderr, "Memory error\n");

		return 1;
	}

	for (l = 0 ; l < test_counter+1 ; l++) {
		test[l] = malloc((test_n+1) * sizeof(double));
		if (test[l] == NULL) {
			fprintf(stderr, "Memory error\n");

			return 1;
		}
	}

	test_class = malloc((test_counter+1) * sizeof(int)); 							  /* Create an array to save the classes of test set's time-series */

	for (l = 1 ; l <= test_counter ; l++) { 											     /* Fill up the array with the classes */
		scanf(" %d", &test_class[l]);

		for (k = 1 ; k <= test_n ; k++) 									 /* Fill up the two-dimensional array with the time-series */
			scanf(" %lf", &test[l][k]);
	}

	if (train_n != test_n) { 					         /* If length of training set's time-series is different from the length of test set's time-series */
		for(i = 0 ; i < train_counter+1 ; i++) 														    /* Free arrays */
			free (train[i]);
		free(train);
		free(train_class);

		for(i = 0 ; i < test_counter+1 ; i++)
			free (test[i]);
		free(test);
		free(test_class);

		fprintf(stderr, "Error: Length of training set's time-series is different from the length of test set's time-series"); 					  /* Error */

		return 4;
	}

	start = clock(); 																	     /* Start time */
	for (l = 1 ; l <= test_counter ; l++) {
		for (j = 1 ; j <= train_counter ; j++) {
			#ifdef REC
			temp_dtw = dtwrec(train_n, train_n, test[l], train_n, train[j], c); 							              /* train_n == test_n */
			#else
			temp_dtw = dtwdp(train_n, test[l], train[j], c);										      /* train_n == test_n */
			#endif
			if (temp_dtw < near_dtw) {	 /* Check all distances of current test set's time-serie with all training set's time-series to find the smallest distance */
				near_dtw = temp_dtw; 												     /* Save the smallest distance */
				near_series = j; 	 /* Save the number of the training set's time-serie that has the smallest distance with the current test set's time-serie */
			}
		}
		printf("Series %d (class %d) is nearest (distance %f) to series %d (class %d)\n", l, test_class[l], near_dtw, near_series, train_class[near_series]);
		if (train_class[near_series] != test_class[l])									        /* If predicted class is not the right one */
			error_counter++; 														 /* Count number of errors */
		near_dtw = 1.0/0.0;
	}
	fprintf(stderr, "Error rate: %.3f\n", (double) error_counter / test_counter); 								 /* Calculate and print Error rate */
	end = clock();																		       /* End time */
 	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; 											       /* Measure CPU time */
 	fprintf(stderr, "CPU time: %.2f secs\n", cpu_time_used); 												 /* Print CPU time */

	for(i = 0 ; i < train_counter+1 ; i++) 															    /* Free arrays */
		free (train[i]);
	free(train);
	free(train_class);

	for(i = 0 ; i < test_counter+1 ; i++)
		free (test[i]);
	free(test);
	free(test_class);

	return 0;
}
