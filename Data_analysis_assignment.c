
/* Program to implement a "calculator" for CSV data files

   Skeleton written by Alistair Moffat, ammoffat@unimelb.edu.au,
   April 2024, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: [Benjamin Cheng 1353182]
   Dated:     [14/5/2024]

Programming is fun!*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>


#define SOLUTION 1
#define MAXROWS 1000
#define MAXCOLS 50

#define LINELEN	500	/* maximum length of any input line */

#define COMMA	','	/* separator for CSV files */

#define NULCMD	'0'	/* empty command */
#define INDEXX	'i'	/* command to list the column headings */
#define DATDMP	'd'	/* command to dump out the data */
#define AVERGE	'a'	/* command to average a column */
#define GRAPHH	'g'	/* command to graph a column */
#define CATAVG	'c'	/* command to category-average a column */
#define KNDALL	'k'	/* command to compute Kendall's tau */

#define MAXARGS  2      /* maximum number of arguments to any command */
#define ARGS_0	"0id"	/* commands that take no arguments */
#define ARGS_1	"ag"	/* commands that take one argument */
#define ARGS_2	"ck"	/* commands that take two arguments */
#define ALLOPS  (ARGS_0 ARGS_1 ARGS_2)
            /* list of all valid commands */

#define GRAPHROWS 20	/* number of rows in graph */
#define GRAPHCOLS 50	/* number of columns in graph */
#define EPSILON   1e-6	/* small adjustment for graph computation */

#define MAXCATS	1000	/* maximum number of categories to be handled */

#define FILINP	1	/* indicates command input coming from a file */
#define PROMPT	"> "

#define MAXSCALE 20

/****************************************************************/

/* structure declarations -- probably no need to change these,
   but you can if you wish... */

typedef char input_line_t[LINELEN+1];

typedef struct {
    char *label;
    double vals[MAXROWS];
} column_t;

typedef struct {
    input_line_t labelstring;
    int nrows;
    int ncols;
    /* the csv matrix is stored as a collection of columns of data */
    column_t cols[MAXCOLS];
} csv_t;

typedef struct {
    char command;
    int nargs;
    int argvals[MAXARGS];
} command_t;

/****************************************************************/

/* function prototypes */

void	read_csv_file(char *fname, csv_t *D);
void	reassign_input(char *fname);
void    print_prompt(void);
int     read_command(command_t *comd, int fileinput, int ncols);
void	process_line(command_t *comd, csv_t *D);
void    do_datdmp(csv_t *D);
void    do_indexx(csv_t *D);
void    do_averge(column_t *col, int nrows);
void    do_graphh(column_t *col, int nrows);
void    do_catavg(column_t *cat, column_t *col, int nrows);
void    do_kndall(column_t *col1, column_t *col2, int nrows);

void    small_to_big(double arr[], int size);
void    dub_swap(double *p1, double *p2);
void    print_array(double arr[], int size);
void    sort_int_arrays(double stud_nos[], double marks[], int n);
void    sort_avg(double *temp, double vals[], double location[], int n, 
        int *counter, int *start);
void    big_to_small(double arr[], int size);
void    removeDuplicates(double arr[], int *size, int *unique);
void    copy_array(double temp[], double array[], int width);
void    scale_graph(int array[], int width, int *scale);
void    big_to_small_int(int arr[], int size);
void    int_swap(int *p1, int *p2);
void    copy_array_int(int temp[], int array[], int width);
/****************************************************************/

/* main program controls all the action
 */
int
main(int argc, char *argv[]) {

    int fileinput=0;
    command_t comd;

    /* declare the structure that will hold the csv data */
    csv_t D;

    /* first argument on commandline is the data file name */
    read_csv_file(argv[1], &D);

    /* second argument, if it exists, is file of input commands */
    if (argc==3) {
        fileinput = 1;
        reassign_input(argv[2]);
    }

    /* start the main execution loop */
    print_prompt();
    while (read_command(&comd, fileinput, D.ncols) != EOF) {
        process_line(&comd, &D);
        /* then round we go */
        print_prompt();
    }

    /* all done, so pack up and go home */
    printf("to daa!\n");
    return 0;
}

/****************************************************************/

/* reads a csv file in to the defined structure, with empty or
   non-numeric values replaced by 0.0/0.0 = nan so as to prevent
   downstream arithmetic being interpreted incorrectly. Probably
   best to just leave this function alone, you don't need to edit
   it to complete the project
*/

void
read_csv_file(char *fname, csv_t *D) {

    FILE *fp;	/* used to read from a named file */
    input_line_t line;
    int ncols=0, nrows=0, bytes=0;
    int c, i, j, chr, ncommas, empties=0;
    double x;

    /* yes, this looks suspicious, but it works as required */
    double nan = 0.0/0.0;

    /* first argument on commandline should the data file name */
    if (fname==NULL) {
        /* and it wasn't there... */
        printf("No csv file specified on commandline\n");
        exit(EXIT_FAILURE);
    }

    /* try and open the named file for reading */
    if ((fp=fopen(fname,"r")) == NULL) {
        printf("Error: unable to open %s\n", fname);
        exit(EXIT_FAILURE);
    }

    /* file is open, can now use fp to access CSV data,
       start by reading the bytes of the header row */
    while (((c=getc(fp))!=EOF) && (c!='\n' && c!='\r')) {
        D->labelstring[bytes++] = c;
    }
    D->labelstring[bytes] = '\0';

    /* now process line again, breaking in to separate labels by
       replacing commas by nulls, and tracking the start of each of
       the column headings */
    D->cols[ncols++].label = D->labelstring;
    for (i=1; i < bytes; i++) {
        if (D->labelstring[i] == COMMA) {
            D->labelstring[i] = '\0';
            D->cols[ncols++].label = D->labelstring+i+1;
        }
        if (ncols>MAXCOLS && i<bytes) {
            printf("Too many columns, limit is %d\n",
                MAXCOLS);
            exit(EXIT_FAILURE);
        }
    }

    /* ok, that's the labels sorted, now for the data */
    while ((chr=getc(fp)) != EOF) {

        /* there is another row, because first character of it
           just got read
        */ 

        /* but check first to make sure won't overflow array */
        if (nrows==MAXROWS) {
            /* time to stop reading data */
            printf("Too many rows, truncated at %d\n", MAXROWS);
            break;
        }

        /* going to be ok, read the line */
        i = 0;
        line[i++] = chr;
        ncommas = (chr==COMMA) ;
        while (((chr=getc(fp))!=EOF) && (chr!='\n' && chr!='\r')) {
            line[i++] = chr;
            ncommas += (chr==COMMA) ;
        }
        line[i] = '\0';
        if (ncommas!=ncols-1) {
            printf("Data input error line %d\n", nrows+2);
            exit(EXIT_FAILURE);
        }

        /* then process that line from end back to start */
        j = i-1;
        for (c=ncols-1; c>=0; c--) {
            /* look for next previous comma */
            while (j>=0 && line[j]!=COMMA) {
                j--;
            }
            /* access the value */
            if (sscanf(line+j+1, "%lf", &x) == 1) {
                D->cols[c].vals[nrows] = x;
            } else {
                D->cols[c].vals[nrows] = nan;
                empties++;
            }
            /* mark the new end of the string */
            if (j>=0) {
                line[j--] = '\0';
            }
        }

        /* finished with this row, step the row counter */
        nrows++;
        /* and go round the loop and see if there is another row */
    }

    /* either input has all been read or array is full */
    printf("Data file: %s\n    %d columns and %d rows\n",
            fname, ncols, nrows);
    if (empties) {
        printf("    %d entries were empty or non-numeric\n",
            empties);
    }

    /* finish populating the structure */
    D->nrows = nrows;
    D->ncols = ncols;
    return;
}

/****************************************************************/

/* if there is a valid filename on the commandline, redirect stdin
   so that the file is read, and return FILINP to show that input
   input lines should be echoed to the output when they are read
*/
void
reassign_input(char *fname) {
    if (freopen(fname, "r", stdin)==NULL) {
        printf("Unable to open \"%s\"\n", fname);
        exit(EXIT_FAILURE);
    }
    /* stdin successfully reopened to the named file */
    printf("Commands file: %s\n", fname);
    return;
}

/****************************************************************/

/* print the "ready for input" prompt
*/
void
print_prompt(void) {
    printf(PROMPT);
    return;
}

/****************************************************************/

