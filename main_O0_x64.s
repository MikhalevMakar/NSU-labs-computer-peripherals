SerExpanGregoryLeibniz:
        //формирование стекового кадра
        pushq   %rbp
        movq    %rsp, %rbp
        movq    %rdi, -40(%rbp)// как я понял максимальный размер стековогго кадра равен 40 так как максимальное значение и при этом rsp остается на месте потому что не вызываются другие функции
//инициализация значений
        pxor    %xmm0, %xmm0
        movsd   %xmm0, -8(%rbp) // заносим 0 res
        movsd   .LC1(%rip), %xmm0 //в векторный регистр xmm0 положили 1
        movsd   %xmm0, -16(%rbp) // занесение на стек 1.0
        movq    $0, -24(%rbp)//записываем в стек по адресу -24(%rbp) значение 0, то есть i = 0
        jmp     .L2
.L3:

//цикл for
        movsd   -16(%rbp), %xmm1 //записываем в xmm1 значение по адресу -16(%rbp). signDef = 1
        movsd   .LC2(%rip), %xmm0 // заносим значение 4.0
        mulsd   %xmm0, %xmm1 //умножение 4.0 * signDef и занносим в xmm1
        pxor    %xmm0, %xmm0 //xmm0 = 0
        cvtsi2sdq -24(%rbp), %xmm0 //xmm0 = i
        movapd  %xmm0, %xmm2 //то есть xmm0 = i и xmm1 = i
        addsd   %xmm0, %xmm2 //сложение xmm2 = xmm0 + xmm2 = 2 * i
        movsd   .LC1(%rip), %xmm0 //xmm0 = 1.0
        addsd   %xmm0, %xmm2 //xmm2 = xmm0 + xm1(2.0 * (i) + 1.0));
        divsd   %xmm2, %xmm1 //xmm1 / xmm2 = (4.0 * signDef) / (2.0 * (i) + 1.0)
        movapd  %xmm1, %xmm0 //xmm0 = result
        movsd   -8(%rbp), %xmm1 //достаём со стека xmm1 значние
        addsd   %xmm1, %xmm0 //xmm0 += xmm1  сложение текущего результата и предыдущей суммы. На первой итерации 0 + (4.0 * signDef ) / (2.0 * (i) + 1.0)
        movsd   %xmm0, -8(%rbp) //помещаем на стек значение суммы
        movsd   -16(%rbp), %xmm0 //достали со стека signDef
        movq    .LC3(%rip), %xmm1 //записываем -1
        xorpd   %xmm1, %xmm0 //выполняем (%xmm0) ^= (%xmm1) этим реализуем умножение signDef  *= (-1)
        movsd   %xmm0, -16(%rbp) //заносим в стек переменую signDef
        addq    $1, -24(%rbp) //реализуем ++i
.L2:
        movq    -24(%rbp), %rax //достаём переменную i
        cmpq    -40(%rbp), %rax //выполняется сравнение значений, хранящихся по адресам -40(%rbp) и %rax соответственно (N > i)
        jl      .L3
        movsd   -8(%rbp), %xmm0 // достаём res
        movq    %xmm0, %rax // ???
        movq    %rax, %xmm0
        popq    %rbp //востанавливаем состояние стека и кадра, которые были до вызова функции
        ret
.LC5:
        .string "Time taken: %lf sec.\n"
.LC6:
        .string "%lf"
main:

main:
        pushq   %rbp // записываем старый указатель базы в стек, чтобы сохранить его на будущее
        movq    %rsp, %rbp // копировавние указателя то есть два указтеля сравнялись
        subq    $64, %rsp // размер стекового кадра
        movl    %edi, -52(%rbp) //копирует аргумент  в локальный (смещение -52 байта от значения указателя кадра, хранящегося в edi (Этот регистр в цепочечных операциях содержит текущий адрес элемента в цепочке-источнике (первоначальная строка).)
        movq    %rsi, -64(%rbp) // второй аргумент кладет на стековый кадр
        cmpl    $2, -52(%rbp) // условный оператор
        je      .L6 //команды условного перехода если if не сработал
        movl    $1, %eax //иначе в регистр eax копируется значение 1 и переход по метке L8
        jmp     .L8
.L6:

//Вызов функции atoll
        movq    -64(%rbp), %rax //теперь rax указывает на вершину стека
        addq    $8, %rax
        movq    (%rax), %rax // разыменовываем rax и заносит в rax
        movq    %rax, %rdi
        call    atoll
        movq    %rax, -8(%rbp)
        
//вызываем функции clock_gettime
        leaq    -32(%rbp), %rax
        movq    %rax, %rsi
        movl    $4, %edi
        call    clock_gettime

//вызываем функции SerExpanGregoryLeibniz
        movq    -8(%rbp), %rax // берём значение со стека и помещаем rax
        movq    %rax, %rdi // затемрегистр регистр rdi первый агумент функции
        call    SerExpanGregoryLeibniz(long long)
        movq    %xmm0, %rax //записываем резульат значения из функции в rax
        movq    %rax, -16(%rbp) // дале помещаем это значение на стек

//вызываем функции clock_gettime
        leaq    -48(%rbp), %rax //со стека достаем знасение и кладем в rax (то есть заполняем сигнатуру)
        movq    %rax, %rsi // передаем в функцию
        movl    $4, %edi //Это количество аргументов, передаваемое ф-ии printf (этот регистр в цепочечных операциях содержит текущий адрес результирующая) строка
        call    clock_gettime
//вызов функции printf
        movq    -48(%rbp), %rdx //для целочисленных аргументов и аргументов указателя
        movq    -32(%rbp), %rax
        subq    %rax, %rdx //
        pxor    %xmm1, %xmm1 //зануляем регистр
        cvtsi2sdq       %rdx, %xmm1 // приведение типа к double
        movq    -40(%rbp), %rdx
        movq    -24(%rbp), %rax
        subq    %rax, %rdx // вычитание
        pxor    %xmm2, %xmm2
        cvtsi2sdq %rdx, %xmm2
        movsd   .LC4(%rip), %xmm0 //0.000000001
        mulsd   %xmm2, %xmm0 //умножение 0.000000001*(end.tv_nsec - start.tv_nsec)
        addsd   %xmm0, %xmm1
        movq    %xmm1, %rax
        movq    %rax, %xmm0 // резудьтат end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec - start.tv_nsec)
        movl    $.LC5, %edi //записываем в edi строку "Time taken: %lf sec.\n" из .LC5
        movl    $1, %eax //записываем в eax значение 1. Это количество аргументов, передаваемое ф-ии printf
        call    printf
        
        movq    -16(%rbp), %rax
        movq    %rax, %xmm0
        movl    $.LC6, %edi //записываем в edi строку "%lf" из .LC6
        movl    $1, %eax //аналогично строке 105
        call    printf
        
        movl    $0, %eax  //зануляем eax
.L8:
        leave leave     // Это эквивалентно movl %ebp, %esp; popl %ebp
// Так мы восстанавливаем состояние стека и кадра, которые были до вызова
        ret
.LC1://1.0
        .long   0
        .long   1072693248
.LC2://4.0
        .long   0
        .long   1074790400
.LC3://-1
        .long   0
        .long   -2147483648
        .long   0
        .long   0
.LC4://0.000000001
        .long   -400107883
        .long   1041313291
// rsp вершина стека
//1)как работает функция
//2)метки -LC старшая младшая часть (переводит эти части )
//3)стек в функции
