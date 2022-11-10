SerExpanGregoryLeibniz:
        cmp     r0, #0
        ble     .L4
        vmov.f64        d18, #1.0e+0
        vmov.i64        d0, #0  @ float
        movs    r3, #0
        vmov.f64        d21, #4.0e+0
        vmov.f64        d20, d18
.L3:
        vmov    s15, r3 @ int
        vmul.f64        d17, d18, d21
        adds    r3, r3, #1
        vneg.f64        d18, d18
        vcvt.f64.s32    d16, s15
        cmp     r0, r3
        vadd.f64        d16, d16, d16
        vadd.f64        d16, d16, d20
        vdiv.f64        d19, d17, d16
        vadd.f64        d0, d0, d19
        bne     .L3
        vcvt.f32.f64    s0, d0
        bx      lr
.L4:
        vldr.32 s0, .L7
        bx      lr
.L7:
        .word   0
.LC0:
        .ascii  "%lf\000"
main:
        cmp     r0, #2  //сравнили первый аргумент ф-ии main arc и 2
        beq     .L20  //перейти, если равно (Branch if Equal)
        movs    r0, #1  //r0 = 1
        bx      lr  // bx обеспечивает выполнение безусловного перехода с возможным переключением системы команд
.L20:
        ldr     r0, [r1, #4]  //загрузили r0 = r1 + 4 = N
        movs    r2, #10  //r2 = 10, 2 аргумент strtoll
        movs    r1, #0  //первый аргумент r1 = 0
        push    {r3, lr}  //добавили в стек r3, lr
        bl      strtoll  //вызов ф-ии strtoll
        cmp     r0, #0  //сравнили r0 = N и 0
        ble     .L14  //в случае неудачи выход из программы
        vmov.f64        d18, #1.0e+0  //d18 = 1 = signDef
        vmov.i64        d19, #0  //d19 = 0 = res
        movs    r3, #0  //r3 = 0 = i
        vmov.f64        d22, #4.0e+0 //d22 = 4
        vmov.f64        d21, d18 //d21 = d18 = 1 константа
.L12:
        vmov    s15, r3 //s15 = r3 = i
        vmul.f64        d17, d18, d22 // d17 = d18 * d22 = 4 * signDef
        adds    r3, r3, #1  //r2  = r3 + 1 = 1 + i
        vneg.f64        d18, d18  //смена знака signDef
        vcvt.f64.s32    d16, s15  //конвертируем s15 в d16 = i
        cmp     r0, r3 //сравнили r3 инкрементированое i и N, устанавливает флаг C
        vadd.f64        d16, d16, d16  //сложили d16 = d16 + d16 = i + i
        vadd.f64        d16, d16, d21  //2*i+1
        vdiv.f64        d20, d17, d16  //d20 = d17 / d16 =  4 * signDef / (2*i+1)
        vadd.f64        d19, d19, d20  //d19 = d19 + d20
        bne     .L12 //команды этой группы выполняют условный переход в зависимости от состояния флагов регистра CPSR
.L11:
        vcvt.f32.f64    s15, d19  //d19 конвертируется в 32 битный регистр с плавущей точкой
        movw    r0, #:lower16:.LC0
        movt    r0, #:upper16:.LC0  //поместить первый аргумент printf, указатель на строку из метки .LC0 в r0
        vcvt.f64.f32    d16, s15
        vmov    r2, r3, d16
        bl      printf
        movs    r0, #0  //возвращаемое значение из ф-ии
        pop     {r3, pc}  //достали регистры r3 pc
.L14:
        vmov.i64        d19, #0 @ float
        b       .L11

//bl выполняют переход на указанный адрес. BL, кроме того, сохраняет адрес возврата (адрес команды, следующей за BL) в регистре LR (R14).
