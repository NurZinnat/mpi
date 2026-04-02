# Компилятор Intel MPI

# Источник окружения Intel MPI
INTEL_MPI_SETUP = source /opt/intel/oneapi/mpi/2021.17/env/vars.sh 2>/dev/null || true

run-debug: $(DEBUG_TARGET)
	. /opt/intel/oneapi/mpi/2021.17/env/vars.sh && \
	I_MPI_OFI_LIBRARY_INTERNAL=1 \
	$(MPIRUN) -np 4 ./$(DEBUG_TARGET)

CC = g++
MPICXX = /opt/intel/oneapi/mpi/2021.17/bin/mpicxx
MPIRUN = /opt/intel/oneapi/mpi/2021.17/bin/mpirun

# Пути Intel MPI
MPI_INCLUDE = -I/opt/intel/oneapi/mpi/2021.17/include
MPI_LIB = -L/opt/intel/oneapi/mpi/2021.17/lib/release -lmpi

# Флаги компиляции (Релиз)
FLAGS = -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format
OPT = -O3

# Флаги компиляции (Отладка)
DEBUG_FLAGS = -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format
DEBUG_OPT = -O0

# Цели
TTT = a.out
DEBUG_TARGET = debug.out

# Список объектов
OBJS = main.o application.o block.o cmd_arg_parser.o data.o matrix_part.o mpi_communicator.o m_sizes.o precision.o reflection_vectors.o reflection_vectors_storage.o rv_sizes.o simple_functions.o application_main.o
OBJS_DEBUG = main_debug.o application_debug.o block_debug.o cmd_arg_parser_debug.o data_debug.o matrix_part_debug.o mpi_communicator_debug.o m_sizes_debug.o precision_debug.o reflection_vectors_debug.o reflection_vectors_storage_debug.o rv_sizes_debug.o simple_functions_debug.o application_main_debug.o

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
	$(CC) -c $(OPT) $(FLAGS) $(MPI_INCLUDE) main.cpp

application.o: application.cpp application.h
	$(CC) -c $(OPT) $(FLAGS) $(MPI_INCLUDE) application.cpp

block.o: block.cpp block.h execution_status.h
	$(CC) -c $(OPT) $(FLAGS) $(MPI_INCLUDE) block.cpp

cmd_arg_parser.o: cmd_arg_parser.cpp cmd_arg_parser.h
	$(CC) -c $(OPT) $(FLAGS) $(MPI_INCLUDE) cmd_arg_parser.cpp

data.o: data.cpp data.h execution_status.h
	$(CC) -c $(OPT) $(FLAGS) $(MPI_INCLUDE) data.cpp

matrix_part.o: matrix_part.cpp matrix_part.h matrix_type.h execution_status.h
	$(CC) -c $(OPT) $(FLAGS) $(MPI_INCLUDE) matrix_part.cpp

mpi_communicator.o: mpi_communicator.cpp mpi_communicator.h
	$(CC) -c $(OPT) $(FLAGS) $(MPI_INCLUDE) mpi_communicator.cpp

m_sizes.o: m_sizes.cpp m_sizes.h
	$(CC) -c $(OPT) $(FLAGS) $(MPI_INCLUDE) m_sizes.cpp

precision.o: precision.cpp precision.h
	$(CC) -c $(OPT) $(FLAGS) $(MPI_INCLUDE) precision.cpp

reflection_vectors.o: reflection_vectors.cpp reflection_vectors.h execution_status.h
	$(CC) -c $(OPT) $(FLAGS) $(MPI_INCLUDE) reflection_vectors.cpp

reflection_vectors_storage.o: reflection_vectors_storage.cpp reflection_vectors.h
	$(CC) -c $(OPT) $(FLAGS) $(MPI_INCLUDE) reflection_vectors_storage.cpp

rv_sizes.o: rv_sizes.cpp rv_sizes.h
	$(CC) -c $(OPT) $(FLAGS) $(MPI_INCLUDE) rv_sizes.cpp

simple_functions.o: simple_functions.cpp simple_functions.h
	$(CC) -c $(OPT) $(FLAGS) $(MPI_INCLUDE) simple_functions.cpp

application_main.o: application_main.cpp application.h
	$(CC) -c $(OPT) $(FLAGS) $(MPI_INCLUDE) application_main.cpp

# Правила для отладочных объектных файлов
main_debug.o: main.cpp
	$(CC) -c $(DEBUG_OPT) $(DEBUG_FLAGS) $(MPI_INCLUDE) main.cpp -o main_debug.o

