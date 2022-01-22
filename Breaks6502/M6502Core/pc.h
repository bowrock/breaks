#pragma once

namespace M6502Core
{
	class ProgramCounter
	{
		BaseLogic::DLatch PCL[8];
		BaseLogic::DLatch PCLS[8];
		BaseLogic::DLatch PCH[8];
		BaseLogic::DLatch PCHS[8];

		void sim_EvenBit(BaseLogic::TriState PHI2, BaseLogic::TriState cin, BaseLogic::TriState& cout, BaseLogic::TriState& sout, size_t n, BaseLogic::DLatch PCx[], BaseLogic::DLatch PCxS[]);

		void sim_OddBit(BaseLogic::TriState PHI2, BaseLogic::TriState cin, BaseLogic::TriState& cout, BaseLogic::TriState& sout, size_t n, BaseLogic::DLatch PCx[], BaseLogic::DLatch PCxS[]);

		M6502* core = nullptr;

	public:

		ProgramCounter(M6502* parent) { core = parent; }

		void sim_Load();

		void sim_Store();

		void sim();

		void sim_HLE();

		uint8_t getPCL();
		uint8_t getPCH();
		uint8_t getPCLS();
		uint8_t getPCHS();
	};
}
