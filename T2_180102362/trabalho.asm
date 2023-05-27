;Trabalho 2- Software Básico
;Isabela Maria Pereira Cruzeiro 
;180102362
;
section .data 
msg_nome_arquivo db "Digite o nome do arquivo:", 0dH, 0ah
tam_msg_nome_arquivo EQU $-msg_nome_arquivo ;tamamho da mensagem
extensao db "diss", 0
tamanho_arquivo EQU 4096 ;TAMANHO MAXIMO DO ARQUIVO
msg_final db "Numero de bytes digitados no arquivo: ", 0
tam_msg_final EQU $-msg_final
espaco db " ", 0dh, 0ah
tam_espaco EQU $-espaco
msg_erro_arquivo db "Erro ao abrir o arquivo, arquivo inexistente", 0dH, 0ah
tam_msg_erro_arquivo EQU $-msg_erro_arquivo

;STRINGS PARA PRINTAR ARQUIVO SAIDA DISASSEMBLER
case_add db "ADD", 0dh, 0ah
tam_case_add EQU $-case_add
case_sub db "SUB", 0dh, 0ah
tam_case_sub EQU $-case_sub
case_mult db "MULT", 0dh, 0ah
tam_case_mult EQU $-case_mult
case_div db "DIV", 0dh, 0ah
tam_case_div EQU $-case_div
case_jmp db "JMP", 0dh, 0ah
tam_case_jmp EQU $-case_jmp
case_jmpn db "JMPN", 0dh, 0ah
tam_case_jmpn EQU $-case_jmpn
case_jmpp db "JMPP", 0dh, 0ah
tam_case_jmpp EQU $-case_jmpp
case_jmpz db "JMPZ", 0dh, 0ah
tam_case_jmpz EQU $-case_jmpz
case_copy db "COPY", 0dh, 0ah
tam_case_copy EQU $-case_copy
case_load db "LOAD", 0dh, 0ah
tam_case_load EQU $-case_load
case_store db "STORE", 0dh, 0ah
tam_case_store EQU $-case_store
case_input db "INPUT", 0dh, 0ah
tam_case_input EQU $-case_input
case_output db "OUTPUT", 0dh, 0ah
tam_case_output EQU $-case_output
case_stop db "STOP", 0dh, 0ah
tam_case_stop EQU $-case_stop
;array dw tamanho_arquivo 

section .bss
pos_index resd 1 ;index do meu byte
pos_atual resb 2 ;uma palavra 
array_index resd 1 ;index do meu array 
nome_arquivo resb 30
arquivo_saida resb 30 ;nome arquivo de saida .dis
entrada resb 1 ;pegando minha entrada
array resw tamanho_arquivo ;tamanho maximo do meu arquivo 
descricao_arquivo resb 30 ;file descriptor entrada
descricao_arquivo_saida resb 30 ;file descriptor saida
index resd 1 ;index que controla o vetor  
vetor_simulador resw tamanho_arquivo
indice_simulador resb 1 
teste resw 1
bytes_escritos resd 1
aux resd 2
acumulador resd 1 ;acumulador 
dados resd 100
dados_indice resd 1
inicio_dados resd 1
final resd 1
aux_tamanho resd 1
 
section .text
global _start
_start:
        push tam_msg_nome_arquivo
        push msg_nome_arquivo
        call printa_mensagem
        
        mov eax, 3 ;recebe o nome arquivo
        mov ebx, 1
        mov ecx, nome_arquivo
        mov edx, 30
        int 80h
        mov byte [ecx + eax - 1], 0 ;botar 0 no final da string pra poder ler 
        mov byte[array_index], 0
        
        push nome_arquivo
        call ler_aquivo
        push descricao_arquivo
        call fecha_arquivo
        mov byte[bytes_escritos], 0
        mov byte[index], 0 ;index do meu vetor principal 
        mov ebx, [index]
        mov byte[pos_index], 0 
        mov byte[indice_simulador], 0 
        mov ecx, [array_index] ;controle de fluxo
        

; PARTE QUE EU AJEITO O VETOR DE INDEX 

