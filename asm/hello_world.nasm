%define STDOUT 1        ;standard output file
%define SYS_WRITE 4     ;system call write()
%define SYS_EXIT 1      ;system call exit()
%define EXIT_SUCCESS 0  ;EXIT_SUCCESS exit status

global main             ;entry point main

bits 32                 ;this program works in 32-bit protected mode

section .data
    msg db 'Hello, world!',0
    len equ $ - msg

section .text

main:
    mov eax,SYS_WRITE    ;call sys_write
    mov ebx,STDOUT       ;file write to (stdout)
    mov ecx,msg          ;message to write
    mov edx,len          ;length of message to write
    int 0x80             ;execution
    
    mov eax,SYS_EXIT     ;call exit
    mov ebx,EXIT_SUCCESS ;exit status
    int 0x80             ;execution
