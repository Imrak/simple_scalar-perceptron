//General Includes Required:
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Project Includes Required:
#include "shift_register.h"
#include "weight_register.h"

//Macros / Defines Required:
#define DEPTH 8				//Use this to define the depth of the Shift and Weight registers. Use steps of 2: 2, 4, 8, 16, 32, etc.
#define MAX_WEIGHT 127		//Use this to define the maximum weight value a Weight can contain
#define MIN_WEIGHT -127		//Use this to define the minimum weight value a Weight can contain
#define THRESHHOLD 0		//Use this to define the threshold required for the Perceptron to guess
								//either Taken or Not Taken.
								//Rule:
								//If the Summation of the Weights is [ less than ] the Threshold the
								// Perceptron will guess [ Not Taken := 0 ]
								//If the Summation of the Weights is [ greater than or equal to ] the Threshold
								// the Perceptron will guess [ Taken := 1 ]
#define LINKS 1				//Use this to define the depth of the Perceptron List, if applicable
#define DEBUG 1				// Will print out debug information if one
//Header Guard: START
#ifndef PERCEPTRON_H
#define PERCEPTRON_H

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Structure: Perceptron Data (Percep_Data)
		The Perceptron Data structure is used to contain all the required information
		a perceptron requires. 
		
		This includes:
			1) hit_count : A float counter that counts the number of times the perceptron
				was correct.
			2) miss_count : A float counter that counts the number of times the perceptron
				was incorrect.
			3) in_file : A character pointer that points to a string that is used for the input file for the Test Data [ Not Required ].
			4) out_file : A character pointer that points to a string that is used for the output file for the Result Data [ Not Required ].
			5) train_count : An integer counter that counts the number of times the perceptron
				went into training.
			6) depth : An unsigned char that holds the maximum depth the Shift and Weight registers are allowed to become.
			7) max & min_weight : signed chars that hold the maximum & minimum size the weight_values are allowed to become.
				(They should be negative opposites of each other. I.E -127 and 127 or -5 and 5, etc.
			8) threshold : A signed char value that holds the threshold value the perceptron will use to determine a prediction.
				The summation of the weights is checked against this number. If the summation is greater than or equal to the threshold
				the Perceptron will predict [ Taken ] otherwise it will predice [ Not Taken ].
			9) links : An unsigned short integer variable that holds just how large any single Perceptron List is allowed to become.
			10) train_value : A signed short integer that holds how much the weight_values will be adjusted during a Training Period.
				This will be defaulted to 1. There will be a program to specifically change this value. When the Perceptron's Data is
				initialized however, it will be hard-coded to a value of 1.
*/
typedef struct Percep_Data{
	float hit_count;
	
	float miss_count;
	
	char *in_file;
	
	char *out_file;
	
	int train_count;
	
	unsigned char depth;
	
	signed char max_weight;
	
	signed char min_weight;
	
	signed char threshold;
	
	unsigned short int links;
	
	signed short int train_value;
	
}Percep_Data;

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Structure: Perceptron
		The Perceptron structure is the housing for the entire Perceptron.
		
		It contains:
			1) shift_reg : An variable of type Shift Register. Not a pointer type.
			2) weight_reg : A variable of type Weight Register. Not a pointer type.
			3) percep_data : A variable of type Percep Data. Not a pointer type.
			4) next_percep : A pointer of type Perceptron that points to the next Perceptron
				in a Perceptron List. [ Not Required when only using 1 ].
			5) prev_percep : A pointer of type Perceptron that points to the previous Perceptron
				in a Perceptron List. [ Not required when only using 1 ].
*/
typedef struct Perceptron{
	//Shift_Register *shift_reg;
	
	Weight_Register *weight_reg;
	
	Percep_Data *percep_data;
	
	struct Perceptron *next_percep;
	
	struct Perceptron *prev_percep;
	
}Perceptron;

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Structure: Perceptron List (Percep_List)
		The Perceptron List is a list that contains multiple perceptrons.
		
		It is for when multiple perceptrons are required.
		
		It contains:
			1) msp : A pointer of type Perceptron. Points to the Most Significant Perceptron,
				which is also the Head of the Perceptron List.
			2) lsp : A pointer of type Perceptron. Points to the Least Significant Perceptron,
				which is also the Tail of the Perceptron List.
			3) size : An integer that counts the size of the Perceptron List.
			4) depth : An integer that holds the depth the Perceptron List is allowed to obtain.
*/
typedef struct Percep_List{
	Perceptron *msp;
	
	Perceptron *lsp;
	
	Shift_Register *shift_reg;
	
	int size;
	
	int depth;
	
	int reg_size;
	
}Percep_List;

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Data Init:
		Data Init takes in a pointer to a Perceptron Data variable and 
		initializes all of its data. 
		
		This function is of type void. It does not need to allocate
		memory as the Perceptron itself will do that.
		
		The depth, max_weight, min_weight, threshold, and links will be passed
		to this function when the Perceptron is initialized. 
		
		These values represent the 'command line arguments' that must be passed
		 to the Perceptron.
		 
		A different function will be used to grab the in_file and out_file data as
		 it is not strictly necessary for the Perceptron itself.