controle:
        mov ebx, [index]
        mov al, byte [array+ebx] ;vendo meu array principal
        mov ah, [pos_index] 
        mov dl, byte [array+ebx]
        cmp dl, "-" ;ver se é outra posição
        jne verifica 
        mov ebx, [index] ; incrementando o meu index principal 
        inc ebx
        mov [index], ebx
        cmp ah, 1
        jne volta ;duas posicoes, vou pro volta
        mov byte[pos_index], 0 ;garantir que ele volta sempre para zero 
        mov al, " "
        mov [pos_atual+1], al
volta:
        mov ebx, [indice_simulador] 
        mov ax, word [pos_atual]
        mov word [vetor_simulador + ebx], ax
        inc ebx
        inc ebx 
        mov [indice_simulador], ebx
        dec ecx
        jnz controle
        jmp compara
verifica:
        cmp ah, 0
        je caso_primeiro_byte
        mov ebx, [pos_index]  ;posicao do meu array de dois bytes
        mov [pos_atual + ebx], al ;colocando na posicao do meu array  
        mov ebx, [index] ; incrementando o meu index principal 
        inc ebx
        mov [index], ebx
        mov byte[pos_index], 0 ;voltando posicao atual pra 0
        dec ecx
        jnz controle
        jmp compara
caso_primeiro_byte:         
        mov ebx, [pos_index]  ;posicao do meu array de dois bytes
        mov [pos_atual + ebx], al ;colocando na posicao do meu array
        mov ebx, [pos_index]
        inc ebx
        mov [pos_index], ebx ; incrementando para ir pro 1 byte
        mov ebx, [index] ; incrementando o meu index principal 
        inc ebx
        mov [index], ebx 
        dec ecx
        jnz controle
        jmp compara
        
;PARTE PARA FAZER SEPARAÇÃO DOS OPCODES
; DISSASSEMBLER       
compara:
        mov eax, [indice_simulador]
        mov [final], eax
        call abrir_arquivo_saida
        mov byte[bytes_escritos], 0
        mov byte[index], 0 ;index do meu vetor principal , retornando para zero 
        mov ebx, [index]
for:
        mov ebx, [index]
        mov ax, word [vetor_simulador+ebx]
        mov [teste], ax 
        cmp word[teste], "1 " ;case add
        je adds
        cmp word[teste], "2 " ;case isub
        je subs
        cmp word[teste], "3 " ;case mulkt
        je mult
        cmp word[teste], "4 " ; case div
        je divs
        cmp word[teste], "5 " ; case jmp
        je jmps
        cmp word[teste], "6 " ; case jmpn
        je jmpn
        cmp word[teste], "7 " ; case jmpp
        je jmpp
        cmp word[teste], "8 " ; case ´jmpz
        je jmpz
        cmp word[teste], "9 " ; case copy
        je copy
        cmp word[teste], "11" ; case store
        je store
        cmp word[teste], "12" ;case input
        je input
        cmp word[teste], "10" ; case load 
        je load
        cmp word[teste], "13" ; case output
        je output
        cmp word[teste], "14" ; case stop

        je stop
        jmp end

adds: 
        push tam_case_add
        push case_add
        call printa_arquivo
        jmp for

subs:
        push tam_case_sub
        push case_sub
        call printa_arquivo
        jmp for

mult: 
        push tam_case_mult
        push case_mult
        call printa_arquivo
        jmp for

jmps: 
        push tam_case_jmp
        push case_jmp
        call printa_arquivo
        jmp for

jmpn:         
        push tam_case_jmpn
        push case_jmpn
        call printa_arquivo
        jmp for  

jmpp:         
        push tam_case_jmpp
        push case_jmpp
        call printa_arquivo
        jmp for     

jmpz:         
        push tam_case_jmpz
        push case_jmpz
        call printa_arquivo
        jmp for       

store: 
        push tam_case_store
        push case_store
        call printa_arquivo
        jmp for

copy:         
        push tam_case_copy
        push case_copy
        call printa_arquivo
        mov ecx, [index]
        add ecx, 2
        mov [index], ecx
        jmp for


output: 
        push tam_case_output
        push case_output
        call printa_arquivo
        jmp for

stop:
        push tam_case_stop
        push case_stop
        call printa_arquivo
        mov dword[dados_indice], 0 ;inicializando
        mov eax, [index]
        sub eax, 2 
        mov [inicio_dados], eax
        mov [index], eax
        mov ebx, [index]
