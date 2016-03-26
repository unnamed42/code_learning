%define STDOUT 1
%define SYS_WRITE 4
%define SYS_EXIT 1

section .data
    msg db 'Hello, world'
    len equ $ - msg

section .text
    global main

main:
    mov eax,SYS_WRITE ;call sys_write
    mov ebx,STDOUT    ;file write to (stdout)
    mov ecx,msg       ;message to write
    mov edx,len       ;length of message to write
    int 0x80          ;execution
    
    mov eax,SYS_EXIT  ;call exit
    mov ebx,0         ;exit status
    int 0x80          ;execution
