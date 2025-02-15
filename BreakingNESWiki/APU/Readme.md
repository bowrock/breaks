# Обзор APU

**APU** - неофициальное название специализированного центрального процессора NES.

Официальное название - MPU (Microprocessor Unit) или просто CPU, но мы будем придерживаться неофициального термина. Иногда ещё APU встречается под аббревиатурой "pAPU".

Разработкой микросхемы занималась [Ricoh](../Ricoh.md), кодовое обозначение - RP 2A03 для NTSC и RP 2A07 для PAL.

В состав APU входят:
- Ядро процессора MOS 6502, с отключенной схемой десятичной коррекции (BCD)
- DMA для отправки спрайтов (жестко настроено на внешние регистры PPU)
- Тональные генераторы: 2 прямоугольных, треугольный, генератор шума и delta-PCM.
- 2 ЦАП для преобразования цифровых выходов синтезированного звука в аналоговые уровни
- DMA для выборки DPCM-сэмплов
- Небольшой DMA-контроллер
- Программный таймер ("Frame counter")
- Делитель входной тактовой частоты
- Порты ввода-вывода (которые в NES используются для получения данных с контроллеров)
- Отладочные регистры (недоступные в Retail-консолях)

Наличие ЦАП переводит APU в разряд полу-аналоговых схем.

Также необходимо принимать в расчёт тот факт, что ядро 6502, входящее в состав APU находится под управлением DMA-контроллера и соответственно является "рядовым" устройством, разделяющим шину с другими устройствами, использующими DMA.

<img src="/BreakingNESWiki/imgstore/apu/apu_blocks.jpg" width="900px">

## Примечание по транзисторным схемам

Транзисторные схемы каждого компонента напилены на составные части, чтобы не занимали много места.

Чтобы вы не заблудились, каждый раздел включает в начале специальный "локатор", где отмечено приблизительное расположение изучаемого компонента.

Пример локатора:

![apu_locator_dma](/BreakingNESWiki/imgstore/apu/apu_locator_dma.jpg)

## Примечание по логическим схемам

Логические схемы в основном сделаны в программе Logisim. Для обозначения DLatch применяется такой элемент:

|DLatch (транзисторная схема)|DLatch (логический эквивалент)|
|---|---|
|![dlatch_tran](/BreakingNESWiki/imgstore/dlatch_tran.jpg)|![dlatch_logic](/BreakingNESWiki/imgstore/dlatch_logic.jpg)|

Для удобства логический вариант DLatch имеет два выхода (`out` и `/out`), так как текущее значение DLatch (out) нередко используется как вход для операции NOR.
