section .data
    scalar_255 dq 255.0        ; Define the constant 255.0 in memory
    scalar_0_5 dq 0.5          ; Define the constant 0.5 in memory

section .text
global imgCvtGrayDoubleToInt
imgCvtGrayDoubleToInt:
    ; rcx = input array (double*)
    ; rdx = output array (unsigned char*)
    ; r8  = width
    ; r9  = height

    ; Save registers
    push rbx
    push rdi
    push rsi

    mov rdi, r9        ; rdi = height
    imul rdi, r8       ; rdi = width * height (total number of pixels)
    xor rsi, rsi       ; rsi = index = 0

    movsd xmm1, [scalar_255]    
    movsd xmm2, [scalar_0_5]  

L1:            
    cmp rsi, rdi 
    jge exit

    movsd xmm0, [rcx + rsi * 8] 

    mulsd xmm0, xmm1

    cvttsd2si eax, xmm0        

    movsd xmm3, xmm0           
    subsd xmm3, xmm0          

    ucomisd xmm3, xmm2
    jb round_down       

    
    inc eax

round_down:
    mov byte [rdx + rsi], al    

    inc rsi
    jmp L1      

exit:          
    pop rsi
    pop rdi
    pop rbx
    ret
