SN=memory

make:
	nasm -f elf32 -g -o $(SN).o $(SN).asm
	gcc -m32 -o $(SN) $(SN).o

clear:
	rm $(SN) $(SN).o

rebuild:
	make clear
	make