application_debug.o: application.cpp application.h
	$(CC) -c $(DEBUG_OPT) $(DEBUG_FLAGS) $(MPI_INCLUDE) application.cpp -o application_debug.o

block_debug.o: block.cpp block.h execution_status.h
	$(CC) -c $(DEBUG_OPT) $(DEBUG_FLAGS) $(MPI_INCLUDE) block.cpp -o block_debug.o

cmd_arg_parser_debug.o: cmd_arg_parser.cpp cmd_arg_parser.h
	$(CC) -c $(DEBUG_OPT) $(DEBUG_FLAGS) $(MPI_INCLUDE) cmd_arg_parser.cpp -o cmd_arg_parser_debug.o

data_debug.o: data.cpp data.h execution_status.h
	$(CC) -c $(DEBUG_OPT) $(DEBUG_FLAGS) $(MPI_INCLUDE) data.cpp -o data_debug.o

matrix_part_debug.o: matrix_part.cpp matrix_part.h matrix_type.h execution_status.h
	$(CC) -c $(DEBUG_OPT) $(DEBUG_FLAGS) $(MPI_INCLUDE) matrix_part.cpp -o matrix_part_debug.o

mpi_communicator_debug.o: mpi_communicator.cpp mpi_communicator.h
	$(CC) -c $(DEBUG_OPT) $(DEBUG_FLAGS) $(MPI_INCLUDE) mpi_communicator.cpp -o mpi_communicator_debug.o

m_sizes_debug.o: m_sizes.cpp m_sizes.h
	$(CC) -c $(DEBUG_OPT) $(DEBUG_FLAGS) $(MPI_INCLUDE) m_sizes.cpp -o m_sizes_debug.o

precision_debug.o: precision.cpp precision.h
	$(CC) -c $(DEBUG_OPT) $(DEBUG_FLAGS) $(MPI_INCLUDE) precision.cpp -o precision_debug.o

reflection_vectors_debug.o: reflection_vectors.cpp reflection_vectors.h execution_status.h
	$(CC) -c $(DEBUG_OPT) $(DEBUG_FLAGS) $(MPI_INCLUDE) reflection_vectors.cpp -o reflection_vectors_debug.o

reflection_vectors_storage_debug.o: reflection_vectors_storage.cpp reflection_vectors.h
	$(CC) -c $(DEBUG_OPT) $(DEBUG_FLAGS) $(MPI_INCLUDE) reflection_vectors_storage.cpp -o reflection_vectors_storage_debug.o

rv_sizes_debug.o: rv_sizes.cpp rv_sizes.h
	$(CC) -c $(DEBUG_OPT) $(DEBUG_FLAGS) $(MPI_INCLUDE) rv_sizes.cpp -o rv_sizes_debug.o

simple_functions_debug.o: simple_functions.cpp simple_functions.h
	$(CC) -c $(DEBUG_OPT) $(DEBUG_FLAGS) $(MPI_INCLUDE) simple_functions.cpp -o simple_functions_debug.o

application_main_debug.o: application_main.cpp application.h
	$(CC) -c $(DEBUG_OPT) $(DEBUG_FLAGS) $(MPI_INCLUDE) application_main.cpp -o application_main_debug.o

clean:
	rm -rf *.o $(TTT) $(DEBUG_TARGET) *.d

run: $(TTT)
	$(MPIRUN) -np 4 ./$(TTT)

run-debug: $(DEBUG_TARGET)
	$(MPIRUN) -np 4 ./$(DEBUG_TARGET)

# Отладка в gdb (открывает отдельное окно xterm)
gdb: $(DEBUG_TARGET)
	. /opt/intel/oneapi/mpi/2021.17/env/vars.sh && \
	I_MPI_FABRICS=shm \
	I_MPI_OFI_LIBRARY_INTERNAL=1 \
	$(MPIRUN) -np 1 xterm -e gdb --args ./$(DEBUG_TARGET) 10 3 10 3

# Отладка в gdb (текущий терминал, без xterm)
gdb-here: $(DEBUG_TARGET)
	. /opt/intel/oneapi/mpi/2021.17/env/vars.sh && \
	I_MPI_FABRICS=shm \
	I_MPI_OFI_LIBRARY_INTERNAL=1 \
	$(MPIRUN) -np 1 gdb --args ./$(DEBUG_TARGET) 10 3 10 3

.PHONY: all debug clean run run-debug