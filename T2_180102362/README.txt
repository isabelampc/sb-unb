Software Básico - Trabalho 2
Isabela Maria Pereira Cruzeiro - 180102362
Sistema operacional: Ubuntu 20.04.4 LTS (WSL) 
Editor: Visual Studio Code
Compilador: NASM version 2.14.02

Para a compilação, ligação e execução utilizou-se os seguintes comandos: 
nasm -f elf -o trabalho.o trabalho.asm
ld -m elf_i386 -o trabalho trabalho.o
./trabalho

O nome do arquivo em assembly é trabalho.asm e o ./trabalho é o arquivo executável. 

O executavel irá solitar a entrada de um arquivo que DEVE obrigatoriamente ser um arquivo já existente e na mesma pasta em que está o executável.
O nome do arquivo de entrada pode ser qualquer extensão e deve ser entrado apenas o nome de arquivo, como por exemplo "bin.out".
A extensão do arquivo de saída é .diss e será gerado na mesma pasta de execução.


O simulador não faz solitações de digite um número, apenas há a chamada de sistema de entrada. Só aceita números inteiros e positivos. 

O trabalho também foi testado no ambiente da Oracle Virtual Machine nas seguintes condições de sistema e compilação:

Sistema operacional: Ubuntu 16.04.6 LTS (32bit)
Compilador: NASM version 2.11.08

nasm -f elf -o trabalho.o trabalho.asm
ld -o trabalho trabalho.o
./trabalho

Tem as mesmas especificações para esse Sistema Operacional.




