// Sprite DMA

// This component acts as a small DMA controller, which besides sprite DMA also handles address bus arbitration and processor readiness control (RDY).

module Sprite_DMA(
	n_ACLK, ACLK, PHI1,
	RES, RnW, W4014, DB, 
	RUNDMC, n_DMCAB, DMCRDY, DMC_Addr, CPU_Addr,
	Addr, RDY_tocore, SPR_PPU);

	input n_ACLK;
	input ACLK;
	input PHI1;					// Sprite DMA can only start if the processor goes into a read cycle (PHI1 = 0 and R/W = 1)
								// This is done to delay the start of the DMA because the RDY clearing is ignored on the 6502 write cycles.

	input RES;
	input RnW;
	input W4014;				// Writing to register $4014 clears the lower part of the address and puts the value to be written into the higher part. The DMA process then starts.
	inout [7:0] DB;

	input RUNDMC;				// As long as the RUNDMC signal is 1 the sprite DMA is in standby mode
	input n_DMCAB;				// The address bus is controlled by the DMC circuitry to perform DMC DMA
	input DMCRDY;				// DMC Ready. If the DMC is not ready - the RDY signal is also forced to 0.
	input [15:0] DMC_Addr;
	input [15:0] CPU_Addr;

	output [15:0] Addr;
	output RDY_tocore;
	output SPR_PPU;				// A constant address value is set for writing to the PPU register $2004

	wire SPRDmaEnd;				// End DMA execution ("End")
	wire SPRDmaStep;			// Increment the low-order part of the address ("Step")
	wire [15:0] SPR_Addr;
	wire SPR_CPU;				// Memory address to read during sprite DMA

	SPRDMA_AddrLowCounter dma_addr_low (
		.n_ACLK(n_ACLK),
		.Clear(RES),
		.Step(SPRDmaStep),
		.Load(W4014),
		.EndCount(SPRDmaEnd),
		.AddrLow(SPR_Addr[7:0]) );

	SPRDMA_AddrHigh dma_addr_high (
		.n_ACLK(n_ACLK),
		.SetAddr(W4014),
		.DB(DB),
		.AddrHigh(SPR_Addr[15:8]) );

	SPRDMA_Control sprdma_ctl (
		.PHI1(PHI1),
		.RnW(RnW),
		.n_ACLK(n_ACLK), 
		.ACLK(ACLK),
		.RES(RES),
		.W4014(W4014),
		.RUNDMC(RUNDMC),
		.DMCReady(DMCRDY),
		.SPRE(SPRDmaEnd),
		.SPRS(SPRDmaStep),
		.RDY(RDY_tocore),
		.SPR_PPU(SPR_PPU),
		.SPR_CPU(SPR_CPU) );

	Address_MUX addr_mux (
		.SPR_CPU(SPR_CPU),
		.SPR_PPU(SPR_PPU),
		.n_DMCAB(n_DMCAB),
		.DMC_Addr(DMC_Addr),
		.SPR_Addr(SPR_Addr),
		.CPU_Addr(CPU_Addr),
		.AddrOut(Addr) );

endmodule // Sprite_DMA