arruma_dados:
        cmp ebx, dword[final]
        ja simulador
        mov ebx, [index]
        mov eax, vetor_simulador
        add eax, ebx
        push 1; so recebo com tamanho dois
        push eax
        call string_to_int

        mov ecx, [dados_indice]
        mov dword[dados+ecx], eax
        ; definir do dados indice VETOR ANDA DE 4 EM QUATRO 
        mov ebx, [dados_indice]
        add ebx, 4
        mov [dados_indice], ebx
        ;definir o loop do vetor principal 
        mov ebx, [index]
        add ebx, 2
        mov [index], ebx
        mov ebx, [index]
        jmp arruma_dados

input:
        push tam_case_input
        push case_input
        call printa_arquivo
        jmp for

load:   
        push tam_case_load
        push case_load
        call printa_arquivo
        jmp for       

divs: 
        push tam_case_div
        push case_div
        call printa_arquivo
        jmp for

; COMEÇO DO SIMULADOR
simulador:
        ;print de bytes
        push descricao_arquivo_saida
        call fecha_arquivo
        mov byte[index], 0 ;index do meu vetor principal , retornando para zero 
        mov byte[dados_indice], 0
        mov ebx, [index]
        
loop_simulador:

        mov ebx, [index]
        
        mov ax, word [vetor_simulador+ebx]
        mov [teste], ax
        cmp word[teste], "12" ;case input
        je simu_input
        cmp word[teste], "10" ; case load 
        je simu_load
        cmp word[teste], "1 " ;case add
        je simu_add
        cmp word[teste], "2 " ;case isub
        je simu_sub
        cmp word[teste], "3 " ;case mulkt
        je simu_mult
        cmp word[teste], "4 " ; case div
        je simu_div
        cmp word[teste], "5 " ; case jmp
        je simu_jmp
        cmp word[teste], "6 " ; case jmpn
        je simu_jmpn
        cmp word[teste], "7 " ; case jmpp
        je simu_jmpp
        cmp word[teste], "8 " ; case ´jmpz
        je simu_jmpz
        cmp word[teste], "9 " ; case copy
        je simu_copy
        cmp word[teste], "11" ; case store
        je simu_store
        cmp word[teste], "13" ; case output
        je simu_output
        cmp word[teste], "14" ; case stop
        je end
        jmp end

simu_add:
        add ebx, 2 ;quero pegar o valor que ta na proxima
        mov eax, vetor_simulador
        add eax, ebx
        mov ecx, 2 ; so recebo com tamanho dois
        push 2; so recebo com tamanho dois
        push eax
        call string_to_int
        mov ebx, eax
        imul ebx, 2
        sub ebx, [inicio_dados]
        imul ebx, 2
        mov ecx, dword[dados+ebx]
        mov eax, [acumulador]
        add eax, ecx
        mov [acumulador], eax
        mov ebx, [index]
        add ebx, 4
        mov [index], ebx
        jmp loop_simulador
simu_sub:
        add ebx, 2 ;quero pegar o valor que ta na proxima
        mov eax, vetor_simulador
        add eax, ebx
        push 2; so recebo com tamanho dois
        push eax
        call string_to_int
        
        mov ebx, eax
        imul ebx, 2
        sub ebx, [inicio_dados]
        imul ebx, 2
        mov ecx, dword[dados+ebx]
        mov eax, [acumulador]
        sub eax, ecx
        mov [acumulador], eax
        mov ebx, [index]
        add ebx, 4
        mov [index], ebx
        jmp loop_simulador

simu_mult:
        add ebx, 2 ;quero pegar o valor que ta na proxima
        mov eax, vetor_simulador
        add eax, ebx
        push 2; so recebo com tamanho dois
        push eax
        call string_to_int
        mov ebx, eax
        imul ebx, 2
        sub ebx, [inicio_dados]
        imul ebx, 2
        mov ecx, dword[dados+ebx]
        mov eax, [acumulador]
        imul eax, ecx
        mov [acumulador], eax
        mov ebx, [index]
        add ebx, 4
        mov [index], ebx
        jmp loop_simulador

