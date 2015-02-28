 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*	MODELING AFTERSHOCKS IN EARTHQUAKES USING LOCAL VISCOSITY							*/
/*		MATTHEW PROCTOR																	*/
/*																						*/
/*	THIS MODEL IS N MASSES OSCILLATING BETWEEN TWO FIXED								*/
/*	BOUNDARY POSITIONS.																	*/
/*																						*/
/*	INPUTS: NUMBER OF TIMESTEPS, NUMBER OF BLOCKS, FRICTION CONSTANT					*/
/*																						*/
/*	OUTPUTS: TIMESTEP, POSITION, VELOCITY, ACCELERATION									*/
/*																						*/
/*	DATE: 10/10/2011																	*/
/*																						*/
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define EXPECTED_ARGS 5

int velStep(int v_i, int a, int t);
void acceleration(int x_p, int *a, int *k, int *x, int n, int p, int *v_i, int *T, int sigma);
int friction(int v_i, int F_0);
int xPos(int x, int v_i, int v, int t); 
void wtf(FILE *output, int t, int x, int v, int a, int j);

int main(int argc, char* argv[]){
//	VARIABLE DECLARATION
	int valid_input=1; //validation
	int i, j, n; // tick counters
	int t, x, *v, *a, *v_i; //instantaneous outputs
	int *k, *x; //inputs
	int x_p, v_p; //previous time-step
	int sigma, p, *T, F_0, tstep=1;	//need to implement a variable timestep based on the cycle
	char str1[FILENAME_MAX];
	const char base[] = ".out";`
	
//	FILE POINTER DECLARATION
	FILE* output, input1, input2;

//	DECLARE FILENAMES
	input1 = fopen("k.dat", "r");
	input2 = fopen("x.dat", "r");

//	VALIDATE THE FILES
	if (input1 == (FILE*) NULL){ //If the file cannot be found/created/opened then the program exits
		printf("The k input file could not be found. Apologies...\n");
		valid_input = 0;
	}
	if (input2 == (FILE*) NULL){ //If the file cannot be found/created/opened then the program exits
		printf("The x input file could not be found. Apologies...\n");
		valid_input = 0;
	}
	if(argc != EXPECTED_ARGS){ //Implement a menu system to select variables if they are missing at runtime
		printf("Time Step/Number of masses expected.\nPlease try again...\n");
		valid_input = 0;
	} else { // VALIDATE THE COMMAND LINE ARGUEMENTS
		valid_input = valid_input && sscanf(argv[1], "%d", &t);
		valid_input = valid_input && sscanf(argv[2], "%d", &n);
		valid_input = valid_input && sscanf(argv[3], "%d", &p);
		valid_input = valid_input && sscanf(argv[4], "%d", &v_p);
		
		valid_input = valid_input && (t > 0);
		valid_input = valid_input && (n > 0);
		valid_input = valid_input && (p >= 0);
	}
	
// Sets the filename to the form of "data#timesteps-#block-frictional force.out"	
	sprintf(str1, "data%d-%d-%d%s", (int)t, (int)n, p, base);
	output = fopen(str1, "w+");
	if (output == (FILE*) NULL){ //If the file cannot be found/created/opened then the program exits
		printf("The output file could not be found/created. Apologies...\n");
		valid_input = 0;
	}1
	if(valid_input != 1) {
		printf("Input Validation Failed.\nExiting...\n");
		return(EXIT_FAILURE);
	} else {		
	
		k=(int*)malloc((n+2)*sizeof(int));
		for(j=0; j<n+2; j++) {
			fscanf(input1, "%d", &k[j]);
		}
		fclose(input1);
		
		x=(int*)calloc(n+1,sizeof(int));
		x[0]=0;
		for(j=1; j<n+1; j++) {
			fscanf(input2, "%d", &x[j]);
		}
		fclose(input2);	
		
		a = (int*) calloc(n+1,sizeof(int)); 
		v = (int*) calloc(n+1,sizeof(int));
		v_i = (int*) calloc(n+1,sizeof(int));
		T = (int*) calloc(n+2, sizeof(int));
		sigma = 10;
// to ensure we write from the start of the file
		rewind(output);

		for(i=0;i<t;i++) {	// This for loop runs until the time step given in command input has occured	
			x_p = v_p*t;
			acceleration(x_p, a, k, x, n, p, v_i, T, sigma);		
			for(j=1; j<n+1; j++){	//this for loop runs for n blocks 
				wtf(output, (int)(i*tstep), x[j], v_i[j], a[j], j);
				v[j] = velStep(v_i[j], a[j], tstep);
				x[j] = xPos(x[j], v_i[j], v[j], tstep);
				v_i[j] = v[j];
			}
			printf("%d\n",(i+1));
		}
//	CLOSE ALL THE FILES and all the malloc arrays too!!
		fclose(output);	//Close off the output files
		printf("All files closed and program is a success\nYAY!\n");		//Declare victory
		return(EXIT_SUCCESS);
	}
}

void wtf(FILE *output, double t, double x, double v, double a, int j){
// This function writes the current iteration of the variables to a comma delimited file
//	INTERNAL VARIABLE DECLARATION
	int k;
//	METHOD	
	k = fprintf(output, "%d %d %d %d %d\n", j, t, x, v, a); 
}


void acceleration(int x_p, int *a, int *k, int *x, int n, int p, int *v_i, int *T, int sigma){
//	This function is to calculate the instantaneous acceleration of the block
//	INTERNAL VARIABLE DECLARATION	
	int *F, i, dx_p;
//	METHOD
	F = (int*) calloc(n+2, sizeof(int));
	T[0] = 0;
	T[n] = 0;
	for (i=1; i<n; i++){
		T[i] = k[i]*(x[i+1]-x[i]);
	}
	for (i=1; i<n+1; i++){
		dx_p = x_p-x[i];
		F[i] = (T[i] - T[i-1])+(p*dx_p);
		if (v_i[i]==0 && F[i]<=sigma){
			a[i]=0;
		}
		else if ( v_i[i]==0 && F[i]>sigma){
			a[i]= F[i]-sigma;
		}
		else {
			a[i]= F[i]-(sigma/1+v_i[i]);
		}
	}
}

int velStep(int v_i, int a, int t){	
//	This function finds the step in velocity from the previous velocity and the current acceleration
//	INTERNAL VARIABLE DECLARATON
	int velout;
//	METHOD	
	velout = v_i + (a*t);	
//	RETURN STATEMENT
	return(velout);
}

int xPos(int x, int v_i, int v, int t){
//	This function calculates the change in position of the block
//	INTERNAL VARIABLE DECLARATION
	int s;
//	METHOD
	s = x + (v*t);	
//	RETURN STATEMENT
	return(s);
}