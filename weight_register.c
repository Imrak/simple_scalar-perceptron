//Includes Required:
#include "weight_register.h"
#include "shift_register.h"

//-----------------------------------------------------------------------------//
//
//	                        STRUCTURES
//
//----------------------------------------------------------------------------//
/*
	typedef struct Weight{
		signed char weight_value;
		
		struct Weight *next_weight;
		
		struct Weight *prev_weight;
	}Weight;
	
	typedef struct Weight_Register{
		int depth;
		
		Weight *msw;
		
		Weight *lsw;
		
		int width;
	};
*/

//-----------------------------------------------------------------------------//
//
//	                        
//----------------------------------------------------------------------------//
/*
	void Weight_Init(
		int depth,
		Weight_Register *weight_reg
	);
*/
struct Weight_Register *Weight_Init( int depth ){
	struct Weight_Register *weight_reg = NULL;
	
	weight_reg = (Weight_Register*)malloc(sizeof(Weight_Register));
	
	//If we ran out of memory when attempting to make a Weight Register, we'll
		//alert the user and then exit the program under exit(EXIT_FAILURE)
		//conditions.
	if(weight_reg == NULL){
		printf("ERROR: OUT OF MEMORY WHEN ATTEMPTING TO MAKE WEIGHT REGISTER!\n");
		printf("\tEXITING PROGRAM UNDER : exit(EXIT_FAILURE)\n\n");
		exit(EXIT_FAILURE);
	}
	
	weight_reg->depth = depth;
	
	weight_reg->msw = NULL;
	
	weight_reg->lsw = NULL;
	
	weight_reg->width = 0;
	
	return weight_reg;
}

//-----------------------------------------------------------------------------//
//
//	                        
//----------------------------------------------------------------------------//
/*
	void Weight_Add_Weight( 
		Weight_Register *weight_reg, 
		Weight *new_weight
	);
*/
void Weight_Add_Weight( Weight_Register *weight_reg, Weight *new_weight ){
	//First we check that the Weight Register is not empty:
	if(weight_reg->msw == NULL && weight_reg->lsw == NULL){
		//If it is, add the new weight to the very first location.
		
		weight_reg->msw = new_weight;
		weight_reg->lsw = new_weight;
		
		new_weight->next_weight = NULL;
		new_weight->prev_weight = NULL;
	}
	
	//Second we check that the Weight Register is not full.
		//If it is, then we need to not add the weight.
		//Unlike the weight Register, the Weight Register, once full,
		//will no longer add to the register.
	else if(weight_reg->width == weight_reg->depth){
		return;
	}
	
	//Lastly, the register is not full, so we simply add the new weight
		//to the Weight Register.
	else{
		//Set the current msw's prev_weight to the new_weight:
		weight_reg->msw->prev_weight = new_weight;
		
		//Set the new_weight's next_weight to the current msw:
		new_weight->next_weight = weight_reg->msw;
		//Set the new_weight's previous weight to NULL, new head 
		new_weight->prev_weight = NULL;	
		//Set the msw to the new_weight:
		weight_reg->msw = new_weight;
	}
	
	//Now we'll increase the Weight Register's Width value, and exit the Function.
	weight_reg->width += 1;
	
	return;
}

//-----------------------------------------------------------------------------//
//
//	                        
//----------------------------------------------------------------------------//
/*
	void Weight_Delete_Weight(
		Shift_Register *shift_reg,
		Weight_Register *weight_reg
	);
*/
void Weight_Delete_Weight( Shift_Register *shift_reg, Weight_Register *weight_reg ){
	//First we need to check that we didn't pass an empty Weight Register to
		//this function. If we did, we'll display a warning (something may have gone wrong)
		//and then exit the function.
	if(weight_reg->msw == NULL && weight_reg->lsw == NULL){
		printf("ERROR: EMPTY WEIGHT REGISTER PASSED TO DELETE WEIGHT FUNCTION.\n");
		printf("\tEXITING FUNCTION, CONTINUING PROGRAM.\n\n");
		return;
	}
	
	//Second we need to check that if we delete this Weight, we won't be deleting the last
		//Weight in the Weight Register. The Weight Register needs to maintail at least 1
		//Weight in it. If this happens we'll display a warning (something may have gone wrong)
		//and then exit the function.
	else if(weight_reg->width == 1){
		printf("ERROR: CANNOT DELETE THE LAST WEIGHT. WEIGHT REGISTER REQUIRES MINIMUM OF 1 WEIGHT.\n");
		printf("\tEXITING FUNCTION, CONTINUING PROGRAM.\n\n");
		return;
	}
	
	//Third we need to check if the width of the Weight Register is less than, or will be less than, the Shift Register.
		//The Weight Register must maintain the same size and depth as the Shift Register, otherwise the summation
		//protocol will not function correctly. 
		//If this is true, we don't want to delete anything. We'll warn the user and the continue the program.
	else if(weight_reg->width < shift_reg->width || ((weight_reg->width) -1) < shift_reg->width ){
		printf("ERROR: CANNOT DELETE WEIGHTS TO THE POINT WEIGHT REGISTER IS LESS THAN THE SHIFT REGISTER\n");
		printf("\tEXITING FUNCTION, CONTINUING PROGRAM.\n");
		return;
	}
	
	//Lastly, if neither of those is true. Then we can simply remove the lsw in the Weight Register.
	else{
			//First we need to make a pointer variable of type Weight so we can keep track of
				//The Weight to be deleted.
		Weight *weight_pointer = NULL;
			
		weight_pointer = weight_reg->lsw;
			
			//Then we'll set the lsw to the current lsw's prev_weight.
		weight_reg->lsw = weight_reg->lsw->prev_weight;
		
			//Then we'll detach the old lsw from the list.
		weight_pointer->prev_weight = NULL;
		weight_pointer->next_weight = NULL;
		
			//Then we'll free the old lsw, and reduce the weight_reg's width by [ -1 ].
		free(weight_pointer);
		
		weight_reg->width -= 1;	
	}
	
	return;
}

//-----------------------------------------------------------------------------//
//
//	                        
//----------------------------------------------------------------------------//
/*
	void Delete_Weight_Regisiter(
		Weight_Regisiter *weight_reg
	);
*/
void Delete_Weight_Register( Weight_Register *weight_reg ){
	//To delete the entire Weight Register we need a Weight pointer:
	Weight *weight_pointer = NULL;
	
	//We'll set it to the Weight Register's msw, then use a while loop
		//to loop through the register and remove all the weights.
	weight_pointer = weight_reg->msw;
	
	while( weight_pointer != NULL ){
		//We'll continue to move the msw pointer down the list, allowin the weight_pointer
			//to trail behind and free the weights as we go.
		weight_reg->msw = weight_reg->msw->next_weight;
		
		weight_pointer->next_weight = NULL;
		
		weight_pointer->prev_weight = NULL;
		
		free(weight_pointer);
		
		//We'll decrease the Weight Register's Width value as we delete the weights:
		weight_reg->width -= 1;
		
		weight_pointer = weight_reg->msw;
	}
	
	return;
}
