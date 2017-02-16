//General Includes Required:
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

//Project Includes Required:
#include "shift_register.h"
#include "weight_register.h"
#include "perceptron.h"

//Macros / Defines Required:

//Header Guard : START
#ifndef HISTORY_BUFFER_H
#define HISTORY_BUFFER_H
//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Structure: Branch History Location
		The Branch History Location Structure is a double linked list element.
		It contains:
			1) Pointers of type Perceptron to the location of the connected
				Perceptron(s) for this branch location.
			2) next_location : a pointer of type Branch History Location that
				points to the next Branch History Location in the list.
			3) prev_location : a pointer of type Branch History Location that
				points to the previous Branch History Location in the list.
			4) branch_code : an integer designed to hold the machine code location
				of a branch request. This code is important as it will allow us to
				identify the correct branch history that we want.
			5) branch_location : an integer designed to hold the machine code location
				of the target branch location. This is the location the branch is
				supposed to jump to after it has taken the branch.
			6) age : An unsigned character variable. The age represents how 'old' the
				particular location is within the Branch History List or Table.
				This variable is important for replacement policies for the Branch History List
				or Table.
				Any time the Branch History List, or Table, is accessed the age will
				be increased by +1 for all Locations that exist.
			7) use : An unsigned integer, use is another tool for replacement policies.
				Any time a Location is accessed, its use is reset to 0. If a List or Table
				exists with multiple Locations, then anytime a Location is accessed all others
				will be increased by +1.
				
				When it comes time to replace a Location due to maximum capacity of the List or
				Table, a formula of: age - use, can be used to determine which locations haven't
				been used that often and would make a good replacement choice.
*/
typedef struct Branch_History_Location{
	Perceptron *percep;
	
	struct Branch_History_Location *next_location;
	struct Branch_History_Location *prev_location;
	
	char* branch_code;
	char* branch_location;
	
	unsigned int age;
	unsigned int use;
}BHL;

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Structure: Branch History List
		The Branch History List Structure is a double-linked list of 
		Branch History Locations.
		
		It contains:
			1) msh : the Most Signficant History is a pointer of type Branch_
				History_Location that points to the most significant (Head)
				location.
			2) lsh : the Least Significant History is a pointer of type Branch_
				History_Location that points to the least significant (Tail)
				location.
			3) max_size : an integer variable to dictates the maximum size the 
				Branch History List is allowed to grow to.
			4) curr_size : an integer variable that holds the current size of
				the Branch History List
*/
typedef struct Branch_History_List{
	BHL *msh;
	BHL *lsh;
	
	int max_size;
	int curr_size;
}BHList;

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Type: Replacement Type
		Replacement Type is a definition designed for use only within the
		Branch History Table. It is used to determine how to replace / remove
		aspects within the Branch History Table.
		
		The Replacement Types include:
			1) Random
			2) Oldest
			3) Useage
