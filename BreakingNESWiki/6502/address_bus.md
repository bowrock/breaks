# Адресная шина

![6502_locator_addr](/BreakingNESWiki/imgstore/6502/6502_locator_addr.jpg)

Хотя 6502 общается с внешним миром по 16-разрядной адресной шине, но так как процессор по своей природе 8-битный - внутри него адресная шина делится на две 8-разрядные половины: верхнюю (ADH) и нижнюю (ADL).

Внутренняя шина адреса ADH/ADL соединяется с внешней 16-разрядной шиной (контакты A0-A15) через регистры ABH/ABL, которые содержат последнее записанное значение (установленный адрес).

Адресная шина однонаправленная. Управлять ей может только 6502.

Транзисторная схема младших разрядов ABL (0-2):

![abl02_tran](/BreakingNESWiki/imgstore/abl02_tran.jpg)

(Для разрядов ABL1 и ABL2 схема аналогична)

Остальные разряды ABL (3-7):

![abl37_tran](/BreakingNESWiki/imgstore/abl37_tran.jpg)

Разряды ABH:

![abh_tran](/BreakingNESWiki/imgstore/abh_tran.jpg)

Команды управления:

- 0/ADL0, 0/ADL1, 0/ADL2: Младшие 3 разряда шины ADL могут быть принудительно обнулены командами при установке [вектора прерываний](interrupts.md)
- ADL/ABL: Поместить значение внутренней шины ADL на регистр ABL
- ADH/ABH: Поместить значение внутренней шины ADH на регистр ABH

## Анализ схемы

Рассмотрим поведение схемы, когда разряд ADL = 0:

![abl_flow_tran](/BreakingNESWiki/imgstore/abl_flow_tran.jpg)

- Flip/Flop разряда ABL организован на двух инверторах (not2 и not3), причем not2 выполняет одновременно роль DLatch (вход Enable которого соединен с PHI2)
- PHI2: В этом полутакте FF "рефрешится"
- PHI1: В этом полутакте старое значение FF "отсекается" тристейтом PHI2 (расположен левее not2), а новое загружается с шины ADL (в инвертированном виде, см. not1), но только если активна команда ADL/ABL
- Выход с not2 организует окончательное формирование выходного значения для внешней шины адреса. Эта часть схемы содержит инвертор not3 для формирования FF, а также инвертор not4, который управляет усилительной "гребенкой" контактов Ax

## Логическая схема

На логических схемах PHI2 не используется, а FF организованный на двух инверторах заменен обычным триггером.

![abl02_logisim](/BreakingNESWiki/imgstore/abl02_logisim.jpg)

![abl_logisim](/BreakingNESWiki/imgstore/abl_logisim.jpg)

![abh_logisim](/BreakingNESWiki/imgstore/abh_logisim.jpg)

## Оптимизированная логическая схема

![0_abl02_tran](/BreakingNESWiki/imgstore/6502/ttlworks/0_abl02_tran.png)
