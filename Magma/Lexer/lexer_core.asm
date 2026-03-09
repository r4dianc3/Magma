; mgla_fast_lexer.asm
; build:
; nasm -felf64 mgla_fast_lexer.asm
; ld mgla_fast_lexer.o -o mgla_lexer

global _start

section .bss
buffer resb 1048576
filesize resq 1

section .data

filename db "test.mgla",0

; token names
tok_identifier db "IDENTIFIER",10
tok_number db "NUMBER",10
tok_float db "FLOAT",10
tok_string db "STRING",10
tok_char db "CHAR",10

tok_plus db "PLUS",10
tok_minus db "MINUS",10
tok_star db "STAR",10
tok_slash db "SLASH",10
tok_percent db "PERCENT",10
tok_power db "POWER",10

tok_increment db "INCREMENT",10
tok_decrement db "DECREMENT",10

tok_equals db "EQUALS",10
tok_eqeq db "EQUAL_EQUAL",10
tok_noteq db "NOT_EQUAL",10

tok_less db "LESS",10
tok_le db "LESS_EQUAL",10
tok_gt db "GREATER",10
tok_ge db "GREATER_EQUAL",10

tok_and db "AND",10
tok_or db "OR",10
tok_not db "NOT",10

tok_bitand db "BIT_AND",10
tok_bitor db "BIT_OR",10
tok_xor db "BIT_XOR",10
tok_bitnot db "BIT_NOT",10
tok_shl db "BIT_SHIFT_LEFT",10
tok_shr db "BIT_SHIFT_RIGHT",10

tok_lparen db "LPAREN",10
tok_rparen db "RPAREN",10
tok_lbrace db "LBRACE",10
tok_rbrace db "RBRACE",10
tok_lbracket db "LBRACKET",10
tok_rbracket db "RBRACKET",10

tok_comma db "COMMA",10
tok_dot db "DOT",10
tok_colon db "COLON",10
tok_qmark db "QMARK",10
tok_arrow db "ARROW",10
tok_backslash db "BACKSLASH",10

tok_newline db "NEWLINE",10
tok_eof db "EOF",10

section .text

_start

; open file
mov rax,2
mov rdi,filename
xor rsi,rsi
syscall

mov r12,rax

; read file
mov rax,0
mov rdi,r12
mov rsi,buffer
mov rdx,1048576
syscall

mov [filesize],rax

; start lexer
mov rsi,0

main_loop

cmp rsi,[filesize]
jge end_lex

mov al,[buffer+rsi]

; whitespace
cmp al,' '
je advance

cmp al,9
je advance

cmp al,10
je newline

; identifier
cmp al,'A'
jb digit_check
cmp al,'z'
jbe identifier

digit_check
cmp al,'0'
jb operator
cmp al,'9'
jbe number

operator

cmp al,'+'
je plus

cmp al,'-'
je minus

cmp al,'*'
je star

cmp al,'/'
je slash

cmp al,'%'
je percent

cmp al,'='
je equals

cmp al,'!'
je not

cmp al,'<'
je less

cmp al,'>'
je greater

cmp al,'&'
je amp

cmp al,'|'
je pipe

cmp al,'^'
je xorop

cmp al,'~'
je bitnot

cmp al,'('
je lparen

cmp al,')'
je rparen

cmp al,'{'
je lbrace

cmp al,'}'
je rbrace

cmp al,'['
je lbracket

cmp al,']'
je rbracket

cmp al,','
je comma

cmp al,'.'
je dot

cmp al,':'
je colon

cmp al,'?'
je qmark

cmp al,'\'
je backslash

advance
inc rsi
jmp main_loop

newline
inc rsi
jmp print_newline

identifier
inc rsi
id_loop
cmp rsi,[filesize]
jge print_identifier
mov al,[buffer+rsi]
cmp al,'0'
jb print_identifier
cmp al,'z'
jbe id_continue
jmp print_identifier
id_continue
inc rsi
jmp id_loop

print_identifier
mov rax,1
mov rdi,1
mov rsi,tok_identifier
mov rdx,11
syscall
jmp main_loop

number
inc rsi
num_loop
cmp rsi,[filesize]
jge print_number
mov al,[buffer+rsi]
cmp al,'0'
jb print_number
cmp al,'9'
jbe num_continue
jmp print_number
num_continue
inc rsi
jmp num_loop

print_number
mov rax,1
mov rdi,1
mov rsi,tok_number
mov rdx,7
syscall
jmp main_loop

plus
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_plus
mov rdx,5
syscall
jmp main_loop

minus
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_minus
mov rdx,6
syscall
jmp main_loop

star
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_star
mov rdx,5
syscall
jmp main_loop

slash
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_slash
mov rdx,6
syscall
jmp main_loop

percent
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_percent
mov rdx,8
syscall
jmp main_loop

equals
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_equals
mov rdx,7
syscall
jmp main_loop

not
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_not
mov rdx,4
syscall
jmp main_loop

less
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_less
mov rdx,5
syscall
jmp main_loop

greater
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_gt
mov rdx,8
syscall
jmp main_loop

amp
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_bitand
mov rdx,8
syscall
jmp main_loop

pipe
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_bitor
mov rdx,7
syscall
jmp main_loop

xorop
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_xor
mov rdx,8
syscall
jmp main_loop

bitnot
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_bitnot
mov rdx,8
syscall
jmp main_loop

lparen
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_lparen
mov rdx,7
syscall
jmp main_loop

rparen
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_rparen
mov rdx,7
syscall
jmp main_loop

lbrace
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_lbrace
mov rdx,7
syscall
jmp main_loop

rbrace
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_rbrace
mov rdx,7
syscall
jmp main_loop

lbracket
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_lbracket
mov rdx,9
syscall
jmp main_loop

rbracket
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_rbracket
mov rdx,9
syscall
jmp main_loop

comma
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_comma
mov rdx,6
syscall
jmp main_loop

dot
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_dot
mov rdx,4
syscall
jmp main_loop

colon
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_colon
mov rdx,6
syscall
jmp main_loop

qmark
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_qmark
mov rdx,6
syscall
jmp main_loop

backslash
inc rsi
mov rax,1
mov rdi,1
mov rsi,tok_backslash
mov rdx,10
syscall
jmp main_loop

print_newline
mov rax,1
mov rdi,1
mov rsi,tok_newline
mov rdx,8
syscall
jmp main_loop

end_lex

mov rax,1
mov rdi,1
mov rsi,tok_eof
mov rdx,4
syscall

mov rax,60
xor rdi,rdi
syscall