*/
typedef enum Replacement_Type{
	Random, 
	oldest,
	useage
}Replacement_Type;

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Structure Branch History Table
		The Branch History Table Structure holds both a Branch History List
		and a Perceptron List.
		
		It contains:
			1) branch_list : A pointer to a Branch History List.
			2) percep_list : A pointer to a Perceptron List.
			3) depth : An integer that holds maximum depth a Branch or 
				Perceptron List is allowed to obtain.
			4) reg_size : An integer that holds the maximum size the
				Shift and Weight Registers within a Perceptron are allowed
				to obtain.
			5) max_weight : A signed character with the maximum weight a
				Perceptron's weight is allowed to obtain.
			6) min_weight : A signed character with the minimum weight a
				Perceptron's weight is allowed to obtain.
			7) threshold : A signed short integer value to contains the
				required threshold for a Perceptron to predict taken or
				not-taken.
			8) replacement : A Replacement_Type variable (defined above
				that will dictate [ how ] the Branch History Table will
				delete / replace articles within its' table.
*/
typedef struct Branch_History_Table{
	BHList *branch_list;
	
	Percep_List *percep_list;
	
	int depth;
	int reg_size;
	
	signed char max_weight;
	signed char min_weight;
	
	signed short int threshold;
	
	Replacement_Type replacement;
}BHT;

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Branch List Init:
		The Branch List Init function returns a pointer to a fully initialized
		Branch History List.
*/
struct Branch_History_List *List_Init(
	int max_size
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Table Init:
		The Table Init function returns a pointer to a fully initialized Branch
		History Table.
		
		The Branch History Table requires a great deal more information than
		other functions because it will be required to initialize both 
		Branch History Locations as well as Perceptrons as they are required.
*/
struct Branch_History_Table *Table_Init(
	int depth, 
	int reg_size, 
	signed char max_weight, 
	signed char min_weight, 
	signed short int threshold,
	Replacement_Type replacement
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Branch Add Location:
		An important factor to this function is that the Branch History Table
		doesn't keep duplicates of any address. Therefore any time we need to
		add a new Location to the Table, or List, we need to first check that
		the location doesn't exist already.
		
		Additionally we have to be warry for when the History Table has reached
		maximum capacity. Once the Table is full we then will have to replace either
		randomly, or based on uses or age, or some other factor.
*/
void Branch_Add_Location( 
	BHT *branch_table, 
	BHL *location
 );
 
//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Branch Delete Location:
		The Branch Delete Location function takes in a pointer to a Branch History
		List, as well as a Branch History Location. It then searches the List
		for that specific Location. If found, it deletes it from the Branch History
		List.
		
		If not found, it simply returns to the program.
*/
void Branch_Delete_Location(
	BHList *branch_list,
	BHL *location
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Branch Delete List:
		The Branch Delete List removes all Branch History Locations from the
		Branch History List.
*/
void Branch_Delete_List(
	BHList *branch_list
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Replace History:
		The Replace History Function will remove a history location within
		the Branch History List and replace it with a Branch History Location we
		pass to this function.
		
		The method in which a Location is replaced depends entirely upon the
		way the Branch History Table was initiated.
		
		Random Method:
			If random method was designated a random number will be generated.
			Beginning at the top of the list, and working down, once that number
			of Locations was passed, that randomly chosen location will be replaced.
			
		Oldest Method:
			If the oldest method was designated then the Branch History Location with
			the greatest age variable will be replaced.
			
			In the event of a tie, the first instance will be replaced.
			
		Useage Method:
			If the use method was designated then the Branch History Location that
			is used the fewest times will be replaced.
			
			The calculation for that is the Branch History Location whose:
				age - use
			is closest to 0 will be replaced. In the event of a tie, the first instance 
			will be replaced.
*/
void Replace_History(
	BHList *list,
	BHL *new_location,
	enum Replacement_Type replace
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Delete History:
		The Delete History function deletes an entire Branch History Table.
*/
void Delete_History(
	BHT *table
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Search Table:
		The Search Table function searches the table for an inputted
		Branch Code. If found, it returns a pointer to that Location so that
		it can be used to run its prediction tests.
*/
struct Branch_History_Location *Search_Table(
	BHT *table, 
	char* branch_code
);

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Test_File:
		The Test File function will generate a Branch History Table,
		and the perceptrons.
		
		It will then read from a file, line by line, and run tests on the
		Perceptrons based on what branch codes come from the test file.
		
		The test file needs to be in the format of:
		branch_code\tbranch_target\tbranch_reslt\n
		
		The function will loop through the file using fgets to grab each
		segement.
		
		After grabbing a segment, specifically the branch_code it will then
		Seaarch the Branch History Table for that code.
		
		If it find it, then it will run a test on the Perceptron with the
		actual result being dictated upon by the branch result from the file.
		
		If it does not find it then it will attempt to make a Brnach Location for
		the new branch code and attach it to the Branch History List. If the List is
		full then it will run through its normal replacement policy.
		
		This will continue until the file is empty and complete.
		
	This Function Requires:
		1) in_file : pointer to the input file
		2) out_file : pointer to the output file
		3) depth : an integer value of how large the History and Perceptron Lists can become
		4) reg_size : an integer value for how large the Perceptron's Shift and Weight registers
			may grow to.
		5) max_weight : a signed char variable that holds the maximum weight a Perceptron weight can obtain.
		6) min_weight : a signed char variable that holds the minimum weight a Perceptron weight can obtain.
		7) threshold : a signed short integer variable that holds the required summation value for a Perceptron
			to predict either Taken or Not-Taken.
		8) replacement : a Replacement Type variable that will determine how the Table will replace location when
			it has reached maximum capacity.
*/
void Test_File(
	char* in_file,
	char* out_file,
	int depth,
	int reg_size,
	signed char max_weight,
	signed char min_weight,
	signed short int threshold,
	Replacement_Type replacement
);

#endif // HISTORY_BUFFER_H