simu_div:
        add ebx, 2 ;quero pegar o valor que ta na proxima
        mov eax, vetor_simulador
        add eax, ebx
        push 2; so recebo com tamanho dois
        push eax
        call string_to_int
        mov ebx, eax
        imul ebx, 2
        sub ebx, [inicio_dados]
        imul ebx, 2
        mov eax, ebx
        mov edx, 0 ;limpar edx
        mov ecx, dword[dados+ebx] ; colocando o divisor em ecx
        mov eax, [acumulador]
        div ecx ; realizando a divisão
        mov [acumulador], eax
        mov ebx, [index]
        add ebx, 4
        mov [index], ebx
        jmp loop_simulador


simu_jmp:
        add ebx, 2 ;quero pegar o valor que ta na proxima
        mov dl, byte[vetor_simulador+ebx+1]
        cmp dl, " " ; se for um espaço tamanho 1
        je tam1_jmp
        mov dword[aux_tamanho], 2
        jmp volta_jmp
tam1_jmp:
        mov dword[aux_tamanho], 1
volta_jmp:
        mov eax, vetor_simulador
        add eax, ebx
        mov ecx, [aux_tamanho] ; so recebo com tamanho dois
        push ecx; so recebo com tamanho dois
        push eax
        call string_to_int
        mov ebx, eax
        imul ebx, 2
        mov [index], ebx
        mov eax, [index]
        jmp loop_simulador

simu_jmpn:
        add ebx, 2 ;quero pegar o valor que ta na proxima
        mov dl, byte[vetor_simulador+ebx+1]
        cmp dl, " " ; se for um espaço tamanho 1
        je tam1_jmpn
        mov dword[aux_tamanho], 2
        jmp volta_jmpn
tam1_jmpn:
        mov dword[aux_tamanho], 1
volta_jmpn:
        mov eax, vetor_simulador
        add eax, ebx
        mov ecx, [aux_tamanho] ; so recebo com tamanho dois
        push ecx; so recebo com tamanho dois
        push eax
        call string_to_int
        mov ebx, eax
        imul ebx, 2 ;valor que pula pra ele 
        cmp dword[acumulador], 0
        jl pula_jmpn
        mov ebx, [index]
        add ebx, 4
        mov [index], ebx
        jmp loop_simulador
pula_jmpn:
        mov [index], ebx
        jmp loop_simulador

simu_jmpp:
        add ebx, 2 ;quero pegar o valor que ta na proxima
        mov dl, byte[vetor_simulador+ebx+1]
        cmp dl, " " ; se for um espaço tamanho 1
        je tam1_jmpp
        mov dword[aux_tamanho], 2
        jmp volta_jmpp
tam1_jmpp:
        mov dword[aux_tamanho], 1
volta_jmpp:
        mov eax, vetor_simulador
        add eax, ebx
        mov ecx, [aux_tamanho] ; so recebo com tamanho dois
        push ecx; so recebo com tamanho dois
        push eax
        call string_to_int
        mov ebx, eax
        imul ebx, 2 ;valor que pula pra ele 
        cmp dword[acumulador], 0
        jg pula_jmpp
        mov ebx, [index]
        add ebx, 4
        mov [index], ebx
        jmp loop_simulador
pula_jmpp:
        mov [index], ebx
        jmp loop_simulador

simu_jmpz:
        add ebx, 2 ;quero pegar o valor que ta na proxima
        mov dl, byte[vetor_simulador+ebx+1]
        cmp dl, " " ; se for um espaço tamanho 1
        je tam1_jmpz
        mov dword[aux_tamanho], 2
        jmp volta_jmpz
tam1_jmpz:
        mov dword[aux_tamanho], 1
volta_jmpz:
        mov eax, vetor_simulador
        add eax, ebx
        mov ecx, [aux_tamanho] ; so recebo com tamanho dois
        push ecx; so recebo com tamanho dois
        push eax
        call string_to_int
        mov ebx, eax
        imul ebx, 2 ;valor que pula pra ele 
        cmp dword[acumulador], 0
        je pula_jmpz
        mov ebx, [index]
        add ebx, 4
        mov [index], ebx
        jmp loop_simulador
pula_jmpz:
        mov [index], ebx
        jmp loop_simulador


