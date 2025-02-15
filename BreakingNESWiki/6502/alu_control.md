# Управление АЛУ

![6502_locator_alu_control](/BreakingNESWiki/imgstore/6502/6502_locator_alu_control.jpg)

Управление [АЛУ](alu.md) предназначено для формирования [команд управления](context_control.md) АЛУ.

## Промежуточные сигналы

|/ROR|SR|AND|CSET|
|---|---|---|---|
|![alu_setup_ror_tran](/BreakingNESWiki/imgstore/alu_setup_ror_tran.jpg)|![alu_setup_sr_tran](/BreakingNESWiki/imgstore/alu_setup_sr_tran.jpg)|![alu_setup_and_tran](/BreakingNESWiki/imgstore/alu_setup_and_tran.jpg)|![alu_setup_cset_tran](/BreakingNESWiki/imgstore/alu_setup_cset_tran.jpg)|

Таблица вспомогательных и промежуточных сигналов, которые встречаются далее на схемах:

|Сигнал|Описание|
|---|---|
|/ROR|Промежуточный сигнал, используется в схеме ADD/SB7|
|SR|Промежуточный сигнал|
|AND|Промежуточный сигнал|
|T0|Приходит со счётчика циклов коротких инструкций|
|T5|Приходит со счётчика циклов длинных инструкций|
|/C_OUT|Значение [флага](flags.md) C (инвертированное значение)|
|CSET|Промежуточный сигнал ("Carry Set"), используется в основной схеме управления АЛУ|
|STK2|Декодер X35|
|RET|Декодер X47|
|SBC0|Декодер X51|
|JSR2|Декодер X48|
|/BR3|Декодер X93 (инвертированное значение). Схема инвертирования где-то потерялась в процессе оптимизации.|
|BRK6E|Приходит со схемы [обработки прерываний](interrupts.md)|
|STKOP|Приходит со схемы [управления регистрами](regs_control.md)|
|/ready|Глобальный внутренний сигнал готовности процессора|
|INC_SB|Промежуточный сигнал ("Increment SB"), используется в основной схеме управления, а также в схеме [управления шинами](bus_control.md)|
|JSR/5|Декодер X56|
|PGX|Приходит со схемы управления шинами|
|NOADL|Приходит со схемы управления шинами|
|BRFW|Приходит со схемы [условных переходов](branch_logic.md)|
|T1|Приходит со схемы инкремента PC (см. [диспатчер](dispatch.md))|
|T6|Приходит со счётчика циклов длинных инструкций|
|D_OUT|Значение флага D|
|C_OUT|Значение флага C|

## Управление АЛУ (основная часть)

Схема представляет собой месиво вентилей и 4 защёлки для формирования входного переноса для АЛУ (контрольный сигнал `/ACIN`).

![alu_setup_main_tran](/BreakingNESWiki/imgstore/alu_setup_main_tran.jpg)

## Управление BCD коррекцией

BCD коррекция производится в следующих случаях:
- Если BCD режим включен флагом D и текущая инструкция `SBC` (контрольный сигнал DSATemp)
- Если BCD режим включен флагом D и текущая инструкция `ADC` (контрольный сигнал DAATemp)

![alu_setup_bcd_tran](/BreakingNESWiki/imgstore/alu_setup_bcd_tran.jpg)

## ADD/SB7

Внимательный читатель заметит, что процессор имеет поддержку инструкций вращения бит (ROL/ROR). Дополнительной обработкой связанной с этими инструкциями как раз занимается указанная схема.

![alu_setup_addsb7_tran](/BreakingNESWiki/imgstore/alu_setup_addsb7_tran.jpg)

Логическая схема:

![alu_control_addsb7](/BreakingNESWiki/imgstore/logisim/alu_control_addsb7.jpg)

Оптимизированная логическая схема:

![27_alu_control_addsb7](/BreakingNESWiki/imgstore/6502/ttlworks/27_alu_control_addsb7.png)

## Управляющие команды АЛУ

![alu_control_commands_tran](/BreakingNESWiki/imgstore/alu_control_commands_tran.jpg)

|Команда|Описание|
|---|---|
|Установка входных значений АЛУ||
|NDB/ADD|Загрузить инверсное значение с шины DB в защёлку BI|
|DB/ADD|Загрузить прямое значение с шины DB в защёлку BI|
|0/ADD|Записать 0 в защёлку AI|
|SB/ADD|Загрузить значение с шины SB в защёлку AI|
|ADL/ADD|Загрузить значение с шины ADL в защёлку BI|
|Команды операций АЛУ||
|ANDS|Операция логического И (AI & BI)|
|EORS|Операция логического дополнения XOR (AI ^ BI)|
|ORS|Операция логического ИЛИ (AI \| BI)|
|SRS|Сдвиг вправо|
|SUMS|Суммирование (AI + BI)|
|Команды управления промежуточным результатом АЛУ||
|ADD/SB06|Поместить значение защёлки ADD на шину SB (разряды 0-6)|
|ADD/SB7|Поместить значение защёлки ADD на шину SB (разряд 7)|
|ADD/ADL|Поместить значение защёлки ADD на шину ADL|
|Дополнительные сигналы||
|/ACIN|Входной перенос|
|/DAA|Выполнить коррекцию после сложения|
|/DSA|Выполнить коррекцию после вычитания|

## Логическая схема

![alu_control_logisim](/BreakingNESWiki/imgstore/logisim/alu_control_logisim.jpg)

## Оптимизированная логическая схема

![28_alu_control_logisim](/BreakingNESWiki/imgstore/6502/ttlworks/28_alu_control_logisim.png)
