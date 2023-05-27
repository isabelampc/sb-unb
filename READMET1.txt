Software Básico - Trabalho 1
Isabela Maria Pereira Cruzeiro - 180102362
Sistema operacional: Windows 11
Editor: Visual Studio Code
Compilador: MinGW gcc 4.9.2

A programação foi feita em dois arquivos, o montador.c, onde esta definida a main, e o arquivo def.h, onde todos as funções 
do montador estao definidas. 

Para compilar o programa a seguinte chamada foi feita:
gcc montador.c -o montador 
Onde:
montador.c é o nome do programa principal e montador é o nome do executável gerado.
Para a execução do programa:
./montador -o entrada.asm saida.obj  
Onde:
entrada.asm - arquivo de entrada com o código fonte
saida.obj - arquivo de saida com o processamento 
-o - modo de operação do montador. 

O programa só aceita as entradas -o, -p, -m como modo de operação. Caso outra entrada seja colocada, o programa irá
imprimir mensagem de erro e não acontecerá nada. O arquivo de entrada deve estar no mesmo diretório que o programa para
funcionar corretamente, a pasta "Exemplos" tem os arquivos disposto no moodle com suas respectivas saídas. 

Caso -p, -m o código de pre-processamento gerado não há o pre-processamento para chamadas de CONST e SPACE no meio do código. 
Caso o programa contenha algum erro, não é possível montar o código objeto de maneira correta e então será impresso 
na linha de comando o erro e em qual linha ele se encontra, de acordo com o pré-processamento. Caso o erro seja do tipo
léxico, ou haja mais de um rotulo definido na mesma linha, ou erros relacionados a macro e diretivas EQU e IF, não há
montagem. 

O montador implementado é o de duas passagens. Onde na primeira passagem há a montagem da tabela de simbolos, analise
léxica e sintática. A segunda é a de montagem e de analise semantica e analise sintatica das instrucoes.

Caso tenha algum tipo de dado CONST ou SPACE, eles serão realocados para o final do código, apos o STOP e as outras
variaveis ali ja definidas. Exemplo:
ROT: INPUT N1
COPY N1, N4 
COPY N2, N3
COPY N3, N3
N2: CONST -48
OUTPUT N3
N4: SPACE
STOP
N1: SPACE

Fica da seguinte forma:
ROT: INPUT N1
COPY N1, N4 
COPY N2, N3
COPY N3, N3
OUTPUT N3
STOP
N1: SPACE
N2: CONST -48
N4: SPACE 

