%include "io.mac"

section .data
msg db 'Hello Word!' , 0dh, 0ah ;ihuigig

section .text

global _start

_start: 
        mov eax, 4
        mov ebx, 1
        mov ecx, msg
        mov edx, 13
        int 80h
        .EXIT

        mov al, [pos_index]
        cmp al, 0
        je zero
        
        mov byte[pos_index], 0
        jmp aqui
zero:   
        mov ebx, [pos_index]
        mov ax, [entrada]
        mov [pos_atual+ebx], ax
        mov ebx, [pos_index]
        inc ebx
        mov [pos_index], ebx
        mov eax, 4 ;printar oq eu to colocando 
        mov ebx, 1
        mov ecx, pos_atual
        mov edx, 1
        int 80h