*/
struct Percep_Data *Data_Init(
	unsigned char depth,
	signed char max_weight,
	signed char min_weight,
	signed char threshold,
	unsigned short int links
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Perceptron Init:
		Perceptron init returns a pointer to a fully initialized Perceptron.
		
		Requires:
			1) depth : the size the Shift and Weight registers are allowed to take on.
			2) max_weight : the maximum weight any weight_value can obtain.
			3) min_weight : the minimum weight any weight_value can obtain.
			4) threshold : the value the Perceptron will use to determine if it will predict
				[ Taken ] or [ Not-Taken ].
			5) links : the size of the perceptron list. It is recommended that you pass 1 to this
				value, even if you are not making a Perceptron List.
*/
struct Perceptron *Perceptron_Init(
	unsigned char depth,
	signed char max_weight,
	signed char min_weight,
	signed char threshold,
	unsigned short int links
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Perceptron List Init:
		Perceptron List Init returns a pointer to a fully initialized Perceptron
		List. 
*/
struct Percep_List *Per_List_init( 
	int depth,
	int size
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Percep Add Percep:
		The Percep Add Percep function adds a fully initialized perceptron to
		the Perceptron List.
		
		It requires:
			1) percep_list : A pointer to the primary perceptron list.
			2) new_percep : A pointer to a Perceptron that has been initialized completely
				and is ready to be added to the Perceptron List.
		
		To completely add the Perceptron to the List requires:
			1) A check that the Perceptron List isn't empty. If it is then this will be
				the first perceptron to be added to the list.
			2) A check that the Perceptron List is not a maximum depth. If it is then
				it will be assumed that no new Perceptrons are to be added. 
				Display a warning and exit the function.
			3) Move the old Most Significant Perceptron down the list and iterate the
				new Perceptron into the msp location.
*/
void Percep_Add_Percep(
	Percep_List *percep_list,
	Perceptron *new_percep
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Remove Perceptron:
		The Remove Perceptron function deletes a specific perceptron from the
			Perceptron List.
			
		To do this requires:
			1) percep_list : A pointer to the primary perceptron list.
			2) target_percep : A pointer to the target perceptron.
			
		To delete the perceptron requires:
			1) Check that the target perceptron is actually in the Perceptron List.
				If it isn't display an error to the user and exit the function.
			2) Check that the target perceptron is not the last Perceptron in the List.
				If it is, then we cannot delete it. The Perceptron List must maintain
				at least 1 Perceptron while the program is running.
			3) If the target perceptron is apart of the Perceptron List, then adjust
				all corresponding next / prev pointers, remove the target perceptron
				from the Perceptron List, and then free it. To fully complete this
				requires attention to:
					A) Target Perceptron is the msp
					B) Target Perceptron is the lsp
					C) Target Perceptron is neither the msp or the lsp
*/
void Remove_Perceptron(
	Percep_List *percep_list,
	Perceptron *target_percep
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Delete Perceptron:
		The Delete Perceptron function deletes the entire Perceptron List.
		
		This function is designed specifically for at the end of the program,
		to free all data.
		
		To do so require:
			1) percep_list : A pointer to the perceptron list.
			
		In using this function we must also:
			1) Delete the Shift Registers within each Perceptron.
			2) Delete the Weight Registers within each Perceptron.
*/
void Delete_Perceptron(
	Percep_List *percep_list
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Sum Weights:
		The Sum Weights function:
			Looks at the Shift Register's Bit Values. 
			If the Bit Value is a 1, the corresponding Weights within the
				Weight Register are [ ADDED ] to the total Sum.
			If the Bit Value is a 0, the corresponding Weights within the
				Weight Register are [ SUBTRACTED ] from the total Sum.
			
			This Sum Value is then returned from this function.
			
			It will be used to determine if the Perceptron will guess Taken or 
				Not-Taken.
*/
int Sum_Weight(
	Perceptron *percep,
	Percep_List *pList
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Decision:
		The Decision function takes in the summation's result and compares it
		against the Perceptron's Threshold (One of the Defined Macro Values). 
		
		That decision is then compared against the test's actual result to see
		if the Perceptron guessed the direction of the branch correctly or not.
		
		If the Perceptron guessed correctly, the Decision will output a 1.
		If the Perceptron guessed incorrectly, the Decision will output a 0.
		
		This function requires:
			1) threshold : The threshold of the Perceptron (Macro'd value)
			2) sum : the result of the Sum Weights function
			3) actual : boolean value that represents the actual direction
				the branch took. 1 := Taken, 0 := Not-Taken.
			4) percep : The perceptron that is being tested. If the decision comes
				out to agree with the actual result, then we will increase the
				Perceptron's hit counter.
				
				Conversely, if the perceptron predicts incorrectly, we'll increase
				the Perceptron's miss counter.