module SPRDMA_AddrLowCounter(n_ACLK, Clear, Step, Load, EndCount, AddrLow);

	input n_ACLK;
	input Clear;
	input Step;
	input Load;
	output EndCount;
	output [7:0] AddrLow;

	wire [7:0] cc; 		// Carry chain

	apu_cntbit cnt [7:0] (
		.Clk(n_ACLK),
		.Clear(Clear),
		.Step(Step),
		.Load(Load),
		.CarryIn({cc[6:0], 1'b1}),
		.Value(8'h00),
		.ValOut(AddrLow),
		.CarryOut(cc) );

	assign EndCount = cc[7];

endmodule // SPRDMA_AddrLowCounter

module SPRDMA_AddrHigh(n_ACLK, SetAddr, DB, AddrHigh);

	input n_ACLK;
	input SetAddr;
	inout [7:0] DB;
	output [7:0] AddrHigh;

	sdffe val_hi [7:0] (
		.d(DB),
		.en(SetAddr),
		.phi_keep(n_ACLK),
		.q(AddrHigh) );

endmodule // SPRDMA_AddrHigh

module SPRDMA_Control(PHI1, RnW, n_ACLK, ACLK, RES, W4014, RUNDMC, DMCReady, SPRE, SPRS, RDY, SPR_PPU, SPR_CPU);

	input PHI1;
	input RnW;
	input n_ACLK;
	input ACLK;
	input RES;
	input W4014;
	
	input RUNDMC;
	input DMCReady;

	input SPRE;
	output SPRS;

	output RDY;
	output SPR_PPU;			// DMA Buffer -> PPU
	output SPR_CPU;			// RAM -> DMA Buffer

	wire n_ACLK2 = ~ACLK;
	wire NOSPR;
	wire DOSPR;
	wire spre_out;
	wire dospr_out;
	wire StopDma;
	wire StartDma;
	wire n_StartDma;
	wire toggle;

	nor (SPRS, NOSPR, ~n_ACLK2);

	dlatch spre_latch (.d(SPRE), .en(n_ACLK), .q(spre_out) );
	dlatch nospr_latch (.d(StopDma), .en(n_ACLK), .nq(NOSPR) );
	dlatch dospr_latch (.d(n_StartDma), .en(n_ACLK2), .q(dospr_out) );

	rsff_2_3 StopDMA (.res1(SPRS & spre_out), .res2(RES), .s(DOSPR), .q(StopDma) );
	rsff_2_3 StartDMA (.res1(~NOSPR), .res2(RES), .s(W4014), .q(StartDma), .nq(n_StartDma) );
	rsff DMADirToggle (.r(n_ACLK), .s(n_ACLK2), .q(toggle) );

	nor(SPR_PPU, NOSPR, RUNDMC, ~toggle);
	nor(SPR_CPU, NOSPR, RUNDMC, toggle);

	// Ready control

	wire sprdma_rdy;
	nor (sprdma_rdy, ~NOSPR, StartDma);
	assign RDY = sprdma_rdy & DMCReady; 		// -> to core

	// 6502 read cycle detect

	wire read_cyc;
	nand (read_cyc, ~PHI1, RnW);
	nor (DOSPR, dospr_out, read_cyc);

endmodule // SPRDMA_Control

module Address_MUX(SPR_CPU, SPR_PPU, n_DMCAB, DMC_Addr, SPR_Addr, CPU_Addr, AddrOut);

	input SPR_CPU;
	input SPR_PPU;
	input n_DMCAB;
	input [15:0] DMC_Addr;
	input [15:0] SPR_Addr;
	input [15:0] CPU_Addr;
	output [15:0] AddrOut;

	wire DMC_AB = ~n_DMCAB;

	wire CPU_AB;
	nor (CPU_AB, SPR_CPU, SPR_PPU, DMC_AB);

	assign AddrOut = SPR_PPU ? 16'h2004 : 
		(SPR_CPU ? SPR_Addr :
			(DMC_AB ? DMC_Addr :
				(CPU_AB ? CPU_Addr : 16'hzzzz) ) );

endmodule // Address_MUX

module apu_cntbit (Clk, Clear, Step, Load, CarryIn, Value, ValOut, n_ValOut, CarryOut);

	input Clk;
	input Clear;
	input Step;
	input Load;
	input CarryIn;
	input Value;

	output ValOut;
	output n_ValOut;
	output CarryOut;

	wire cntint;
	wire cgin;
	wire cgout;

	assign cntint = Load ? Value : (Clear ? 1'b0 : (Step ? cgout : (Clk ? ValOut : 1'bz) ));
	assign cgin = CarryIn ? ValOut : n_ValOut;

	dlatch latch1 (.d(cntint), .en(1'b1), .q(ValOut), .nq(n_ValOut));
	dlatch latch2 (.d(cgin), .en(Clk), .nq(cgout));

	nor (CarryOut, ~CarryIn, n_ValOut);

endmodule // apu_cntbit
