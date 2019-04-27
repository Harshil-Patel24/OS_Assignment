CC = gcc
CFLAGS = -Wall -ansi -pedantic -pthread
OBJ_TASK_FILE = task_file_randomiser.o
OBJ = file_io.o linked_list.o scheduler.o
EXEC_TASK_FILE = task_file_randomiser
EXEC = scheduler
EXEC_ALL = $(EXEC_TASK_FILE) $(EXEC)
LIBS = -pthread

ALL: $(EXEC_ALL)

$(EXEC_TASK_FILE): clean $(OBJ_TASK_FILE)
	$(CC) $(OBJ_TASK_FILE) -o $(EXEC_TASK_FILE)

$(EXEC): clean $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LIBS)

task_file_randomiser.o: task_file_randomiser.c task_file_randomiser.h
	$(CC) -c task_file_randomiser.c $(CFLAGS)

file_io.o: file_io.h file_io.c
	$(CC) -c file_io.c $(CFLAGS)

linked_list.o: linked_list.c linked_list.h
	$(CC) -c linked_list.c $(CFLAGS)

scheduler.o: scheduler.c scheduler.h file_io.h linked_list.h
	$(CC) -c scheduler.c $(CFLAGS)

clean:
	rm -f $(OBJ) $(OBJ_TASK_FILE) $(EXEC) $(EXEC_TASK_FILE) task_file
