CC = gcc
CFLAGS = -Wall -ansi -pedantic 
OBJ = task_file_randomiser.o
EXEC = task_file_randomiser

$(EXEC): clean $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

task_file_randomiser.o: task_file_randomiser.c task_file_randomiser.h
	 $(CC) -c task_file_randomiser.c $(CFLAGS)

clean:
	rm -f $(OBJ) $(EXEC) task_file
