#!/bin/bash
rm -f task_file simulation_log
./task_file_randomiser
./scheduler task_file 1
