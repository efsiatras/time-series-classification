/*************************************************************************
Implementation File : functions.c
Author - Date       : Efstathios Siatras - 09/01/2017
Purpose             : Functions used in the Time-Series Classification Project
**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

double d(double a, double b, int i, int j, int c) { 							      /* Function to measure distance between two numbers */
	const double infty = 1.0/0.0; 													       /* Define infinity */
	int abs;
	if (i-j >= 0) 													    /* Find absolute value of i-j ; |i-j| */
		abs = i-j;
	else
		abs = j-i;

	if (abs <= c) 											      /* Check if absolute value observes the restriction */
		return (a - b) * (a - b); 												       /* Return distance */

	else /* (abs > c) */
		return infty; 								/* Absolute value is not valid based on the restriction ; Return infinity */
}


double min(double x, double y, double z) { 							     /* Function to find the smallest number out of three numbers */
	double min = x; 												     /* Assume that x is the smallest one */
	if (min > y) 														  /* Check if x is smaller than y */
		min = y; 											   /* If it's not, then set y as the smallest one */
	if (min > z) 														  /* Check if y is smaller than z */
		min = z; 											   /* If it's not, then set z as the smallest one */

	return min;														    /* Return the smallest number */
}

#ifdef REC
double dtwrec(int n, int i, double A[], int j, double B[], int c) {	        /* Function to calculate the DTW distance between two time-series using Recursion */
	if ((i > 1 && i <= n) && (j > 1 && j <= n)) 							  /* Simple implemented algorithm, given by the excercise */
		return d(A[i], B[j], i, j, c) + min(dtwrec(n, i-1, A, j, B, c), dtwrec(n, i, A, j-1, B, c), dtwrec(n, i-1, A, j-1, B, c));

	else if (i == 1 && (j > 1 && j <= n))
		return d(A[1], B[j], i, j, c) +  dtwrec(n, 1, A, j-1, B, c);

	else if (j == 1 && (i > 1 && i <= n))
		return d(A[i], B[1], i, j, c) + dtwrec(n, i-1, A, 1, B, c);

	else 																    /* (i == 1 && j == 1) */
		return d(A[1], B[1], i, j, c);
}

#else
double dtwdp(int n, double A[], double B[], double c) {		 /* Function to calculate the DTW distance between two time-series using Dynamic Programming (DP) */
	double **arr, result;
	int i, j, a;

	arr = malloc((n+1) * sizeof(double *)); 		 /* Make a two-dimensional array to save all DTW distances, till we find the one needed to return */
	if (arr == NULL) {
		fprintf(stderr, "Memory Error\n");

		return 1;
	}

	for(a = 0 ; a < n+1 ; a++) {
		arr[a] = malloc((n+1) * sizeof(double));
		if (arr[a] == NULL) {
			fprintf(stderr, "Memory Error\n");

			return 1;
		}
	}

	for (j = 1 ; j <= n ; j++) { 						     /* Start filling the two-dimensional array horizonally from left to right... */
		for (i = 1 ; i <= n ; i++) { 					        /* ...using the same simple implemented algorithm, given by the excercise */
			if ((i > 1 && i <= n) && (j > 1 && j <= n))
				arr[j][i] = d(A[i], B[j], i, j, c) + min(arr[j][i-1], arr[j-1][i], arr[j-1][i-1]);

			else if (i == 1 && (j > 1 && j <= n))
				arr[j][1] = d(A[1], B[j], i, j, c) + arr[j-1][1];

			else if (j == 1 && (i > 1 && i <= n))
				arr[1][i] = d(A[i], B[1], i, j, c) + arr[1][i-1];

			else if (i == 1 && j == 1)
				arr[1][1] = d(A[1], B[1], i, j, c);
		}
	}
	result = arr[n][n];    								    /* The DTW distance we need to return is the very last one calculated */

	for(a = 0 ; a < n+1 ; a++) 												/* Free the two-dimensional array */
		free (arr[a]);
	free(arr);

	return result; 														       /* Return the DTW distance */
}
#endif
