//General Includes Required:
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Project Includes Required:
#include "shift_register.h"

//Macros / Defines Required:

//Header Guard: START
#ifndef WEIGHT_REGISTER_H
#define WEIGHT_REGISTER_H

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Structure: Weight
		The Weight structure will be used within the Weight Register. 
		It is the basic structure for our Weight Register.
		
		The Bit Structure contains:
			1) weight_value : A signed char value that will hold the weight of the Weight. 
				At the creation of the Weight this value will be randomly generated.
			2) next_weight : A pointer of type Weight, that points to the next Weight in the Weight Register.
			3) prev_weight : A pointer of type Weight, that points to the previous Weight in the Weight Register.
*/

typedef struct Weight{
	signed char weight_value;
	
	struct Weight *next_weight;
	
	struct Weight *prev_weight;
}Weight;

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Structure: Weight Register
		The Weight Register is one of our primary registers for the Perceptron.
		It contains the weight information that will be summed, and then compared
		against the Perceptron's threshold value. 
		
		This will then be used to determine if the Perceptron guessed the direction
		of the branch correctly or not.
		
		The Weight Register contains:
			1) depth : An integer value defined by the Perceptron. Dictates how large the
				Weight Register is allowed to be.
			2) msw : A pointer of type Weight, points to the Most Significant Weight. Represents
				the List's Header Pointer.
			3) lsw : A pointer of type Weight, points to the Least Significant Weight. Represents
				the List's Tail Pointer.
			4) width : The current size of the List. Cannot exceed the depth. Only
				important when want to have running Register size changes during
					code experimentation.
*/

typedef struct Weight_Register{
	int depth;
	
	Weight *msw;
	
	Weight *lsw;
	
	int width;
}Weight_Register;

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Weight Init:
		The Weight Init function takes in a pointer to a Weight Register, initializes all
		the Weight Register's data, and then returns. 
		
		It is a void type function as it doesn't need to generate the memory for
		the Weight Register itself, the Perceptron will take care of that.
		
		Requires:
			1) depth : An integer. Used to define the depth of the Weight Register.
			2) weight_reg : A pointer of type Weight_Register. Will be used
				to generate all the data for the corresponding Weight_Register.
*/
struct Weight_Register *Weight_Init(
	int depth
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Weight Add Weight:
		The Weight Add Weight function takes in a pointer to a new Weight and adds it
		to the list in the msw position.
		
		It needs to:
			1) Check that the Weight Register is not full. If it is it needs
				to simply exit the function.
			2) Check that the Weight Register is not brand new. If it is it
				needs to set the msw and lsw pointers.
			3) If the weight is just being added to the list it needs to update
				the msw location, and simply add the weight to the list in the
				msw location.
				
		IMPORTANT: This function should only be called at the very beginning
			creation of a Perceptron. Once a Perceptron is called this function
			should not be called anymore.
*/
void Weight_Add_Weight( 
	Weight_Register *weight_reg, 
	Weight *new_weight
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Weight Delete Weight:
		The Weight Delete Weight function takes in a pointer to the Shift Register
			and a pointer to the Weight Register.
			
		It needs to:
			1) Check that the Weight Register is not empty, if it is it needs
				to warn the user and exit the function.
			2) Check that deleting the Weight will not delete the last remaining
				Weight within the Weight Register. If it would, it needs to warn
				the user and then exit the function.
			3) Check that deleting the Weight will not delete the Weight Register to
				a position where it has fewer items in its' list than the Shift Register.
				The Shift Register and the Weight Register must contain the same number
				of list items, otherwise the Perceptron will not function correctly.
				
				If this is true, it needs to warn the user and then exit the function.
				
			4) If none of 1 -> 3 are true, then this function will simply delete the
				Weight Register in the lsw position, readjust the lsw pointer, and
				decrement the Weight Register's width value.
				
			IMPORTANT: This function should not actually be getting called. 
*/
void Weight_Delete_Weight(
	Shift_Register *shift_reg,
	Weight_Register *weight_reg
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Delete Weight Register:
		The Delete Weight Register function takes in a pointer to a Weight Register
		and then simply begins freeing all the Weighs attached to that Register.
		
		This function is designed to be run before the end of the program itself
		to ensure all allocated memory is returned.
*/
void Delete_Weight_Regisiter(
	Weight_Register *weight_reg
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//

#endif //WEIGHT_REGISTER_H
//Header Guard: END