*/
char *Decision(
	int threshold,
	int sum,
	Perceptron *percep
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Perceptron Training:
		The Perceptron Training function takes in the result of the Decision
		function.
		
		If the output of the Decision Function is 1 ( Perceptron predicted correctly )
			the Perceptron will not engage in training.
			
		If the output of the Decision Function is 0 ( Perceptron predicted incorrectly )
			the Perceptron will engage in training.
			
		The Perceptron's training data variable will be increased by +1.
		The Perceptron will then use the Actual value the perdiction should have been
		to increase or decrease the weight_values by the train_value within the Perceptron's
		Data.
		
		The Perceptron will need to cycle through both the Shift and Weight Registers.
		If the bit_value within the Shift Register matches that of the Actual value, then that
		corresponding Weight's weight_value will be [ INCREASED ] by the Perceptron's train_value.
		
		If the bit_value within the SHift Register does not match the Actual value, then that
		corresponding Weight's weight_value will be [ DECREASED ] by the Perceptron's train_value.
*/
void Perceptron_Training(
	char decision,
	char actual,
	Perceptron *percep,
	Percep_List *pList
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Set Files:
		The Set Files function sets the in_file and out_files for a Perceptron.
		
		Since the in_file and out_file are not reqired for a Perceptron to
		test, they will not be required as part of the Perceptron's Initialization
		parameters.
		
		Because of this, we require this function to set the files if they exist at all.
*/
void Set_Files( 
	char *in_file, 
	char *out_file, 
	Perceptron *percep
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Test From File:
		The Test From File function will test a Perceptron given an input and
		output file parameter. 
		
		This function will require:
			1) Depth the Shift and Weight registers will be allowed to become.
			2) Depth the Perceptron List will be allowed to become.
			3) Maximum Weight the Weight Values are allowed to become.
			4) Minimum Weight the Weight Values are allowed to become.
			5) The required Threshold for the Perceptron to make a prediction.
			6) The input file
			7) The output file
			
		This function will then run an entire test. It will create, read, and test
		the entire Perceptron(s) through the inputfile that was given to it.
		
		It will then display the results at the end of the testing to the output file.
*/
void Test_From_File(
	unsigned char depth,
	unsigned short int list,
	signed char max_weight,
	signed char min_weight,
	signed char threshold,
	char *in_file,
	char *out_file
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Write Output:
		The Write Output file used the out_file within the passed Perceptron
		to write to an output file. The file will display:
		-Hit Count 	|	[ Hit Count Number ]
		-Miss Count	|	[ Miss Count Number ]
		-Trained	|	[ Times Trained ]
		-Accuracy	|	[ Hit Count / (Hit + Miss Count) ]
*/
void Write_Output(
	Perceptron *percep
);


// Method created to print out results to stdout
void Print_Output(
	Percep_List *list
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Test From Array:
		The Test From Array function is similar to the Test From File function.
		The only difference is that instead of looping through a file, it loops
		through an array of type boolean. 
		
		It will still run the entire test from this function, outputting a file
		with the Write Output data within it.
*/

void Test_From_Array(
		int *array_pointer,
		int array_size,
		unsigned char depth,
		unsigned short int list,
		signed char max_weight,
		signed char min_weight,
		signed char threshold,
		char *out_file
	);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Daisy Chain:
		The Daisy Chain function is to test the Perceptrons by having the
		Prediction of one perceptron be the input to the Shift Register for a 
		second. The final say on the Prediction is then based on the last
		Perceptron in the chain.
		
		Based on the results of that Perceptron dictates if any, or all, of
		the Perceptrons are trained.
*/
void Daisy_Chain(
	int *array_pointer,
	int array_size,
	unsigned char depth,
	unsigned short int list,
	signed char max_weight,
	signed char min_weight,
	signed char threshold,
	char *in_file,
	char *out_file
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Hash Perceptron:
		The Hash Perceptron function takes in an interger representation of the
		branching address and return the Perceptron within the Perceptron List
		to that location's perceptron.
*/
struct Perceptron *Hash_Percep( 
int address, 
Percep_List *list
);

double Sum_Train(
	Percep_List *pList	
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Update_Shift_Reg:
		The Update Shift Reg function allows you to modify the Global Shift Register
		without having to simply rerun the entire program.
		
		It requires:
			1) The Perceptron List that has the Global Shift Register.
			2) The new depth for the Shift Register.
				In the event that shift register is [ Increasing ] in size, new bit
					will be added.
				In the event the shift register is [ Decreasing ] in size, bits will
					be removed, starting from the LSB location.
					
		Be warned:
			If you make the Global Shift Register larger than the Weight Registers when
			the Perceptron calls the summing function it, currently, will continue to loop
			through the weight register(s) until the Global Shift Register has reached the end.
			
			If you make the Global Shift Register smaller than the Weight Registers, the 
			Perceptron will continue to sum the weights until the Global Shift Register has
			reached the LSB.
*/
void Update_Shift_Reg(
	Percep_List *pList,
	int new_shift_reg_size
);

#endif //PERCEPTRON_H
//Header Guard: END
