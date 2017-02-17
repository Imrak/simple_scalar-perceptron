//Includes Required:
#include "shift_register.h"

//-----------------------------------------------------------------------------//
//
//	                        STRUCTURES
//
//----------------------------------------------------------------------------//
/*
	typedef struct Bit{
		char bit_value;
	
		struct Bit *next_bit;
	
		struct Bit *prev_bit;
	}Bit;
	
	typedef struct Shift_Register{
		int depth;
		
		Bit *msb;
		
		Bit *lsb;
		
		int width;
	};
*/

//-----------------------------------------------------------------------------//
//
//	                        
//----------------------------------------------------------------------------//
/*
	void Shift_Init(
		int depth,
		Shift_Register *shift_reg
		);
*/
struct Shift_Register *Shift_Init( int depth ){
	struct Shift_Register *shift_reg = NULL;
	
	shift_reg = (Shift_Register*)malloc(sizeof(Shift_Register));
	
	//If we ran out of memory, we'll alert the user and then exit the program
		//under the exit(EXIT_FAILURE) condition.
	if(shift_reg == NULL){
		printf("ERROR: OUT OF MEMORY WHEN ATTEMPTING TO MAKE SHIFT REGISTER!\n");
		printf("EXITING PROGRAM UNDER : exit(EXIT_FAILURE)\n\n");
		exit(EXIT_FAILURE);
	}
	
	shift_reg->msb = NULL;
	
	shift_reg->lsb = NULL;
	
	shift_reg->depth = depth;
	
	shift_reg->width = 0;
	
	return shift_reg;
}

//-----------------------------------------------------------------------------//
//
//	                        
//----------------------------------------------------------------------------//
/*
	void Shift_Add_Bit( 
		Shift_Register *shift_reg, 
		Bit *new_bit
	);
*/
void Shift_Add_Bit( Shift_Register *shift_reg, Bit *new_bit ){
	//First we check that the Shift Register is not empty:
	if(shift_reg->msb == NULL && shift_reg->lsb == NULL){
		//If it is, add the new bit to the very first location.
		
		shift_reg->msb = new_bit;
		shift_reg->lsb = new_bit;
		
		new_bit->next_bit = NULL;
		new_bit->prev_bit = NULL;
	}
	
	//Second we check that the Shift Register is not full.
		//If it is, then we need to first delete the lsb.
	else if(shift_reg->width == shift_reg->depth){
		Shift_Delete_Bit( shift_reg );
		
		//Once the bit has been deleted, we just add it to the Register.
			//Set the current msb's prev_bit to the new_bit:
		shift_reg->msb->prev_bit = new_bit;
		
			//Set the new_bit's next_bit to the current msb:
		new_bit->next_bit = shift_reg->msb;
		
			//Set the msb to the new_bit:
		shift_reg->msb = new_bit;
	}
	
	//Lastly, the register is not full, so we simply add the new bit
		//to the Shift Register.
	else{
		//Set the current msb's prev_bit to the new_bit:
		shift_reg->msb->prev_bit = new_bit;
		
			//Set the new_bit's next_bit to the current msb:
		new_bit->next_bit = shift_reg->msb;
		
			//Set the msb to the new_bit:
		shift_reg->msb = new_bit;
	}
	
	//Now we'll increase the Shift Register's Width value, and exit the Function.
	shift_reg->width += 1;
	
	//For now, the final thing we will do is ensure that the lsb remains as a
		//weighed bit. This means it will always be 1.
	shift_reg->lsb->bit_value = 1;
	
	return;
}

//-----------------------------------------------------------------------------//
//
//	                        
//----------------------------------------------------------------------------//
/*
	void Shift_Delete_Bit(
		Shift_Register *shift_reg
	);
*/
void Shift_Delete_Bit( Shift_Register *shift_reg ){
	//First we need to check that we didn't pass an empty Shift Register to
		//this function. If we did, we'll display a warning (something may have gone wrong)
		//and then exit the function.
	if(shift_reg->msb == NULL && shift_reg->lsb == NULL){
		printf("ERROR: EMPTY SHIFT REGISTER PASSED TO DELETE BIT FUNCTION.\n");
		printf("\tEXITING FUNCTION, CONTINUING PROGRAM.\n\n");
		return;
	}
	
	//Second we need to check that if we delete this Bit, we won't be deleting the last
		//Bit in the Shift Register. The Shift Register needs to maintail at least 1
		//Bit in it. If this happens we'll display a warning (something may have gone wrong)
		//and then exit the function.
	else if(shift_reg->width == 1){
		printf("ERROR: CANNOT DELETE THE LAST BIT. SHIFT REGISTER REQUIRES MINIMUM OF 1 BIT.\n");
		printf("\tEXITING FUNCTION, CONTINUING PROGRAM.\n\n");
		return;
	}
	
	//Lastly, if neither of those is true. Then we can simply remove the lsb in the Shift Register.
	else{
			//First we need to make a pointer variable of type Bit so we can keep track of
				//The bit to be deleted.
		Bit *bit_pointer = NULL;
			
		bit_pointer = shift_reg->lsb;
			
			//Then we'll set the lsb to the current lsb's prev_bit.
		shift_reg->lsb = shift_reg->lsb->prev_bit;
		
			//Then we'll detach the old lsb from the list.
		bit_pointer->prev_bit = NULL;
		bit_pointer->next_bit = NULL;
		
			//Then we'll free the old lsb, and reduce the shift_reg's width by [ -1 ].
		free(bit_pointer);
		
		shift_reg->width -= 1;	
	}
	
	return;
}

//-----------------------------------------------------------------------------//
//
//	                        
//----------------------------------------------------------------------------//
/*
	void Delete_Shift_Regisiter(
		Shift_Regisiter *shift_reg
	);
*/
void Delete_Shift_Register( Shift_Register *shift_reg ){
	//To delete the entire Shift Register we need a Bit pointer:
	Bit *bit_pointer = NULL;
	
	//We'll set it to the Shift Register's msb, then use a while loop
		//to loop through the register and remove all the bits.
	bit_pointer = shift_reg->msb;
	
	while( bit_pointer != NULL && shift_reg->width != 0){
		//We'll continue to move the msb pointer down the list, allowin the bit_pointer
			//to trail behind and free the bits as we go.
		shift_reg->msb = shift_reg->msb->next_bit;
		
		bit_pointer->next_bit = NULL;
		
		bit_pointer->prev_bit = NULL;
		
		free(bit_pointer);
		
		//We'll decrease the Shift Register's Width value as we delete the bits:
		shift_reg->width -= 1;
		
		bit_pointer = shift_reg->msb;
	}
	
	return;
}
