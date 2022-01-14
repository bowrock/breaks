#include "pch.h"

using namespace BaseLogic;

namespace M6502Core
{
	void Flags::sim_Load(TriState inputs[], TriState DB[])
	{
		TriState PHI1 = inputs[(size_t)Flags_Input::PHI1];
		TriState PHI2 = inputs[(size_t)Flags_Input::PHI2];
		TriState SO = inputs[(size_t)Flags_Input::SO];
		TriState B_OUT = inputs[(size_t)Flags_Input::B_OUT];
		TriState ACR = inputs[(size_t)Flags_Input::ACR];
		TriState AVR = inputs[(size_t)Flags_Input::AVR];
		TriState n_IR5 = inputs[(size_t)Flags_Input::n_IR5];
		TriState BRK6E = inputs[(size_t)Flags_Input::BRK6E];

		TriState P_DB = inputs[(size_t)Flags_Input::P_DB];
		TriState DB_P = inputs[(size_t)Flags_Input::DB_P];
		TriState DBZ_Z = inputs[(size_t)Flags_Input::DBZ_Z];
		TriState DB_N = inputs[(size_t)Flags_Input::DB_N];
		TriState IR5_C = inputs[(size_t)Flags_Input::IR5_C];
		TriState DB_C = inputs[(size_t)Flags_Input::DB_C];
		TriState ACR_C = inputs[(size_t)Flags_Input::ACR_C];
		TriState IR5_D = inputs[(size_t)Flags_Input::IR5_D];
		TriState IR5_I = inputs[(size_t)Flags_Input::IR5_I];
		TriState DB_V = inputs[(size_t)Flags_Input::DB_V];
		TriState AVR_V = inputs[(size_t)Flags_Input::AVR_V];
		TriState Z_V = inputs[(size_t)Flags_Input::Z_V];

		// Z

		TriState n_DBZ = NOT(NOR8(DB));
		TriState zin = AND(NAND(NOT(DB[1]), DB_P), NAND(n_DBZ, DBZ_Z));

		z_latch1.set(MUX(OR(DB_P, DBZ_Z), z_latch2.nget(), zin), PHI1);
		z_latch2.set(z_latch1.nget(), PHI2);

		// N

		n_latch1.set(MUX(DB_N, n_latch2.nget(), NAND(NOT(DB[7]), DB_N)), PHI1);
		n_latch2.set(n_latch1.nget(), PHI2);

		// C

		TriState c[4];
		c[0] = NAND(n_IR5, IR5_C);
		c[1] = NAND(NOT(ACR), ACR_C);
		c[2] = NAND(NOT(DB[0]), DB_C);
		c[3] = NAND(NOR3(DB_C, IR5_C, ACR_C), c_latch2.get());

		c_latch1.set(AND4(c), PHI1);
		c_latch2.set(c_latch1.nget(), PHI2);

		// D

		TriState d[3];
		d[0] = NAND(IR5_D, n_IR5);
		d[1] = NAND(NOT(DB[3]), DB_P);
		d[2] = NAND(NOR(IR5_D, DB_P), d_latch2.get());

		d_latch1.set(AND3(d[0], d[1], d[2]), PHI1);
		d_latch2.set(d_latch1.nget(), PHI2);

		// I

		TriState i[3];
		i[0] = NAND(n_IR5, IR5_I);
		i[1] = NAND(NOT(DB[2]), DB_P);
		i[2] = NAND(NOR(DB_P, IR5_I), i_latch2.get());

		i_latch1.set(AND3(i[0], i[1], i[2]), PHI1);
		i_latch2.set(AND(i_latch1.nget(), NOT(BRK6E)), PHI2);

		// V

		avr_latch.set(AVR_V, PHI2);
		so_latch1.set(NOT(SO), PHI1);
		so_latch2.set(so_latch1.nget(), PHI2);
		so_latch3.set(so_latch2.nget(), PHI1);
		vset_latch.set(NOR(so_latch1.nget(), so_latch3.get()), PHI2);

		TriState v[4];
		v[0] = NAND(NOT(AVR), avr_latch.nget());
		v[1] = NAND(NOT(DB[6]), DB_V);
		v[2] = NAND(NOR3(DB_V, avr_latch.nget(), vset_latch.get()), v_latch2.get());
		v[3] = NOT(Z_V);

		v_latch1.set(AND4(v), PHI1);
		v_latch2.set(v_latch1.nget(), PHI2);
	}

	void Flags::sim_Store(TriState P_DB, TriState BRK6E, TriState B_OUT, TriState DB[], bool DB_Dirty[8])
	{
		if (P_DB == TriState::One)
		{
			if (DB_Dirty[0])
			{
				DB[0] = AND(DB[0], NOT(getn_C_OUT()));
			}
			else
			{
				DB[0] = NOT(getn_C_OUT());
				DB_Dirty[0] = true;
			}

			if (DB_Dirty[1])
			{
				DB[1] = AND(DB[1], NOT(getn_Z_OUT()));
			}
			else
			{
				DB[1] = NOT(getn_Z_OUT());
				DB_Dirty[1] = true;
			}

			if (DB_Dirty[2])
			{
				DB[2] = AND(DB[2], NOT(getn_I_OUT(BRK6E)));
			}
			else
			{
				DB[2] = NOT(getn_I_OUT(BRK6E));
				DB_Dirty[2] = true;
			}

			if (DB_Dirty[3])
			{
				DB[3] = AND(DB[3], NOT(getn_D_OUT()));
			}
			else
			{
				DB[3] = NOT(getn_D_OUT());
				DB_Dirty[3] = true;
			}

			if (DB_Dirty[4])
			{
				DB[4] = AND(DB[4], B_OUT);
			}
			else
			{
				DB[4] = B_OUT;
				DB_Dirty[4] = true;
			}

			DB[5];

			if (DB_Dirty[6])
			{
				DB[6] = AND(DB[6], NOT(getn_V_OUT()));
			}
			else
			{
				DB[6] = NOT(getn_V_OUT());
				DB_Dirty[6] = true;
			}

			if (DB_Dirty[7])
			{
				DB[7] = AND(DB[7], NOT(getn_N_OUT()));
			}
			else
			{
				DB[7] = NOT(getn_N_OUT());
				DB_Dirty[7] = true;
			}
		}
	}

	BaseLogic::TriState Flags::getn_Z_OUT()
	{
		return z_latch1.nget();
	}

	BaseLogic::TriState Flags::getn_N_OUT()
	{
		return n_latch1.nget();
	}

	BaseLogic::TriState Flags::getn_C_OUT()
	{
		return c_latch1.nget();
	}

	BaseLogic::TriState Flags::getn_D_OUT()
	{
		return d_latch1.nget();
	}

	BaseLogic::TriState Flags::getn_I_OUT(TriState BRK6E)
	{
		return AND(i_latch1.nget(), NOT(BRK6E));
	}

	BaseLogic::TriState Flags::getn_V_OUT()
	{
		return v_latch1.nget();
	}
}
