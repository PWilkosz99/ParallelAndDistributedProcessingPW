# kompilator c
CCOMP = mpicc

# konsolidator
LOADER = mpicc

# program uruchomieniowy MPI
MPI_run = mpiexec

# opcje optymalizacji:
# wersja do debugowania
OPT = -g -DDEBUG
# wersja zoptymalizowana do mierzenia czasu
# OPT = -O3

# pliki naglowkowe
#INC = -I../pomiar_czasu

# biblioteki
LIB = -lm

all: MPI_PI run 

run: MPI_PI
	$(MPI_run) --oversubscribe -np 8 MPI_PI

# zaleznosci i komendy
MPI_PI : MPI_PI.o
	$(LOADER) $(OPT) MPI_PI.o -o MPI_PI $(LIB)

# jak uzyskac plik moj_program.o ?
MPI_PI.o: MPI_PI.c 
	$(CCOMP) -c $(OPT) MPI_PI.c

clean:
	rm -f *.o