/* read a line of input into the array passed as argument
   returns false if there is no input available
   all whitespace characters are removed
*/
int    
read_command(command_t *comd, int fileinput, int ncols) {

    int i=0, c;
    int col;
    input_line_t line;

    /* get a whole input line, single blank of multiples */
    while (((c=getchar())!=EOF) && (c!='\n')) {
        if (i<LINELEN) {
            line[i] = c;
            if (i==0 || (isspace(line[i-1])*isspace(line[i])==0)) {
                i++;
            }
        }
    }
    /* close it off */
    line[i] = '\0';

    /* echo the input line if it came from an input file */
    if (fileinput) {
        /* print out the input command */
        printf("%s\n", line);
    }

    /* nothing typed? straight back to caller */
    if (i==0 && c==EOF) {
        return EOF;
    }
    if (i==0) {
        comd->command = NULCMD;
        comd->nargs = 0;
        return !EOF;
    }

    /* something typed? parse into parts needed */
    comd->command = line[0];
    comd->nargs = 0;
    for (i=1; line[i]; i++) {
        if (!isdigit(line[i]) && !isspace(line[i])) {
            printf("Invalid input character\n");
            return 1;
        }
        if (line[i-1]==' ' && line[i]!=' ') {
            col = atoi(line+i);
            comd->argvals[comd->nargs++] = col;
        }
    }
    return ((i>0) || (c!=EOF));
}

/****************************************************************/

/* process a command by parsing the input line into parts and
   carrying out the specified action
 */
void
process_line(command_t *comd, csv_t *D) {

    int optype, col1=0, col2=0;

    /* determine the operation to be performed, it
       must be first character in line
     */
    optype = comd->command;
    if (strchr(ALLOPS, optype) == NULL) {
        printf("Unknown operator\n");
        return;
    }

    /* determine the string argument (if one is required),
       it must start in second character of line
     */
    if (strchr(ARGS_0, optype)) {
        if (comd->nargs != 0) {
            printf("No argument required for '%c'\n", optype);
            return;
        }
    } else if (strchr(ARGS_1, optype)) {
        if (comd->nargs != 1) {
            printf("One argument required for '%c'\n", optype);
            return;
        }
        col1 = comd->argvals[0];
        if (col1>D->ncols) {
            printf("Invalid column number, ");
            printf("max is %d\n", D->ncols);
            return;
        }
    } else if (strchr(ARGS_2, optype)) {
        if (comd->nargs != 2) {
            printf("Two arguments required for '%c'\n", optype);
            return;
        }
        col1 = comd->argvals[0];
        col2 = comd->argvals[1];
        if (col1 > D->ncols || col2 > D->ncols) {
            printf("Invalid column number, ");
            printf("max is %d\n", D->ncols);
            return;
        }
    }

    /* now need to rebase the column numbers to suit the zero-origin
       counting in C */
    col1 -= 1;
    col2 -= 1;

    /* finally, make the actual operation take place
     */
    if (optype == NULCMD) {
        /* empty commands are easy! */
    } else if (optype == INDEXX) {
        do_indexx(D);
    } else if (optype == DATDMP) {
        do_datdmp(D);
    } else if (optype == AVERGE) {
        do_averge((D->cols)+col1, D->nrows);
    } else if (optype == GRAPHH) {
        do_graphh((D->cols)+col1, D->nrows);
    } else if (optype == CATAVG) {
        do_catavg((D->cols)+col1, (D->cols)+col2, D->nrows);
    } else if (optype == KNDALL) {
        do_kndall((D->cols)+col1, (D->cols)+col2, D->nrows);
    }
    return;
}

/****************************************************************/

/* provide an index list of the column headings
*/
void
do_indexx(csv_t *D) {
    int c;
    printf("      col  data\n");
    for (c=0; c < D->ncols; c++) {
        printf("      %3d  %s\n", c+1, D->cols[c].label);
    }
    return;
}

/****************************************************************/

/* dump out all of the data in the CSV structure D
*/
void
do_datdmp(csv_t *D) {
    int r, c;
    /* first the header labels */
    printf("      ");
    for (c=0; c < D->ncols; c++) {
        printf(" %10s", D->cols[c].label);
    }
    printf("\n");
    /* now the values in the data rows */
    for (r=0; r < D->nrows; r++) {
        printf("%4d: ", r+1);
        for (c=0; c < D->ncols; c++) {
            printf("    %7.2f", D->cols[c].vals[r]);
        }
        printf("\n");
    }
    return;
}

/****************************************************************/

/* And now for the four commands that you are to write....
*/

/****************************************************************/