simu_input:
        add ebx, 2 ;quero pegar o valor que ta na proxima
        mov eax, vetor_simulador
        add eax, ebx
        push 2; so recebo com tamanho dois
        push eax
        call string_to_int
        mov [aux], eax ;salvando endereço
        mov eax, 3
        mov ebx, 1
        mov ecx, entrada
        mov edx, 4
        int 80h
        mov byte [ecx + eax - 1], '.' ;botar 0 no final da string pra poder ler 
        sub eax, 1
        mov ecx, eax
        push ecx; so recebo com tamanho dois
        push entrada
        call string_to_int
        mov edx, eax
        mov eax, ebx
        mov ebx, [aux]
        imul ebx, 2
        sub ebx, [inicio_dados]
        imul ebx, 2
        mov dword[dados+ebx], edx 
        mov ebx, [index]
        add ebx, 4
        mov [index], ebx 
        push tam_espaco
        push espaco
        call printa_mensagem
        jmp loop_simulador

simu_copy:
        add ebx, 2 ;quero pegar o valor que ta na proxima
        mov eax, vetor_simulador
        add eax, ebx
        push 2; so recebo com tamanho dois
        push eax
        call string_to_int
        mov ebx, eax
        imul ebx, 2
        sub ebx, [inicio_dados]
        imul ebx, 2
        mov eax, dword[dados+ebx]
        mov [aux], eax; aux = memoria op1
        mov ebx, [index]
        add ebx, 4 ; pegando o segundo operando 
        mov eax, vetor_simulador
        add eax, ebx
        push 2; so recebo com tamanho dois
        push eax
        call string_to_int
        mov ebx, eax
        imul ebx, 2
        sub ebx, [inicio_dados]
        imul ebx, 2
        mov eax, [aux]
        mov dword[dados+ebx], eax
        mov ebx, [index]
        add ebx, 6 ; copy tem dois operandos logo somo 6
        mov [index], ebx
        jmp loop_simulador

simu_store:
        add ebx, 2 ;quero pegar o valor que ta na proxima
        mov eax, vetor_simulador
        add eax, ebx
        push 2; so recebo com tamanho dois
        push eax; so recebo com tamanho dois
        call string_to_int
        mov ebx, eax
        imul ebx, 2
        sub ebx, [inicio_dados]
        imul ebx, 2
        mov eax, [acumulador]
        mov dword[dados+ebx], eax
        mov eax, dword[dados+ebx] 
        mov ebx, [index]
        add ebx, 4
        mov [index], ebx
        jmp loop_simulador

simu_load:       
        add ebx, 2 ;quero pegar o valor que ta na proxima
        mov eax, vetor_simulador
        add eax, ebx
        push 2; so recebo com tamanho dois
        push eax
        call string_to_int
        mov ebx, eax
        imul ebx, 2
        sub ebx, [inicio_dados]
        imul ebx, 2
        mov eax, dword[dados+ebx]
        mov [acumulador], eax
        mov ebx, [index]
        add ebx, 4
        mov [index], ebx
        jmp loop_simulador

simu_output:
        add ebx, 2 ;quero pegar o valor que ta na proxima
        mov eax, vetor_simulador
        add eax, ebx
        push 2; so recebo com tamanho dois
        push eax
        call string_to_int
        mov ebx, eax
        imul ebx, 2
        sub ebx, [inicio_dados]
        imul ebx, 2
        ;mov eax, dword[dados+ebx]
        push dword[dados+ebx]
        call printa_inteiro
        push tam_espaco
        push espaco
        call printa_mensagem
        mov ebx, [index]
        add ebx, 4
        mov [index], ebx
        jmp loop_simulador


end:    
        push tam_msg_final
        push msg_final 
        call printa_mensagem      
        push dword[bytes_escritos]
        call printa_inteiro
        push tam_espaco
        push espaco
        call printa_mensagem
        mov eax, 1 ; termina programa
        mov ebx, 0
        int 80h

ler_aquivo: 
        mov ebp, esp
        mov eax, 5 ; abrir ler arquivo 
        mov ebx, [ebp+4]
        mov ecx, 0 ; leitura
        mov edx, 0777
        int 0x80
        mov dword [descricao_arquivo], eax ;salvando file descriptor
        cmp eax, 0
        jl erro_arquivo

