#include<pthread.h>  //The include file <pthread.h> contains declarations for the Pthreads functions and is required if your program uses Pthreads.
#include<stdlib.h>  //The stdlib.h header defines four variable types, several macros, and various functions for performing general functions.
#include<unistd.h>  //is the name of the header file that provides access to the POSIX operating system API.
#include<stdio.h>   //The header file which is added to the program that one has written is basically what 'include stdio.h.' means.
#include<string.h>  //string.h is the header in the C standard library for the C programming language which contains macro definitions, constants and declarations of 				functions and types used not only for string handling but also various memory handling functions.
#include<semaphore.h>  //The <semaphore.h> header shall define the sem_t type, used in performing semaphore operations. The semaphore may be implemented using a file 				descriptor, in which case applications are able to open up at least a total of {OPEN_MAX} files and semaphores. 
#include <time.h>    //The time.h header defines four variable types, two macro and various functions for manipulating date and time.

//We have defined the default variables you have given us in the assignment file.
#define CUSTOMER_NUMBER 25 //The number of customers
#define CUSTOMER_ARRIVAL_TIME_MIN 1  //Minimum waiting time for customer.
#define CUSTOMER_ARRIVAL_TIME_MAX 3  //Maximum waiting time for customer.
#define REGISTER_NUMBER 5            //The definition above will be used for how many registers your program should work for.This will also be used to identify registers 			by given them IDs from 0 to 4.
#define COFFEE_TIME_MIN 2	     //This represents that minimum waiting time should be 2 second.
#define COFFEE_TIME_MAX 5            //This represents that minimum waiting time should be 1 second.
sem_t semaphore;
pthread_t tid[CUSTOMER_NUMBER];
int generateRandom(int lower, int upper)//random number generator function
{
    int num = (rand() %
    (upper - lower + 1)) + lower;
    return num; 		//return the random number 
}
int reg[4][1]={{0,0},{0,0},{0,0},{0,0},{0,0}};//2D array, The first coulmn of the array shows the register number and the second coulmn shows it is used or not.
void* Coffee(void *arg) //coffee purchase transaction 
{
	int a =0;
	
	int reg_id;	//register id 
	int b=0; //This integer we assign to check the register id
	for (a = 0; a < REGISTER_NUMBER-1; a++)
	//this loop chooses registers with random numbers because customers can buy coffee from any register they want as long as it is empty.
	{
		reg_id=generateRandom(0,REGISTER_NUMBER-1);
		if(reg[reg_id][1]==0)//if random number as equal to reg[a][0]and reg[a][0] is not used we can use this
		{
			b=reg_id;
			reg[a][1]=1;//flag is 1
			break ;
	
		}
	}
	
	
	int id = (int)arg;
	int value;
	time_t start;//start time
	start=time(NULL);
	sem_wait(&semaphore);//The sem_wait() function decrements by one the value of the semaphore. The semaphore will be decremented when its value is greater than zero.
	sleep(generateRandom(COFFEE_TIME_MIN,COFFEE_TIME_MAX));//we call random generator between "COFFEE_TIME_MIN,COFFEE_TIME_MAX"
	/* START CRITICAL REGION */
	sem_getvalue(&semaphore, &value);//The sem_getvalue() function retrieves the value of a named or unnamed semaphore. If the current value of the semaphore is zero 							and there are threads waiting on the semaphore, a negative value is returned.	
	printf("CUSTOMER <%d> GOES TO REGISTER <%d>.\n", id,reg_id);// print the customer number and the register id
	reg[a][1]=0;//flag is 0 if we want to use this register we can use this.
	unsigned long i = 0;
	for (i = 0; i < 100000000; i++);
	/* END CRITICAL REGION */
	
	time_t finish;//end time
	finish=time(NULL);
	sem_post(&semaphore);
	printf("CUSTOMER <%d> FINISHED BUYING FROM REGISTER <%d> AFTER <%d> SECONDS.\n", id,reg_id,(finish - start));//customer finished purchasing coffee here
	printf("Thread %d: Semaphore's value is: %d\n", id, value);//semaphore value count 
	pthread_exit(NULL);
}

int main(void) {
	int i = 0, j = 0;
	int err;
	int value;
	sem_init(&semaphore, 0, REGISTER_NUMBER);//The sem_init() function initializes an unnamed semaphore and sets its initial value. The maximum value of the semaphore 							is set to SEM_VALUE_MAX. The title for the semaphore is set to the character representation of the address of the 							semaphore. If an unnamed semaphore already exists at sem, then it will be destroyed and a new semaphore will be initialized.
	
	while (i < CUSTOMER_NUMBER) //while loop returns as many customers
	{
	int a=generateRandom(CUSTOMER_ARRIVAL_TIME_MIN,CUSTOMER_ARRIVAL_TIME_MAX);//we call random generator between "CUSTOMER_ARRIVAL_TIME_MIN,CUSTOMER_ARRIVAL_TIME_MAX"
	sleep(a);
		err = pthread_create(&(tid[i]), NULL, &Coffee, (void *)(i));
		//customer starts purchasing coffee here 
		if (err != 0)
			printf("Thread creation error: [%s]",strerror(err));     //error message
		printf("CUSTOMER <%d> IS CREATED AFTER <%d> SECONDS.\n",i,a);    //print the customer id , created after random between (1,3) seconds
		
		i++;
		
	}
	
	while (j < CUSTOMER_NUMBER) {
		pthread_join(tid[j], NULL);
		j++;
	}
	
	sem_getvalue(&semaphore, &value);//The sem_getvalue() function retrieves the value of a named or unnamed semaphore. If the current value of the semaphore is zero 						and there are threads waiting on the semaphore, a negative value is returned.
					//sem (Input) A pointer to an initialized unnamed semaphore or an opened named semaphore.
					//value (Output) A pointer to the integer that contains the value of the semaphore
	
	printf("\n");
	
	sem_destroy(&semaphore);//The sem_destroy() function destroys an unnamed semaphore that was previously initialized using sem_init() or sem_init_np(). Any threads 					that have blocked from calling sem_wait() or sem_wait_np() on the semaphore will unblock and return an [EINVAL] or [EDESTROYED] error.
	return 0;
}
