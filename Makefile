# Компилятор Intel MPI
CC = g++
MPICXX = /opt/intel/oneapi/mpi/2021.17/bin/mpicxx
MPIRUN = /opt/intel/oneapi/mpi/2021.17/bin/mpirun

# Пути Intel MPI
MPI_INCLUDE = -I/opt/intel/oneapi/mpi/2021.17/include
MPI_LIB = -L/opt/intel/oneapi/mpi/2021.17/lib/release -lmpi

# Переменные для запуска (можно переопределять в командной строке)
PROC ?= 4
ARGS ?=

# Переменные окружения для стабильной работы Intel MPI
MPI_ENV = . /opt/intel/oneapi/mpi/2021.17/env/vars.sh && \
          I_MPI_FABRICS=shm \
          I_MPI_OFI_LIBRARY_INTERNAL=1

# Флаги компиляции (Релиз)
FLAGS = -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format
OPT = -O3

# Флаги компиляции (Отладка)
DEBUG_FLAGS = -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format
DEBUG_OPT = -O0

# Короткие переменные для компиляции
COMPILE = $(CC) -c $(OPT) $(FLAGS) $(MPI_INCLUDE)
COMPILE_DEBUG = $(CC) -c $(DEBUG_OPT) $(DEBUG_FLAGS) $(MPI_INCLUDE)

# Цели
TTT = a.out
DEBUG_TARGET = debug.out

# Список объектов
OBJS = main.o application.o block_string.o cmd_arg_parser.o data.o data_view.o discrepancy.o group_view.o householder_logic.o matrix_part.o mpi_group.o mpi_message.o multiply_strings.o m_sizes.o precision.o reflection_vectors.o rv_sizes.o simple_functions.o application_main.o block_sizes.o block_view.o
OBJS_DEBUG = main_debug.o application_debug.o block_string_debug.o cmd_arg_parser_debug.o data_debug.o data_view_debug.o discrepancy_debug.o group_view_debug.o householder_logic_debug.o matrix_part_debug.o mpi_group_debug.o mpi_message_debug.o multiply_strings_debug.o m_sizes_debug.o precision_debug.o reflection_vectors_debug.o rv_sizes_debug.o simple_functions_debug.o application_main_debug.o block_sizes_debug.o block_view_debug.o

# Цель по умолчанию - релизная сборка
all: $(TTT)

# Отладочная сборка
debug: $(DEBUG_TARGET)

# Релизная сборка
$(TTT): $(OBJS)
	$(MPICXX) $(OPT) $(FLAGS) $(OBJS) $(MPI_LIB) -o $(TTT)

# Отладочная сборка
$(DEBUG_TARGET): $(OBJS_DEBUG)
	$(MPICXX) $(DEBUG_OPT) $(DEBUG_FLAGS) $(OBJS_DEBUG) $(MPI_LIB) -o $(DEBUG_TARGET)

# Правила для релизных объектных файлов
main.o: main.cpp
	$(COMPILE) main.cpp

application.o: application.cpp application.h
	$(COMPILE) application.cpp

block_string.o: block_string.cpp block_string.h
	$(COMPILE) block_string.cpp

cmd_arg_parser.o: cmd_arg_parser.cpp cmd_arg_parser.h
	$(COMPILE) cmd_arg_parser.cpp

data.o: data.cpp data.h execution_status.h
	$(COMPILE) data.cpp

data_view.o: data_view.cpp data_view.h
	$(COMPILE) data_view.cpp

discrepancy.o: discrepancy.cpp application.h
	$(COMPILE) discrepancy.cpp

group_view.o: group_view.cpp group_view.h
	$(COMPILE) group_view.cpp

householder_logic.o: householder_logic.cpp application.h
	$(COMPILE) householder_logic.cpp

matrix_part.o: matrix_part.cpp matrix_part.h matrix_type.h execution_status.h
	$(COMPILE) matrix_part.cpp

mpi_group.o: mpi_group.cpp mpi_group.h
	$(COMPILE) mpi_group.cpp

mpi_message.o: mpi_message.cpp mpi_message.h
	$(COMPILE) mpi_message.cpp

multiply_strings.o: multiply_strings.cpp application.h
	$(COMPILE) multiply_strings.cpp

m_sizes.o: m_sizes.cpp m_sizes.h
	$(COMPILE) m_sizes.cpp

precision.o: precision.cpp precision.h
	$(COMPILE) precision.cpp

reflection_vectors.o: reflection_vectors.cpp reflection_vectors.h execution_status.h
	$(COMPILE) reflection_vectors.cpp

rv_sizes.o: rv_sizes.cpp rv_sizes.h
	$(COMPILE) rv_sizes.cpp

simple_functions.o: simple_functions.cpp simple_functions.h
	$(COMPILE) simple_functions.cpp

application_main.o: application_main.cpp application.h
	$(COMPILE) application_main.cpp

block_sizes.o: block_sizes.cpp block_sizes.h
	$(COMPILE) block_sizes.cpp

block_view.o: block_view.cpp block_view.h
	$(COMPILE) block_view.cpp

