#pragma once

/// <summary>
/// Basic logic primitives used in N-MOS chips.
/// Combinational primitives are implemented using ordinary methods.
/// Sequential primitives are implemented using classes.
/// </summary>
namespace BaseLogic
{
	enum TriState
	{
		Zero = 0,
		One = 1,
		Z = -1,
		X = -2,
	};

	/// <summary>
	/// The simplest element, implemented with a single N-MOS FET.
	/// </summary>
	/// <param name="a"></param>
	/// <returns></returns>
	TriState NOT(TriState a);

	/// <summary>
	/// 2-nor
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	TriState NOR(TriState a, TriState b);

	/// <summary>
	/// 3-nor
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="c"></param>
	/// <returns></returns>
	TriState NOR3(TriState a, TriState b, TriState c);

	/// <summary>
	/// 4-nor
	/// </summary>
	/// <param name="in"></param>
	/// <returns></returns>
	TriState NOR4(TriState in[4]);

	/// <summary>
	/// 5-nor
	/// </summary>
	/// <param name="in"></param>
	/// <returns></returns>
	TriState NOR5(TriState in[5]);

	/// <summary>
	/// 6-nor
	/// </summary>
	/// <param name="in"></param>
	/// <returns></returns>
	TriState NOR6(TriState in[6]);

	/// <summary>
	/// 7-nor
	/// </summary>
	/// <param name="in"></param>
	/// <returns></returns>
	TriState NOR7(TriState in[7]);

	/// <summary>
	/// 8-nor
	/// </summary>
	/// <param name="in"></param>
	/// <returns></returns>
	TriState NOR8(TriState in[8]);

	/// <summary>
	/// 9-nor
	/// </summary>
	/// <param name="in"></param>
	/// <returns></returns>
	TriState NOR9(TriState in[9]);

	/// <summary>
	/// 2-nand
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	TriState NAND(TriState a, TriState b);

	/// <summary>
	/// 2-and
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	TriState AND(TriState a, TriState b);

	/// <summary>
	/// 3-and
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="c"></param>
	/// <returns></returns>
	TriState AND3(TriState a, TriState b, TriState c);

	/// <summary>
	/// 4-and
	/// </summary>
	/// <param name="in"></param>
	/// <returns></returns>
	TriState AND4(TriState in[4]);

	/// <summary>
	/// 2-or
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	TriState OR(TriState a, TriState b);

	/// <summary>
	/// 3-or
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="c"></param>
	/// <returns></returns>
	TriState OR3(TriState a, TriState b, TriState c);

	/// <summary>
	/// 2-xor
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	TriState XOR(TriState a, TriState b);

	/// <summary>
	/// The real latch works as a pair of N-MOS transistors.
	/// The first transistor is the tri-state (`d`). It opens the input to the gate of the second transistor, where the value is stored.
	/// After closing the tri-state the value is stored as a `floating` value on the gate of the second transistor.
	/// The output from the second transistor is the DLatch output.
	/// Since the second transistor is essentially an inverter, the output will also be in inverse logic (`/out`)
	/// </summary>
	class DLatch
	{
		TriState g = TriState::Zero;

	public:

		void set(TriState val, TriState en);
		TriState get();
		TriState nget();
	};

	/// <summary>
	/// Flip/Flop.
	/// Instead of using ordinary variables, a class is used to emphasize the circuitry nature.
	/// </summary>
	class FF
	{
		TriState g = TriState::Zero;

	public:

		void set(TriState val);
		TriState get();
	};

	/// <summary>
	/// 2-mux
	/// </summary>
	/// <param name="sel"></param>
	/// <param name="in0"></param>
	/// <param name="in1"></param>
	/// <returns></returns>
	TriState MUX(TriState sel, TriState in0, TriState in1);

	/// <summary>
	/// Generalized PLA matrix emulator.
	/// Although PLA is a combinatorial element, it is made as a class because of its complexity.
	/// </summary>
	class PLA
	{
		uint8_t* rom = nullptr;			// ROM Matrix. We do not use `TriState` to define transistors in the `1` places.
		size_t romSize = 0;				// ROM matrix size in bytes
		size_t romInputs = 0;			// Saved number of decoder inputs (set in the constructor)
		size_t romOutputs = 0;			// Saved number of decoder outputs (set in the constructor)

		std::unordered_map<size_t, TriState*> cache;

		bool Exists(TriState inputs[], TriState outputs[]);
		void Map(TriState inputs[], TriState outputs[]);

	public:
		PLA(size_t inputs, size_t outputs);
		~PLA();

		/// <summary>
		/// Set the decoder matrix.
		/// </summary>
		/// <param name="bitmask">An array of bit values. msb corresponds to input `0`. lsb corresponds to input `romInputs-1`.</param>
		void SetMatrix(size_t bitmask[]);

		/// <summary>
		/// Simulate decoder.
		/// </summary>
		/// <param name="inputs">Input values. The index 0 defines the input `0`. The last index defines the input `romInputs-1`.</param>
		/// <param name="outputs">Output values. The number of outputs must correspond to the value defined in the constructor.</param>
		void sim(TriState inputs[], TriState outputs[]);
	};

	/// <summary>
	/// Pack a bit vector into a byte.
	/// </summary>
	/// <param name="in"></param>
	/// <returns></returns>
	uint8_t Pack(TriState in[8]);

	/// <summary>
	/// Unpack a byte into a bit vector.
	/// </summary>
	/// <param name="val"></param>
	/// <param name="out"></param>
	void Unpack(uint8_t val, TriState out[8]);

	/// <summary>
	/// Dump vector.
	/// </summary>
	/// <param name="in"></param>
	void Dump(TriState in[8], const char* name);

}
