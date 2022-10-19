SerExpanGregoryLeibniz:
        testl   %edi, %edi    # N
        jle     .L4       #,
        movsd   .LC0(%rip), %xmm4     #, tmp102
        movsd   .LC2(%rip), %xmm6     #, tmp103
        xorl    %eax, %eax      # i
        pxor    %xmm3, %xmm3    # <retval>
        movq    .LC3(%rip), %xmm5       #, tmp104
        movapd  %xmm4, %xmm2        # tmp102, signDef
.L3:
        pxor    %xmm0, %xmm0    # tmp93
        movapd  %xmm2, %xmm1        # signDef, tmp91
        xorpd   %xmm5, %xmm2  # tmp104, signDef
        cvtsi2sdl       %eax, %xmm0   # i, tmp93
        mulsd   %xmm6, %xmm1  # tmp103, tmp91
        addl    $1, %eax        #, i
        addsd   %xmm0, %xmm0  # tmp93, tmp94
        addsd   %xmm4, %xmm0  # tmp102, tmp95
        divsd   %xmm0, %xmm1  # tmp95, tmp97
        addsd   %xmm1, %xmm3  # tmp97, <retval>
        cmpl    %eax, %edi      # i, N
        jne     .L3       #,
        movapd  %xmm3, %xmm0        # <retval>,
        ret
.L4:
        pxor    %xmm3, %xmm3    # <retval>
        movapd  %xmm3, %xmm0        # <retval>,
        ret
.LC4:
        .string "%lf"
main:
        cmpl    $2, %edi  //сравнили 2 и argc
        je      .L19 //если равно то переходим по метке
        movl    $1, %eax        //<retval> иначе возвращаем значение 1 и выходим изфункции
        ret
.L19:
        pushq   %rcx  // записываем в
        movq    8(%rsi), %rdi //MEM[(char * *)argv_5(D) + 8B], MEM[(char * *)argv_5(D) + 8B]
        movl    $10, %edx
        xorl    %esi, %esi //занулили  esi = 0
        call    strtol
        pxor    %xmm3, %xmm3 //res = 0
        movl    %eax, %edx      # _7, _9
        testl   %eax, %eax    # _7
        jle     .L9       #,
        movsd   .LC0(%rip), %xmm4     #, tmp108
        movsd   .LC2(%rip), %xmm6     #, tmp109
        xorl    %eax, %eax      # i
        movq    .LC3(%rip), %xmm5       #, tmp110
        movapd  %xmm4, %xmm2        # tmp108, signDef
.L10:
        pxor    %xmm0, %xmm0    # tmp99
        movapd  %xmm2, %xmm1        # signDef, tmp97
        xorpd   %xmm5, %xmm2  # tmp110, signDef
        cvtsi2sdl       %eax, %xmm0   # i, tmp99
        mulsd   %xmm6, %xmm1  # tmp109, tmp97
        addl    $1, %eax        #, i
        addsd   %xmm0, %xmm0  # tmp99, tmp100
        addsd   %xmm4, %xmm0  # tmp108, tmp101
        divsd   %xmm0, %xmm1  # tmp101, tmp103
        addsd   %xmm1, %xmm3  # tmp103, res
        cmpl    %eax, %edx      # i, _9
        jne     .L10      #,
.L9:
        movapd  %xmm3, %xmm0        # res,
        movl    $.LC4, %edi     #,
        movl    $1, %eax        #,
        call    printf  #
        xorl    %eax, %eax      # <retval>
        popq    %rdx    #
        ret

.LC0:
        .long   0
        .long   1072693248
//0 01111111111 00000000000000000000 = (-1)^0*2^(1023-1023)*1,00000000000000000000.. = 1.0

.LC2:
        .long   0
        .long   1074790400
//0 10000000001 00000000000000000000 = (-1)^0*2^(1025-1023)*1,00000000000000000000.. = 4.0

.LC3: //-1
        .long   0
        .long   -2147483648
        .long   0
        .long   0
//1 000000000000000000000000000000000000000011111111111 111111111111111111111 = (-1)^1*2(0)*1,0000...0111111111111111111111111111111111100000...000 = -1
