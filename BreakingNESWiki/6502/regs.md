# Регистры

![6502_locator_regs](/BreakingNESWiki/imgstore/6502/6502_locator_regs.jpg)

Регистры X и Y используются для индексной адресации. Регистр S представляет собой указатель стека, при этом стек располагается по адресам 0x100 ... 0x1FF (на первой странице).

Схематически регистры X, Y и S состоят из 8 одинаковых кусков-разрядов:

![regs_tran](/BreakingNESWiki/imgstore/regs_tran.jpg)

(На схеме выше нужно заменить SB0 и ADL0 на SBx и ADLx для остальных разрядов регистров)

В основе каждого разряда регистра лежит триггер, загрузка и выгрузка значений на шины осуществляется [управляющими сигналами](context_control.md):
- Y/SB: поместить значение регистра Y на шину SB
- SB/Y: загрузить значение регистра Y из шины SB
- X/SB: поместить значение регистра X на шину SB
- SB/X: загрузить значение регистра X из шины SB
- S/ADL: поместить старое значение регистра S на шину ADL
- S/SB: поместить старое значение регистра S на шину SB
- SB/S: загрузить новое значение регистра S из шины SB
- S/S: рефрешить регистр S, активна если SB/S = 0

Таким образом регистры могут соединяться только с двумя шинами: SB и ADL.

:warning: Обратите особое внимание на устройство регистра S. У него есть входная защёлка (для загрузки нового значения) и выходная (для сохранения старого значения). Загрузка нового значения (SB/S) и сохранение старого (S/ADL) могут происходить одновременно.

## Логическая схема

![regs_logic](/BreakingNESWiki/imgstore/regs_logic.jpg)

- Во время PHI1 регистры X и Y выдают на шину SB своё значение / перегружаются новыми значениями с шины SB.
- У регистра S есть входная защёлка и выходная. Во время PHI1 значение с выходной защёлки помещаются на шины SB или ADL, а входная защёлка либо загружается новым значением с шины SB, либо рефрешится с выходной защёлки (S/S).
- Во время PHI2 регистры X и Y "хранят" своё старое значение, так как управляющие сигналы отключают их от шины.
- Регистр S во время PHI2 просто выводит своё значение на шины SB или ADL. Входная защёлка перекрывается, так как команды обмена отключены во время PHI2.

Шины SB и ADL подзаряжаются во время PHI2. Сделано это по причине того, что "разрядить" шину дольше чем "зарядить". Поэтому когда шина не нужна - она подзаряжается, чтобы на ней не оставалось "плавающих" значений.
Если помещаемое значение на шину равно 1, то шина уже заранее подготовлена ("заряжена"). Если помещаемое значение на шину равно 0, то шина "разряжается" на землю.

В современных процессорах задачу подзарядки шины выполняют специальные стандартные ячейки, называемые Bus Keeper.

На транзисторной схеме выше можно увидеть только транзисторы для подзарядки шины SB (находятся в схеме для разрядов регистра S). Транзисторы для подзарядки шины ADL разбросаны рядом со счётчиком инструкций (PC).

## Оптимизированная логическая схема

![25_regs_logic](/BreakingNESWiki/imgstore/6502/ttlworks/25_regs_logic.png)
