SerExpanGregoryLeibniz:
        testl   %edi, %edi //Команда TEST выполняет логическое И между всеми битами двух операндов. Результат никуда не записывается, команда влияет только на флаги
        jle     .L4 //Инструкция JLE проверяет флаги SF, OF и ZF. Переход выполняется, если SF (Флаг знака)  OF Флаг переполнения, если число переполнилось то ставится 1 .  ZF = 1 если результат равен 0
        //эта чатсь повторяется ниже
        movsd   .LC0(%rip), %xmm4
        movsd   .LC2(%rip), %xmm6     #, tmp103
        xorl    %eax, %eax      # i
        pxor    %xmm3, %xmm3 //<retval>
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
        movl    $1, %eax //<retval> иначе возвращаем значение 1 и выходим изфункции
        ret
.L19:
        pushq   %rcx  //добавить адрес начала предыдущего кадра стека
        movq    8(%rsi), %rdi //MEM[(char * *)argv_5(D) + 8B], MEM[(char * *)argv_5(D) + 8B] Memory объекты, передаваемые через стек. Передача первого (1) rdi
        movl    $10, %edx //регистр данных для 3 аргумента ф-ии strtol
        xorl    %esi, %esi //занулили  esi = 0
        call    strtol

        pxor    %xmm3, %xmm3 //res = 0
        movl    %eax, %edx //положили N в edx
        testl   %eax, %eax //Команда TEST выполняет логическое И между всеми битами двух операндов. Результат никуда не записывается, команда влияет только на флаги
        jle     .L9 //Инструкция JLE проверяет флаги SF, OF и ZF. Переход выполняется, если SF (Флаг знака)  OF Флаг переполнения, если число переполнилось то ставится 1 .  ZF = 1 если результат равен 0
//SF не равен OF или ZF = 1
        movsd   .LC0(%rip), %xmm4 //перемещение 1.0 в векторный регистр
        movsd   .LC2(%rip), %xmm6  //переместили 4.0
        xorl    %eax, %eax  //в eax лежит i = 0
        movq    .LC3(%rip), %xmm5 //перемещение -1
        movapd  %xmm4, %xmm2  // signDef перемещение упакованого выровненного double
.L10:
        pxor    %xmm0, %xmm0 //занулили xmm0 = 0
        movapd  %xmm2, %xmm1  //скопировали из signDef в tmp97
        xorpd   %xmm5, %xmm2  //таки образом в xmm2 лежит -1
        cvtsi2sdl       %eax, %xmm0   #i, tmp99 (Convert Doubleword Integer to Scalar Double-Precision Floating-Point Value)
        mulsd   %xmm6, %xmm1 //4.0*signDef
        addl    $1, %eax // добавили +1 к i
        addsd   %xmm0, %xmm0  //i+i
        addsd   %xmm4, %xmm0  # 2*i+1
        divsd   %xmm0, %xmm1  //4.0*signDef / 2*i+1
        addsd   %xmm1, %xmm3  // к res += xmm1
        cmpl    %eax, %edx     //сравнили N и i
        jne     .L10
.L9:
        movapd  %xmm3, %xmm0 //копируем  res в xmm0
        movl    $.LC4, %edi //в edi записали
        movl    $1, %eax  //зколичество аргументов функции
        call    printf
        xorl    %eax, %eax //<retval>
        popq    %rdx
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

//Первое, что нам нужно сделать, это выяснить, на какой инструкции мы сейчас находимся. Сделать это мы можем путем изучения указателя инструкции или счетчика команды. Указатель инструкции — это регистр, который хранит адрес следующей инструкции. В архитектуре x86_64 этот регистр называется %rip. Мы можем получить доступ к указателю инструкции с помощью переменной $rip, или, как альтернативу, можем использовать архитектурно независимую переменную $pc:
