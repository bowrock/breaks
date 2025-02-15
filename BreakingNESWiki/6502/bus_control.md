# Управление шинами

![6502_locator_bus_control](/BreakingNESWiki/imgstore/6502/6502_locator_bus_control.jpg)

Управление шинами больше всего "раскидано" по поверхности процессора. Проще всего будет вначале дать описание всех [команд управления](context_control.md) шинами, а потом по отдельности рассмотреть соответствующие схемы.

Команды управления шинами:

|Команда|Описание|
|---|---|
|Управление внешней шиной адреса||
|ADH/ABH|Установить старшие 8 разрядов внешней шины адреса, в соответствии со значением внутренней шины ADH|
|ADL/ABL|Установить младшие 8 разрядов внешней шины адреса, в соответствии со значением внутренней шины ADL|
|Соединение АЛУ с шинами SB, DB||
|AC/DB|Поместить значение AC на шину DB|
|SB/AC|Поместить значение с шины SB/схемы BCD коррекции в аккумулятор|
|AC/SB|Поместить значение AC на шину SB|
|Управление внутренними шинами SB, DB и ADH||
|SB/DB|Соединить шины SB и DB|
|SB/ADH|Соединить шины SB и ADH|
|0/ADH0|Принудительно очистить разряд ADH\[0\]|
|0/ADH17|Принудительно очистить разряды ADH\[1-7\]|
|Управление внешней шиной данных||
|DL/ADL|Записать значение DL на шину ADL|
|DL/ADH|Записать значение DL на шину ADH|
|DL/DB|Обменять значение DL и внутренней шины DB. Направление обмена зависит от режима работы внешней шины данных (чтение/запись)|

Мотив всех схем примерно следующий:
- На вход схем управления поступает много выходов с декодера и других вспомогательных сигналов
- Все схемы по большей части комбинаторные (не содержат триггеров, просто месиво вентилей)
- Выходы со схем управления поступают на выходные защёлки команд управления нижней частью процессора.

## Вспомогательные сигналы

Схемы для получения вспомогательных сигналов:

|NOADL и IND|JSXY|
|---|---|
|![bus_noadl_ind_tran](/BreakingNESWiki/imgstore/bus_noadl_ind_tran.jpg)|![bus_jsxy_tran](/BreakingNESWiki/imgstore/bus_jsxy_tran.jpg)|

В схеме `IND` выход декодера X90 дополнительно модифицирован сигналом Push/Pull (X129).

Остальные вспомогательные и промежуточные сигналы, которые можно встретить на схемах в этом разделе:

|Сигнал|Описание|
|---|---|
|RTS/5|Декодер X84|
|RTI/5|Декодер X26|
|STXY|Приходит со схемы [управления регистрами](regs_control.md)|
|BR0|Декодер X73. Дополнительно модифицирован сигналом /PRDY|
|T5|Приходит со счётчика циклов длинных инструкций|
|T6|Приходит со счётчика циклов длинных инструкций|
|PGX|Выходной сигнал со схемы ADL/ABL|
|JSR/5|Декодер X56|
|T2|Декодер X28|
|!PCH/PCH|Приходит со схемы [управления PC](pc_control.md)|
|SBA|Сигнал выходит из схемы получения #SB/ADH, используется в схеме #ADH/ABH|
|/ready|Глобальный внутренний сигнал готовности процессора|
|BR3|Декодер X93|
|0/ADL0|Приходит со схемы установки вектора прерывания|
|AND|Приходит со схемы [управления АЛУ](alu_control.md)|
|STA|Декодер X79|
|STOR|Промежуточный сигнал с диспатчера|
|SBXY|Приходит со схемы управления регистрами (не путать с STXY)|
|T1|Приходит со схемы инкремента PC (см. [диспатчер](dispatch.md))|
|BR2|Декодер X80|
|ZTST|Выходной сигнал для [управления флагами](flags_control.md) со схемы SB/DB|
|ACRL2|Один из выходов ACR Latch|
|T0|Приходит со счётчика циклов коротких инструкций|
|ABS/2|Декодер X83. Дополнительно модифицирован сигналом Push/Pull (X129)|
|JMP/4|Декодер X101|
|IMPL|Декодер X128. Дополнительно модифицирован сигналами Push/Pull (X129) и IR0.|
|JSR2|Декодер X48|
|/JSR|Инверсия JSR2 для схемы управления регистрами|
|BRK6E|Приходит со схемы [обработки прерываний](interrupts.md)|
|INC_SB|Приходит со схемы управления АЛУ|
|DL/PCH|Приходит со схемы управления PC|

