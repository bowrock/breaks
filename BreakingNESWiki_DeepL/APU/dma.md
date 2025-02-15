# Sprite DMA

![apu_locator_dma](/BreakingNESWiki/imgstore/apu/apu_locator_dma.jpg)

This component acts as a small DMA controller, which besides sprite DMA also handles address bus arbitration and processor readiness control (RDY).

![SPRDMA](/BreakingNESWiki/imgstore/apu/SPRDMA.jpg)

The sprite DMA is very closely tied in with the [DMC DMA](dpcm.md) and is "slave" to it (as long as the RUNDMC signal is `1` the sprite DMA is in standby mode).

Unfortunately, the sprite DMA destination address cannot be configured and is hardwired to PPU register $2004.

## SPR DMA Address

Low address bits:

|![sprdma_addr_low_tran](/BreakingNESWiki/imgstore/apu/sprdma_addr_low_tran.jpg)|![SPRDMA_AddrLowCounter](/BreakingNESWiki/imgstore/apu/SPRDMA_AddrLowCounter.jpg)|
|---|---|

High address bits:

|![sprdma_addr_hi_tran](/BreakingNESWiki/imgstore/apu/sprdma_addr_hi_tran.jpg)|![SPRDMA_AddrHigh](/BreakingNESWiki/imgstore/apu/SPRDMA_AddrHigh.jpg)|
|---|---|

The arrows mark the places where the constant address of the PPU $2004 register is formed.

## SPR DMA Control

![sprdma_control_tran](/BreakingNESWiki/imgstore/apu/sprdma_control_tran.jpg)

Arrows indicate "other" /ACLKs, which differ from normal /ACLKs by a slight delay.

Logic:

![SPRDMA_Control](/BreakingNESWiki/imgstore/apu/SPRDMA_Control.jpg)

Control signals for interaction with DMC:
- RUNDMC: The DMC is doing its thing. Until it finishes, the sprite DMA is in standby mode
- #DMC/AB: DMC occupies the address bus
- DMCRDY: DMC Ready. If the DMC is not ready - the RDY signal is also forced to 0.

Processor control signals:
- PHI1 and R/W: Sprite DMA can only start if the processor goes into a read cycle (PHI1 = 0 and R/W = 1). Without this condition the `DOSPR` control signal will not be active. This is done to delay the start of the DMA because the RDY clearing is ignored on the 6502 write cycles.

![Write_4014_Timing](/BreakingNESWiki/imgstore/apu/Write_4014_Timing.jpg)

|STA T0 PHI1|STA T0 PHI2|STA T1 PHI1|STA T1 PHI2|
|---|---|---|---|
|![SPRDMA_Control_T0_Phi1](/BreakingNESWiki/imgstore/apu/SPRDMA_Control_T0_Phi1.jpg)|![SPRDMA_Control_T0_Phi2](/BreakingNESWiki/imgstore/apu/SPRDMA_Control_T0_Phi2.jpg)|![SPRDMA_Control_T1_Phi1](/BreakingNESWiki/imgstore/apu/SPRDMA_Control_T1_Phi1.jpg)|![SPRDMA_Control_T1_Phi2](/BreakingNESWiki/imgstore/apu/SPRDMA_Control_T1_Phi2.jpg)|

Signals affecting the DMA process:
- W4014: Writing to register $4014 clears the lower part of the address and puts the value to be written into the higher part. The DMA process then starts.
- SPRS: Increment the low-order part of the address ("Step")
- SPRE: End DMA execution ("End")

Immediately after the start of sprite DMA the SPR/PPU and SPR/CPU control signals alternate their values so that the value is first read from memory into the sprite buffer and then written to the PPU register $2004.

## SPR DMA Buffer

![sprbuf_tran](/BreakingNESWiki/imgstore/apu/sprbuf_tran.jpg)

Logic:

![SPRDMA_Buffer](/BreakingNESWiki/imgstore/apu/SPRDMA_Buffer.jpg)

## Address MUX

The address multiplexer is used to arbitrate the external address bus. The control signals are used to select who is "using" the address bus now:

- SPR/CPU: Memory address to read during sprite DMA
- SPR/PPU: A constant address value is set for writing to the PPU register $2004
- #DMC/AB: The address bus is controlled by the DMC circuitry to perform DMC DMA
- Default (CPU/AB): The address bus is controlled by the CPU

Address multiplexer control:

![sprdma_addr_mux_control_tran](/BreakingNESWiki/imgstore/apu/sprdma_addr_mux_control_tran.jpg)

The multiplexer itself consists of 16 repeating circuits, for each bit:

![sprdma_addr_mux_bit_tran](/BreakingNESWiki/imgstore/apu/sprdma_addr_mux_bit_tran.jpg)

Logic:

![Addr_MUX](/BreakingNESWiki/imgstore/apu/Addr_MUX.jpg)
