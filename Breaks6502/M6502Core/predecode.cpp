#include "pch.h"

using namespace BaseLogic;

namespace M6502Core
{
	void PreDecode::sim(uint8_t* data_bus)
	{
		TriState PHI2 = core->wire.PHI2;
		TriState Z_IR = core->wire.Z_IR;

		if (PHI2 != TriState::One)
		{
			for (size_t n = 0; n < 8; n++)
			{
				PD[n] = NOR(pd_latch[n].get(), Z_IR);
				n_PD[n] = NOT(PD[n]);
			}

			return;
		}

		for (size_t n = 0; n < 8; n++)
		{
			TriState Dn = (*data_bus) & (1 << n) ? TriState::One : TriState::Zero;

			pd_latch[n].set(Dn == TriState::Z ? TriState::One : NOT(Dn), PHI2);
		}

		for (size_t n = 0; n < 8; n++)
		{
			PD[n] = NOR(pd_latch[n].get(), Z_IR);
			n_PD[n] = NOT(PD[n]);
		}

		TriState IMPLIED = NOR3(PD[0], PD[2], NOT(PD[3]));

		TriState res2 = NOR3(PD[1], PD[4], PD[7]);

		TriState in3[4];
		in3[0] = NOT(PD[0]);
		in3[1] = PD[2];
		in3[2] = NOT(PD[3]);
		in3[3] = PD[4];
		TriState res3 = NOR4(in3);

		TriState in4[5];
		in4[0] = PD[0];
		in4[1] = PD[2];
		in4[2] = PD[3];
		in4[3] = PD[4];
		in4[4] = NOT(PD[7]);
		TriState res4 = NOR5(in4);

		core->wire.n_TWOCYCLE = AND (NAND(IMPLIED, NOT(res2)), NOR(res3, res4));
		core->wire.n_IMPLIED = NOT(IMPLIED);
	}
}
