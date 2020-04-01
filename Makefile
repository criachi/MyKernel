CC=gcc -g 

all : loader.o kernel.o interpreter.o shell.o shellmemory.o pcb.o cpu.o ram.o memorymanager.o
	$(CC) loader.o kernel.o interpreter.o shell.o shellmemory.o pcb.o cpu.o ram.o memorymanager.o -o mykernel -lm

loader.o : loader.c
	$(CC) -c loader.c  

kernel.o : kernel.c
	$(CC) -c kernel.c  

interpreter.o : interpreter.c 
	$(CC) -c interpreter.c 

shell.o : shell.c 
	$(CC) -c shell.c 

shellmemory.o : shellmemory.c
	$(CC) -c shellmemory.c 
	
pcb.o : pcb.c 
	$(CC) -c pcb.c 

cpu.o : cpu.c 
	$(CC) -c cpu.c

ram.o : ram.c 
	$(CC) -c ram.c     
memorymanager.o : memorymanager.c 
	$(CC) -c memorymanager.c 
.PHONY:
clean:
	rm -f interpreter.o shell.o shellmemory.o pcb.o cpu.o kernel.o ram.o loader.o
	rm -f mykernel
.PHONY:
run: mykernel
	./mykernel
.PHONY:
debug: mykernel
	gdb mykernel
.PHONY:
test: mykernel testfile.txt
	./mykernel < testfile.txt
.PHONY:
mytest: mykernel mytestfile.txt
	./mykernel < mytestfile.txt