# Правила для отладочных объектных файлов
main_debug.o: main.cpp
	$(COMPILE_DEBUG) main.cpp -o main_debug.o

application_debug.o: application.cpp application.h
	$(COMPILE_DEBUG) application.cpp -o application_debug.o

block_string_debug.o: block_string.cpp block_string.h
	$(COMPILE_DEBUG) block_string.cpp -o block_string_debug.o

cmd_arg_parser_debug.o: cmd_arg_parser.cpp cmd_arg_parser.h
	$(COMPILE_DEBUG) cmd_arg_parser.cpp -o cmd_arg_parser_debug.o

data_debug.o: data.cpp data.h execution_status.h
	$(COMPILE_DEBUG) data.cpp -o data_debug.o

data_view_debug.o: data_view.cpp data_view.h
	$(COMPILE_DEBUG) data_view.cpp -o data_view_debug.o

discrepancy_debug.o: discrepancy.cpp application.h
	$(COMPILE_DEBUG) discrepancy.cpp -o discrepancy_debug.o

group_view_debug.o: group_view.cpp group_view.h
	$(COMPILE_DEBUG) group_view.cpp -o group_view_debug.o

householder_logic_debug.o: householder_logic.cpp application.h
	$(COMPILE_DEBUG) householder_logic.cpp -o householder_logic_debug.o

matrix_part_debug.o: matrix_part.cpp matrix_part.h matrix_type.h execution_status.h
	$(COMPILE_DEBUG) matrix_part.cpp -o matrix_part_debug.o

mpi_group_debug.o: mpi_group.cpp mpi_group.h
	$(COMPILE_DEBUG) mpi_group.cpp -o mpi_group_debug.o

mpi_message_debug.o: mpi_message.cpp mpi_message.h
	$(COMPILE_DEBUG) mpi_message.cpp -o mpi_message_debug.o

multiply_strings_debug.o: multiply_strings.cpp application.h
	$(COMPILE_DEBUG) multiply_strings.cpp -o multiply_strings_debug.o

m_sizes_debug.o: m_sizes.cpp m_sizes.h
	$(COMPILE_DEBUG) m_sizes.cpp -o m_sizes_debug.o

precision_debug.o: precision.cpp precision.h
	$(COMPILE_DEBUG) precision.cpp -o precision_debug.o

reflection_vectors_debug.o: reflection_vectors.cpp reflection_vectors.h execution_status.h
	$(COMPILE_DEBUG) reflection_vectors.cpp -o reflection_vectors_debug.o

rv_sizes_debug.o: rv_sizes.cpp rv_sizes.h
	$(COMPILE_DEBUG) rv_sizes.cpp -o rv_sizes_debug.o

simple_functions_debug.o: simple_functions.cpp simple_functions.h
	$(COMPILE_DEBUG) simple_functions.cpp -o simple_functions_debug.o

application_main_debug.o: application_main.cpp application.h
	$(COMPILE_DEBUG) application_main.cpp -o application_main_debug.o

block_sizes_debug.o: block_sizes.cpp block_sizes.h
	$(COMPILE_DEBUG) block_sizes.cpp -o block_sizes_debug.o

block_view_debug.o: block_view.cpp block_view.h
	$(COMPILE_DEBUG) block_view.cpp -o block_view_debug.o

clean:
	rm -rf *.o $(TTT) $(DEBUG_TARGET) *.d

# Запуск релизной версии (с переменными PROC и ARGS)
run-release: $(TTT)
	$(MPI_ENV) $(MPIRUN) -np $(PROC) ./$(TTT) $(ARGS)

# Запуск отладочной версии (с переменными PROC и ARGS)
run-debug: $(DEBUG_TARGET)
	$(MPI_ENV) $(MPIRUN) -np $(PROC) ./$(DEBUG_TARGET) $(ARGS)

# Отладка в gdb (открывает отдельное окно xterm)
gdb: $(DEBUG_TARGET)
	$(MPI_ENV) $(MPIRUN) -np $(PROC) xterm -e gdb --args ./$(DEBUG_TARGET) $(ARGS)

# Отладка в gdb (текущий терминал, без xterm)
gdb-here: $(DEBUG_TARGET)
	$(MPI_ENV) $(MPIRUN) -np $(PROC) gdb --args ./$(DEBUG_TARGET) $(ARGS)
	
# Запуск под Valgrind (проверка утечек памяти и ошибок)
valgrind: $(DEBUG_TARGET)
	$(MPI_ENV) $(MPIRUN) -np $(PROC) valgrind --leak-check=full --track-origins=yes ./$(DEBUG_TARGET) $(ARGS)

# Запуск под Valgrind с выводом в файл
valgrind-log: $(DEBUG_TARGET)
	$(MPI_ENV) $(MPIRUN) -np $(PROC) valgrind --leak-check=full --track-origins=yes --log-file=valgrind_%p.log ./$(DEBUG_TARGET) $(ARGS)

.PHONY: all debug clean run-release run-debug gdb gdb-here valgrind valgrind-log