lendo:
        mov eax, 3 ; lendo arquivo
        mov ebx, [descricao_arquivo] ; carregando o arquivo 
        lea ecx, [entrada] ;aqui eu pego o buffer para a leitura atual
        mov edx, 1 ; quero ler apenar um byte 
        int 80h ;chamada sistema 
        cmp eax, edx ;chegou no final do arquivo?
        jne eof
        mov al, [entrada]
        cmp al , " "
        je igual        
        ;colocar no array 

        mov ebx, [array_index]
        mov ax, [entrada]
        mov [array + ebx], ax
        mov ebx, [array_index]
        inc ebx
        mov [array_index], ebx

        jmp escrita

igual:  ;vem pra ca
        mov ebx, [array_index]
        mov ax, "-" 
        mov [array + ebx], ax
        mov ebx, [array_index]
        inc ebx
        mov [array_index], ebx
         
;escrita arquivo, ainda preciso fazer mover +1 para a saída do arquivo de escrita!

escrita:
        jmp lendo

eof:    
        mov ebx, [array_index]
        mov ax, "-" 
        mov [array + ebx], ax
        mov ebx, [array_index]
        inc ebx
        mov [array_index], ebx
        ret

string_to_int:; passar para esi e ecx
        mov ebp, esp
        mov esi, [ebp+4] ; palavra
        mov ecx, [ebp+8] ; tamanho        
        sub ebx,ebx    ; limpar o ebx 
next_digit:
        movzx eax,byte[esi] ;tem q completar com zero
        inc esi
        sub al,'0'    ; converter ASCII para numero
        imul ebx,10
        add ebx,eax   ; ebx = ebx*10 + eax
        loop next_digit  
        mov eax,ebx
        ret 4

abrir_arquivo_saida:
        mov byte[array_index], 0

prox_arquivo:
        mov ebx, [array_index]
        cmp byte[nome_arquivo+ebx], "."
        je termina_arquivo
        cmp byte[nome_arquivo+ebx], 0
        je termina_arquivo
        mov al, byte[nome_arquivo+ebx]
        mov [arquivo_saida+ebx], al
        inc ebx 
        mov [array_index],ebx
        jmp prox_arquivo
 
termina_arquivo:
        mov byte[arquivo_saida+ebx], '.'
        inc ebx
        mov eax, [extensao]
        mov [arquivo_saida+ebx], eax
        mov eax, 8
        mov ebx, arquivo_saida
        mov ecx, 0777
        int 0x80
        mov dword [descricao_arquivo_saida], eax ;file description
        ret

fecha_arquivo:
        mov ebp, esp
        mov eax, 6
        mov ebx, [ebp+4]
        int 80h
        ret

printa_inteiro:
        mov ebp, esp
        mov eax, [ebp+4]
        sub esi, esi ;limpando esi

loop_print_inteiro:
        mov edx, 0
        mov ebx, 10
        div ebx
        add edx, '0' ; ASCII para número 
        push edx
        inc esi
        cmp eax,0
        jz   prox 
        jmp loop_print_inteiro
prox:
        cmp esi, 0 ; ver se chegou ao final do número
        jz   saida_printa_inteiro
        dec esi
        mov eax, 4
        mov ecx, esp ;valor atual
        mov ebx, 1
        mov edx, 1
        int  80h
        add esp, 4
        jmp  prox
saida_printa_inteiro:   ret 

printa_mensagem:
        mov ebp,esp ; copy TOS into EBP
        mov eax, 4
        mov ebx, 1 
        mov ecx, [ebp+4]
        mov edx, [ebp+8]
        int 80h
        ret 4

printa_arquivo:
        mov ebp, esp
        mov eax, 4
        mov ebx, [descricao_arquivo_saida]
        mov ecx, [ebp+4] ;saindo a entrada normal por enquanto 
        mov edx, [ebp+8]
        int 80h
        add edx, [bytes_escritos] ; adicionando os bytes escritos
        mov [bytes_escritos], edx
        mov ecx, [index]
        add ecx, 4
        mov [index], ecx
        ret 4

erro_arquivo:
        push tam_msg_erro_arquivo
        push msg_erro_arquivo
        call printa_mensagem
        mov eax, 1
        mov ebx, 0
        int 80h





      