/* implement the 'a' averaging command
*/
void
do_averge(column_t *col, int nrows) {
    int i;
    double avg = 0, temp[nrows];
    
    copy_array(temp, col->vals, nrows);
    //order vals from small to big
    small_to_big(temp, nrows);
    //calculate avg   
    for(i = 0; i < nrows;i++){
        avg = avg + temp[i];
    }
    avg = avg/nrows;
    
    printf("average %s: %7.2lf (over %d values)\n",col->label, avg, nrows);
    printf("    max %s: %7.2f\n",col->label, temp[nrows-1]);
    printf("    min %s: %7.2f\n",col->label, temp[0]);

    return;
}

/****************************************************************/

/* implement the 'g' graphing command
*/
void
do_graphh(column_t *col, int nrows) {
    double temp, val, bucket[4*nrows], colvals[nrows];
    double max, min;
    int i = 0, j, k, counter[GRAPHROWS]= {0}, scale = 1, count;
    
    copy_array(colvals, col->vals, nrows);
    small_to_big(colvals, nrows);
    //calculate max and min vals
    max = colvals[nrows - 1] ;
    min = colvals[0];
    
    temp = ((max + EPSILON) - (min - EPSILON)) / (double)GRAPHROWS;
    
    val = colvals[nrows - 1] + EPSILON;
    
    //compute bucket values
    while(i <= GRAPHROWS){
        bucket[i] = val;
        val = val - temp;
        i++;
    }
    
    for(i = 0; i < GRAPHROWS; i++){
        //Compute number of values in the bucket
        for(j = 0; j < nrows; j++){
            if((col->vals[j] < bucket[i]) && (col->vals[j] >= bucket[i + 1])){
                counter[i]++;  
            }     
            
        }
    }
    //calculate scale
    scale_graph(counter, GRAPHROWS, &scale);
    
    printf("graph of %s, scaled by a factor of %d\n", col->label, scale);
    for(i = 0; i < GRAPHROWS; i++){
        if(bucket[i+1] == 0){
            printf("  -%0.2f--%7.2f ",bucket[i+1], bucket[i]);
        }
        else{
            printf("%7.2f--%7.2f ",bucket[i+1], bucket[i]);
        }
        
    printf("[%5d]:", counter[i]);
        if(scale == 1){
            for(k = 0; k < counter[i]; k++){
                printf("*");
            }
        }
        else{
            //print amount of astericks 
            count = (((10 * counter[i]) / scale) + 9) / 10;
            for(k = 0; k < count; k++){
                printf("*");
            }
            
        }
        
    printf("\n");
    }
    return;
}

/****************************************************************/
/* implement the 'c' category average command
*/
void
do_catavg(column_t *cat, column_t *col, int nrows) {
    int counter[MAXROWS] = {0}, remain , i, start = 0;
    double temp[MAXROWS] = {0}, colvals[nrows], catvals[nrows];

    copy_array(colvals, col->vals, nrows);
    copy_array(catvals, cat->vals, nrows);
    //sort first col in ascending order while col1 and col2 are in pairs 
    sort_int_arrays(catvals, colvals, nrows);
    //computer catergory avg
    sort_avg(&temp[0], colvals, catvals, nrows, &counter[0], &start);
    
    for(i = 1; i < nrows; i++){
        sort_avg(&temp[i], colvals, catvals, nrows, &counter[i], 
        &start);
    }

    removeDuplicates(catvals, &nrows, &remain);
    
    printf("%10s   average %s\n",cat->label, col->label);
    printf("%9.2f %9.2f (over %d values)\n",catvals[0], temp[0], counter[0]);
    
    for(i = 1; i < remain; i++){
        printf("%9.2f %9.2f (over %d values)\n",catvals[i], temp[i], 
        counter[i]); 
    }  
}

/****************************************************************/

/* implement the 'k' command to compute Kendall's tau-b
   coefficient for similarity between two sets of paired values
*/
void   
do_kndall(column_t *col1, column_t *col2, int nrows) {
    int i, j, con_pairs = 0, dis_pairs = 0;
    double tau = 0;
    //calculate concordant pairs
    for(i = 0; i < nrows; i++){
        for(j = 0; j < nrows; j++){
            if(col1->vals[i] < col1->vals[j] && col2->vals[i] < col2->vals[j]){
                con_pairs++;
            }
    //calculate discordant pairs 
            else if(col1->vals[i] < col1->vals[j] && col2->vals[i] > 
            col2->vals[j]){
                dis_pairs++;
            } 
        }
    }
    //calculate tau coefficient
    tau = (2 * (con_pairs - dis_pairs)) / (double)(nrows * (nrows - 1));
   
    printf("tau coefficient between %s and %s = %7.2f\n", col1->label, 
    col2->label, tau);
    
    return;
}

