# kompilator c
#CCOMP = /usr/lib64/openmpi/bin/mpicc
CCOMP = mpicc

# konsolidator
#LOADER = /usr/lib64/openmpi/bin/mpicc
LOADER = mpicc

# program uruchomieniowy MPI
#MPI_run = /usr/lib64/openmpi/bin/mpiexec
MPI_run = mpiexec

# opcje optymalizacji:
# wersja do debugowania
# OPT = -g -DDEBUG -p
# wersja zoptymalizowana do mierzenia czasu
# OPT = -O3 -fopenmp -p
OPT = -O3  -fopenmp
#OPT = -O0 

# pliki naglowkowe
#INC = -I../pomiar_czasu

# biblioteki
#LIB = -L../pomiar_czasu -lm
#LIB = -L/opt/intel//mkl/8.1/lib/em64t -lmkl_em64t
#LIB = -L/opt/intel//mkl/8.1/lib/32 -lmkl_ia32
#LIB = -L/opt/intel/mkl/10.1.0.015/lib/em64t/ -lmkl_em64t 
LIB =  -lm

# zaleznosci i komendy
moj_program: moj_program.o mat_vec.o pomiar_czasu.o
	$(LOADER) $(OPT) moj_program.o mat_vec.o pomiar_czasu.o -o moj_program $(LIB)
	@echo 'To run execute "make run" (4 processes) or mpiexec manually.'

moj_program.o: moj_program.c pomiar_czasu.h
	$(CCOMP) -c $(OPT) moj_program.c $(INC)

mat_vec.o: mat_vec.c
	$(CCOMP) -c $(OPT) mat_vec.c

pomiar_czasu.o: pomiar_czasu.c pomiar_czasu.h
	$(CCOMP) -c $(OPT) pomiar_czasu.c

clean:
	rm -f *.o

run: moj_program
	$(MPI_run) -np 4 moj_program
