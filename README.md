# Producer_Consumer_Program
PROGRAMMING ASSIGNMENT 3
CSC 341, SP 2016
03/14/16

GROUP
	Nolan Thompson
	Jarrett Dixon

CONTRIBUTIONS
	Developed as group.

DESCRIPTION
	A course assignment which implements semaphores and a mutex lock to
	prevent race conditions between processes adding to and removing 
	data from a shared buffer.	
	
	The program generates 5 producer and 5 consumer processes which,
	respectively, attempt to add and remove items from a fixed array.
 
	
INSTRUCTIONS
	To run the program buffer.c in linux, enter the following command
	in the directory containing the file to compile the program:
	
		gcc -pthread buffer.c

	Following compilation, enter the following command to run the
	program with 5 producer and 5 consumer threads:
		
		./a.out 20 5 5 > output.txt

	This will write the program output to the file output.txt.

EXTRA CREDIT
	1)  The program prints the id of each thread process when and action
		is performed using the function pthread_self().

	2)	The program uses an added variable buffer_time to track the time
		elapsed from when a process attempts to access the buffer, and
		when the process is successful.  Variable buffer_time is a
		counter that increments every time the buffer is accessed. The
		individual processes compare the buffer_time prior to attempting
		to access the buffer to the buffer_time after returning from
		completing the process to track the total time taken.

OBSERVATIONS
	The program will write a line each time a process attempts to
	modify the buffer.  This output will sometimes appear inter-
	spersed within output statements from other processes.  This is
	due to the processes running concurrently, resulting in the
	scheduling of the write tasks for each output line.