/****************************************************************/
/*From assignment 1*/
/*Order elements in array from smallest to largest*/
void
small_to_big(double arr[], int size){
    int i, j;
    for (i = 0; i < size; i++){ 
        for (j = i + 1; j < size; j++){
            if (arr[i] > arr[j]){ 
                dub_swap(&arr[j], &arr[i]);
            }                
        }              
    }
}
/********************************************************************/
/*Modified from small_to_big function*/
/*Order elements in array from largest to smallest*/
void
big_to_small(double arr[], int size){
    int i, j;
    for (i = 0; i < size; i++){ 
        for (j = i + 1; j < size; j++){
            if (arr[i] < arr[j]){ 
                dub_swap(&arr[j], &arr[i]);
            }
                            
         }
                        
     }
}
/*From assignment 1*/
/*Swap two doubles*/
void
dub_swap(double *p1, double *p2) {
    double tmp;
    tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

/********************************************************************/
/*From assignment 1*/
/*prints out array*/
void
print_array(double arr[], int size){ 
    int i; 
    for (i = 0; i < size; i++){
        printf(" %0.1lf", arr[i]); 
    } 
    printf("\n");
} 

/*******************************************/
/* From Grok exercise 7.05*/
void
sort_int_arrays(double stud_nos[], double marks[], int n) {
	int i, j;
	/* assume that A[0] to A[n-1] have valid values */
	for (i=1; i<n; i++) {
		/* swap A[i] left into correct position, where
		   "correct" is defined by *decreasing* order */
		for (j=i-1; j>=0 && stud_nos[j+1]<stud_nos[j]; j--) {
				       /* ^ the critical change */
			/* not there yet */
			dub_swap(&stud_nos[j], &stud_nos[j+1]);
			dub_swap(&marks[j], &marks[j+1]);
		}
	}
	/* and that's all there is to it! */
}

/************************************************/
/*For rows with the same location, find the average value*/
void
sort_avg(double *temp, double vals[], double location[], int n,
int *counter, int *start){
    int i;

    for (i=0; i<n; i++) {	
        //check if location row[start] is equal to another location
        if(location[*start] == location[i]){
            *temp = *temp + vals[i];
            *counter = *counter + 1;
            
        }    
    }
    *temp = *temp / *counter;
    *start = *start + *counter;
}
/*********************************************************/
//remove duplicate elements within an array//
void 
removeDuplicates(double arr[], int *size, int *unique) {
    int i, j, k;
    *unique = *size;
    // Loop through each element of the array
    for (i = 0; i < *size; ++i) {
        // Check if the current element is duplicate of any previous elements
        for (j = i + 1; j < *size;) {
            if (arr[j] == arr[i]) {
                // If duplicate found, shift elements to left
                for (k = j; k < *size; ++k) {
                    arr[k] = arr[k + 1];
                }
                // Decrease size of array
                (*size)--;
                (*unique)--;
            } else {
                // Move to next element
                ++j;
            }
        }
    }
}

/********************************************************/
//copy array  to another temporary array//
void
copy_array(double temp[], double array[], int width){
    int i;
    for(i = 0; i < width; i++){
        temp[i] = array[i];     
    }
}


/*************************************************************/
//calculate scaling factor
void
scale_graph(int array[], int width, int *scale){
    int temp[width], i = 0, val;
    copy_array_int(temp, array, width);
    big_to_small_int(temp, width);
    
    while(i < MAXSCALE){
    val = temp[0] / pow(2, i);
        if(val < 50){
            *scale = pow(2,i);
            break;
        }
        else{
            i++;
        }
    } 
}

/*******************************************************************/
//from big_to_small
void
big_to_small_int(int arr[], int size){
    int i, j;
    for (i = 0; i < size; i++){ 
        for (j = i + 1; j < size; j++){
            if (arr[i] < arr[j]){ 
                int_swap(&arr[j], &arr[i]);
            }
                            
         }
                        
     }
}
/****************************************************************/
//from dub_swap
void
int_swap(int *p1, int *p2) {
    int tmp;
    tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}
/*******************************************************************/
//from copy_array
void
copy_array_int(int temp[], int array[], int width){
    int i;
    for(i = 0; i < width; i++){
        temp[i] = array[i];     
    }
}





