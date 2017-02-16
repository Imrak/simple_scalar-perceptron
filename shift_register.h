//General Includes Required:
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

//Project Includes Required:

//Macros / Defines Required:

//Header Guard: START
#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H
//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Structure: Bit
		The Bit structure will be used within the Shift Register. 
		It is the basic structure for our Shift Register.
		
		The Bit Structure contains:
			1) bit_value : The boolean value of the current register (1 = Branch Taken, 0 = Branch Not-Taken)
			2) next_bit : A pointer of type Bit, that points to the next bit in the Shift Register.
			3) prev_bit : A pointer of type Bit, that points to the previous bit in the Shift Register.
*/

typedef struct Bit{
	bool bit_value;
	
	struct Bit *next_bit;
	
	struct Bit *prev_bit;
}Bit;

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Structure: Shift Register
		The Shift Register is one of our primary registers for the Perceptron.
		It contains the most recent information taken from the branch result itself.
		
		The Shift Register contains:
			1) depth : An integer value defined by the Perceptron. Dictates how large the
				Shift Register is allowed to be.
			2) msb : A pointer of type Bit, points to the Most Significant Bit. Represents
				the List's Header Pointer.
			3) lsb : A pointer of type Bit, points to the Least Significant Bit. Represents
				the List's Tail Pointer.
			4) width : The current size of the List. Cannot exceed the depth. Only
				important when want to have running Register size changes during
					code experimentation.
*/

typedef struct Shift_Register{
	int depth;
	
	Bit *msb;
	
	Bit *lsb;
	
	int width;
}Shift_Register;

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Shift Init:
		The Shift Init function takes in a pointer to a Shift Register, initializes all
		the Shift Register's data, and then returns. 
		
		It is a void type function as it doesn't need to generate the memory for
		the Shift Register itself, the Perceptron will take care of that.
		
		Requires:
			1) depth : An integer. Used to define the depth of the Shift Register.
			2) shift_reg : A pointer of type Shift_Register. Will be used
				to generate all the data for the corresponding Shift_Register.
*/
struct Shift_Register *Shift_Init(
	int depth
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Shift Add Bit:
		The Shift Add Bit function takes in a pointer to a new Bit and adds it
		to the list in the msb position.
		
		It needs to:
			1) Check that the Shift Register is not full. If it is it needs
				to Delete a Bit first.
			2) Check that the Shift Register is not brand new. If it is it
				needs to set the msb and lsb pointers.
			3) If the bit is just being added to the list it needs to update
				the msb location, and simply add the bit to the list in the
				msb location.
*/
void Shift_Add_Bit( 
	Shift_Register *shift_reg, 
	Bit *new_bit
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Shift Delete Bit:
		The Shift Delete Bit function takes in a pointer to the Shift Register
			and removes the Bit in the lsb position.
			
		It needs to:
			1) Check that the Shift Register is not empty, if it is it needs
				to warn the user and exit the function.
			2) Check that deleting the Bit will not delete the last remaining
				Bit within the Shift Register. If it would, it needs to warn
				the user and then exit the function.
			3) Delete the Bit in the lsb position, update the entire list, update
				the Shift Register's current size, and then return.
*/
void Shift_Delete_Bit(
	Shift_Register *shift_reg
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Delete Shift Register:
		The Delete Shift Register function takes in a pointer to a Shift Register
		and then simply begins freeing all the Bits attached to that Register.
		
		This function is designed to be run before the end of the program itself
		to ensure all allocated memory is returned.
*/
void Delete_Shift_Register(
	Shift_Register *shift_reg
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//

#endif //SHIFT_REGISTER_H
//Header Guard: END
