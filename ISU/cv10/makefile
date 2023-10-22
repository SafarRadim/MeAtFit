SN=expression

build:
	nasm -f elf32 -g -o $(SN).o $(SN).asm
	gcc -m32 $(SN).o -o $(SN)

clear:
	rm $(SN) $(SN).o