Сигналы расположены в порядке появления на схемах.

## Управление внешней шиной адреса

Схемы для формирования промежуточных сигналов:

|#ADL/ABL|#ADH/ABH (1)|#ADH/ABH (2)|
|---|---|---|
|![bus_adlabl_tran](/BreakingNESWiki/imgstore/bus_adlabl_tran.jpg)|![bus_adhabh_tran1](/BreakingNESWiki/imgstore/bus_adhabh_tran1.jpg)|![bus_adhabh_tran2](/BreakingNESWiki/imgstore/bus_adhabh_tran2.jpg)|

Первый кусок схемы #ADH/ABH находится правее флага B, второй кусок находится в схеме формирования адреса прерывания. Сигнал #ADH/ABH соединяется напрямую между этими двумя частями.

Выходные защёлки команд управления ADL/ABL и ADH/ABH:

![bus_addr_bus_commands_tran](/BreakingNESWiki/imgstore/bus_addr_bus_commands_tran.jpg)

## Соединение АЛУ с шинами SB, DB

Схемы для формирования промежуточных сигналов:

|#AC/DB|#SB/AC, #AC/SB|
|---|---|
|![bus_acdb_tran](/BreakingNESWiki/imgstore/bus_acdb_tran.jpg)|![bus_acsb_tran](/BreakingNESWiki/imgstore/bus_acsb_tran.jpg)|

Выходные защёлки команд управления AC/DB, SB/AC, AC/SB:

![bus_alu_commands_tran](/BreakingNESWiki/imgstore/bus_alu_commands_tran.jpg)

## Управление внутренними шинами SB, DB и ADH

Схемы для формирования промежуточных сигналов (для 0/ADH0 получается сразу управляющая команда):

|#SB/DB (также #0/ADH17)|0/ADH0|#SB/ADH|
|---|---|---|
|![bus_control_tran1](/BreakingNESWiki/imgstore/bus_control_tran1.jpg)|![bus_0adh0_tran](/BreakingNESWiki/imgstore/bus_0adh0_tran.jpg)|![bus_sbadh_tran](/BreakingNESWiki/imgstore/bus_sbadh_tran.jpg)|

Выходные защёлки команд управления SB/DB, SB/ADH, 0/ADH17:

![bus_sb_commands_tran](/BreakingNESWiki/imgstore/bus_sb_commands_tran.jpg)

(Схема получения команды и защёлка 0/ADH0 выше)

## Управление внешней шиной данных

Схемы для формирования промежуточных сигналов:

|#DL/ADL|#DL/DB (1)|#DL/DB (2)|
|---|---|---|
|![bus_dladl_tran](/BreakingNESWiki/imgstore/bus_dladl_tran.jpg)|![bus_dldb_tran](/BreakingNESWiki/imgstore/bus_dldb_tran.jpg)|![bus_dldb_tran2](/BreakingNESWiki/imgstore/bus_dldb_tran2.jpg)|

Первый кусок схемы #DL/DB находится рядом с ACR Latch, второй кусок прямо внутри схемы управления АЛУ. Сигнал #DL/DB соединяется напрямую между этими двумя частями.

Выходные защёлки команд управления DL/ADL, DL/ADH, DL/DB:

![bus_data_latch_commands_tran](/BreakingNESWiki/imgstore/bus_data_latch_commands_tran.jpg)

## Логическая схема

![bus_control_logisim](/BreakingNESWiki/imgstore/logisim/bus_control_logisim.jpg)

## Оптимизированная логическая схема

![7_bus_control_logisim](/BreakingNESWiki/imgstore/6502/ttlworks/7_bus_control_logisim.png)
