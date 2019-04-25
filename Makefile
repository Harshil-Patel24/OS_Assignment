CC = gcc
CFLAGS = -Wall -ansi -pedantic
OBJ_TASK_FILE = task_file_randomiser.o
OBJ = file_io.o linked_list.o scheduler.o
EXEC_TASK_FILE = task_file_randomiser
EXEC = scheduler
EXEC_ALL = $(EXEC_TASK_FILE) $(EXEC)

ALL: $(EXEC_ALL)

$(EXEC_TASK_FILE): clean $(OBJ_TASK_FILE)
	$(CC) $(OBJ_TASK_FILE) -o $(EXEC_TASK_FILE)

$(EXEC): clean $(OBJ)
	$(CC) $(OBJ_TASK_FILE) -o $(EXEC)

task_file_randomiser.o: task_file_randomiser.c task_file_randomiser.h
	 $(CC) -c task_file_randomiser.c $(CFLAGS)

clean:
	rm -f $(OBJ_TASK_FILE) $(EXEC) task_file
