.globl _main
_main:
sub sp, sp, #64
mov w0, #4
str w0, [sp, #48]
mov w0, #9
ldr w1, [sp, #48]
mul w0, w0, w1
str w0, [sp, #48]
mov w0, #3
ldr w1, [sp, #48]
mul w0, w0, w1
add sp, sp, #64
ret
