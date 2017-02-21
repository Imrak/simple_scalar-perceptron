//Includes Required:
#include "history_buffer.h"
#include "perceptron.h"
#include "shift_register.h"
#include "weight_register.h"

#if 0
//-----------------------------------------------------------------------------//
//
//	                           STRUCTURES
//
//----------------------------------------------------------------------------//
/*
	typedef struct Branch_History_Location{
		Perceptron *percep;
		
		struct Branch_History_Location *next_location;
		struct Branch_History_Location *prev_location;
		
		char* branch_code;
		char* branch_location;
		
		unsigned int age;
		unsigned int use;
	}BHL;
	
	typedef struct Branch_History_List{
		BHL *msh;
		BHL *lsh;
		
		int max_size;
		int curr_size;
	}BHList;
	
	typedef enum Replacement_Type{
		random, 
		oldest,
		useage
	}ReType;
	
	typedef struct Branch_History_Table{
		BHList *branch_list;
		
		Percep_List *percep_list;
		
		int depth;
		int reg_size;
		
		signed char max_weight;
		signed char min_weight;
		
		signed short int threshold;
		
		ReType replacement;
	}BHT;

//-----------------------------------------------------------------------------//
//
//	                           FUNCTIONS
//
//----------------------------------------------------------------------------//
/*
	struct BHList *List(
		int max_size
	);
*/
struct Branch_History_List *List_Init( int max_size ){
	struct Branch_History_List *list = NULL;
	
	list = (BHList*)malloc(sizeof(BHList));
	
	list->msh = NULL;
	list->lsh = NULL;
	list->max_size = max_size;
	list->curr_size = 0;
	
	return list;
}

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	struct BHT *Table(
		int depth, 
		int reg_size, 
		signed char max_weight, 
		signed char min_weight, 
		signed short int threshold
	);
*/
struct Branch_History_Table *Table( int depth, int reg_size, signed char max_weight, signed char min_weight, signed short int threshold, Replacement_Type replacement ){
	struct Branch_History_Table *table = NULL;
	
	table = (BHT*)malloc(sizeof(BHT));
	
	table->branch_list = List_Init( depth );
	
	table->percep_list = Per_List_init( depth );
	
	table->max_weight = max_weight;
	
	table->min_weight = min_weight;
	
	table->threshold = threshold;
	
	table->reg_size = reg_size;
	
	table->replacement = replacement;
	
	return table;
}

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	void Branch_Add_Location( 
		BHList *branch_list, 
		BHL *location
	 );
*/
void Branch_Add_Location( BHT *branch_table, BHL *location ){
	//To add a location to our Branch History List we'll need
		//a location pointer.
	BHL *loc_pointer = NULL;
	
	//First we must check that the Branch List isn't empty.
		//If it is, we'll simply add the location to the head of
		//the list.
	if(branch_table->branch_list->msh == NULL && branch_table->branch_list->lsh == NULL){
		branch_table->branch_list->msh = location;
		branch_table->branch_list->lsh = location;
		
		branch_table->branch_list->curr_size += 1;
		
		//We'll actually return from this statement because there are
			//other aspect this program must run if this case isn't true.
			//To avoid running them when we don't want to we'll end this
			//function early.
		return;
	}
	
	//Next we must check that the Location doesn't already exists within
		//the Branch History List. If it doesn, then we won't add it to
		//the list. In fact, if this instance occures we'll actually just
		//exit this function and continue the program using the Perceptron
		//that is already embedded with the location this function is
		//interested in.
		
	//Unfortunately to check, we must first look through the entire Branch
		//History List. We'll use a boolean type variable called found to
		//tell us if we've spoted the required information or not.
	bool found = 0;
	
	//We'll start at the top of the list:
	loc_pointer = branch_table->branch_list->msh;
	
	while(loc_pointer != NULL){
		//All we'll do is see if the branch_codes match. If they
			//do then we found the location already in our list.
			//We'll then return from this function.
		if(strcmp(loc_pointer->branch_code,location->branch_code) == 0)
			return;
			
		loc_pointer = loc_pointer->next_location;
	}
	
	//Next we have to see if the Branch List is full. If the Branch List is
		//full then things become complicated.
	
	//The Branch History List isn't like our Shift or Weight Registers. We can't
		//simply just delete a Branch Location from the end of the List and then
		//add a new one to the top of the list. Nor will we not add the branch
		//to the branch list.
		
	//Unforutnately, if the Branch History List is full and we need to add a new
		//location, that means we must replace something first.
		//Luckily we have a function that will do that dirty work for us.
		//We'll pass adding the location to our Replace Function at this point.
	if(branch_table->branch_list->curr_size == branch_table->branch_list->max_size){
		Replace_History( branch_table->branch_list, location, branch_table->replacement  );
	}
	
	//If, the list isn't empty, the branch_code doesn't already exist, and we
		//aren't at maximum list capacity. Then we can simply add the Branch
		//Location to the top of our list.
	else{
		//First we attach the new location to the list:
		location->next_location = branch_table->branch_list->msh;
		location->prev_location = NULL;
		
		//Then we set the new location as the head of the Branch List:
		branch_table->branch_list->msh = location;
		
		//Then we increase the size counter:
		branch_table->branch_list->curr_size += 1;
	}
	
	return;
}

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	void Branch_Delete_Location(
		BHList *branch_list,
		BHL *location
	);
*/
void Branch_Delete_Location( BHList *branch_list, BHL *location ){
	//To delete a Branch Location we must first see if the location exists
		//within the branch in the first place.
		//If it does then we can maneuver ourself nicely to be able to delete
		//it rather painlessly.
	
	//First thing first though, we'll see if the list is empty.
		//If it is we'll simply return:
	if(branch_list->msh == NULL && branch_list->lsh == NULL)
		return;
	
	//If it isn't then we need a pointer to traverse the list:
	BHL *loc_pointer = NULL;
	
	loc_pointer = branch_list->msh;
	
	while(loc_pointer != NULL){
		//If we find the location we want to delete then we'll break from the
		//while loop. This will also allow our loc_pointer to maintain the exact
		//location of what we want to delete:
		if(loc_pointer->branch_code == location->branch_code)
			break;
			
		loc_pointer = loc_pointer->next_location;
	}
	
	//First we check to see if the location is the head of the list:
	if(loc_pointer == branch_list->msh){
		//If so, we need to move the branch list most-significant-history down the list.
		branch_list->msh = branch_list->msh->next_location;
		
		//Then we disconnect the location:
		loc_pointer->next_location = NULL;
		loc_pointer->prev_location = NULL;
		
		//Then we decrease the list's current size by -1.
		branch_list->curr_size -= 1;
		
		//Then we free the location:
		free(loc_pointer);
	}
	
	//Then we check if the location is the end of the list:
	else if(loc_pointer == branch_list->lsh){
		//If so, we need to move the branch list least-significant-history up the list.
		branch_list->lsh = branch_list->lsh->prev_location;
		
		//Then we disconnect the location:
		loc_pointer->next_location = NULL;
		loc_pointer->prev_location = NULL;
		
		//Then we decrease the list's current size by -1.
		branch_list->curr_size -= 1;
		
		//Then we free the location:
		free(loc_pointer);	
	}
	
	//Lastly we just need to adjust the pointers around the 
	else{
		//First we set the location's previous pointer to its' next pointer:
		loc_pointer->prev_location->next_location = loc_pointer->next_location;
		
		//Then we set the location's next pointer ot its' prev pointer:
		loc_pointer->next_location->prev_location = loc_pointer->prev_location;
		
		//Then we disconnect the location:
		loc_pointer->next_location = NULL;
		loc_pointer->prev_location = NULL;
		
		//Then we decrease the list's current size by -1.
		branch_list->curr_size -= 1;
		
		//Then we free the location:
		free(loc_pointer);
	}
	
	return;
}

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	void Branch_Delete_List(
		BHList *branch_list
	);
*/
void Branch_Delete_List( BHList *branch_list ){
	//To delete the entire Branch History List we'll start at the top
		//and work our way down.
		
	BHL *loc_pointer = NULL;
	
	loc_pointer = branch_list->msh;
	
	while(loc_pointer != NULL){
		//First, move the branch list msh pointer:
		branch_list->msh = branch_list->msh->next_location;
		
		//Then disconnect the location pointer:
		loc_pointer->next_location = NULL;
		loc_pointer->prev_location = NULL;
		
		//Then decrease the list's current size by -1.
		branch_list->curr_size -= 1;
		
		//Then free the location:
		free(loc_pointer);
		
		//Then make location pointer equal the next branch_list->msh:
		loc_pointer = branch_list->msh;
	}
}

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	void Replace_History(
		BHList *list,
		BHL *new_location
	);
*/
void Replace_History( BHList *list, BHL *new_location, Replacement_Type replace ){
	//The most important aspect of this entire function is how do we replace
		//the desired location. That will depend greatly on how the Branch
		//History Table was initalized:
		
	//First thing first though, check to see if we recieved an empty list.
		//If we did we shall simply return from this function:
	if(list->msh == NULL && list->lsh == NULL)
		return;
		
	//Next, we'll use a switch statement to determine which replacement
		//method to utilize:
	if(replace == Random_repl){
			//In this case we'll need to use both a random number, and a 
			//location pointer to traverse the list, and a counter
			//to count up to the specified random number amount:
			
			int target = (int)rand() % list->max_size + 1;
			int count = 0;
			BHL *loc_pointer = list->msh;
			
			//We'll then loop through the list until we reach the designated 
				//location. We'll then delete it, and replace it.
			while(loc_pointer != NULL && count != target){
				loc_pointer = loc_pointer->next_location;
				
				count++;
			}
			
			//Once we're done looping, we'll add the new location to the list by 
				//checking if the location is the head or tail, or simply
				//rearange some pointers in the list:
			
			//Head of List scenario:
			if(loc_pointer == list->msh){
				//Attach the location to the list:
				new_location->next_location = list->msh->next_location;
				new_location->prev_location = NULL;
				
				//Allow the list to attach itself to the new_location
				list->msh->next_location->prev_location = new_location;
				
				//Connect the new location to the old location's perceptron:
				new_location->percep = list->msh->percep;
				
				//Adjust Head pointer of list:
				list->msh = new_location;
				
				//Disconnect the location pointer:
				loc_pointer->next_location = NULL;
				loc_pointer->prev_location = NULL;
				loc_pointer->percep = NULL;
				
				//Free it:
				free(loc_pointer);
			}
			
			//Tail scenario:
			else if(loc_pointer == list->lsh){
				//Attach the location to the list:
				new_location->prev_location = list->lsh->prev_location;
				new_location->next_location = NULL;
				
				//Allow the list to attack itself to the new_location:
				list->lsh->prev_location->next_location = new_location;
				
				//Connect the new location to the old location's perceptron:
				new_location->percep = list->lsh->percep;
				
				//Adjust the Tail pointer of the list:
				list->lsh = new_location;
				
				//Disconnect the location pointer:
				loc_pointer->next_location = NULL;
				loc_pointer->prev_location = NULL;
				loc_pointer->percep = NULL;
				
				//Free it:
				free(loc_pointer);
			}
				
			//Not the Head or Tail scenario:
			else{
				//Attach the location to the list:
				new_location->prev_location = loc_pointer->prev_location;
				new_location->next_location = loc_pointer->next_location;
				
				//Connect the new location to the old location's perceptron:
				new_location->percep = loc_pointer->percep;
				
				//Allow the list to attach itself to the new_location:
				loc_pointer->next_location->prev_location = new_location;
				loc_pointer->prev_location->next_location = new_location;
				
				//Disconnect the location pointer:
				loc_pointer->next_location = NULL;
				loc_pointer->prev_location = NULL;
				loc_pointer->percep = NULL;
				
				//Free it:
				free(loc_pointer);
			}
			
		}
			
		else if(replace == oldest){
			//In this case we'll need to traverse the list, use a maximum,
				//and a location counter. We'll then need to traverse the list
				//to find the location with the greatest age.
				//We'll then replace that location:
			int loc_count = 0;
			int oldest_count = 0;
			unsigned int oldest = 0;
			BHL *loc_pointer = list->msh;
			
			while(loc_pointer != NULL){
				//Search for the oldest location:
				if(loc_pointer->age > oldest){
					oldest = loc_pointer->age;
					oldest_count = loc_count;
				}
				
				loc_pointer = loc_pointer->next_location;
				loc_count++;
			}
			
			//We then need to loop through the list one more time. This time we'll
				//break from the loop the moment we have returned to the oldest
				//location by means of the oldest_count == loc_count:
			loc_count = 0;
			loc_pointer = list->msh;
			while(oldest_count != loc_count){
				if(oldest_count == loc_count)
					break;
					
				loc_count++;
				loc_pointer = loc_pointer->next_location;
			}
			
			//Now that we have our location, we can replace it using the same methodology as the 
				//random case above:
				
			//Head of List scenario:
			if(loc_pointer == list->msh){
				//Attach the location to the list:
				new_location->next_location = list->msh->next_location;
				new_location->prev_location = NULL;
				
				//Allow the list to attach itself to the new_location
				list->msh->next_location->prev_location = new_location;
				
				//Connect the new location to the old location's perceptron:
				new_location->percep = list->msh->percep;
				
				//Adjust Head pointer of list:
				list->msh = new_location;
				
				//Disconnect the location pointer:
				loc_pointer->next_location = NULL;
				loc_pointer->prev_location = NULL;
				loc_pointer->percep = NULL;
				
				//Free it:
				free(loc_pointer);
			}
			
			//Tail scenario:
			else if(loc_pointer == list->lsh){
				//Attach the location to the list:
				new_location->prev_location = list->lsh->prev_location;
				new_location->next_location = NULL;
				
				//Allow the list to attack itself to the new_location:
				list->lsh->prev_location->next_location = new_location;
				
				//Connect the new location to the old location's perceptron:
				new_location->percep = list->lsh->percep;
				
				//Adjust the Tail pointer of the list:
				list->lsh = new_location;
				
				//Disconnect the location pointer:
				loc_pointer->next_location = NULL;
				loc_pointer->prev_location = NULL;
				loc_pointer->percep = NULL;
				
				//Free it:
				free(loc_pointer);
			}
				
			//Not the Head or Tail scenario:
			else{
				//Attach the location to the list:
				new_location->prev_location = loc_pointer->prev_location;
				new_location->next_location = loc_pointer->next_location;
				
				//Connect the new location to the old location's perceptron:
				new_location->percep = loc_pointer->percep;
				
				//Allow the list to attach itself to the new_location:
				loc_pointer->next_location->prev_location = new_location;
				loc_pointer->prev_location->next_location = new_location;
				
				//Disconnect the location pointer:
				loc_pointer->next_location = NULL;
				loc_pointer->prev_location = NULL;
				loc_pointer->percep = NULL;
				
				//Free it:
				free(loc_pointer);
			}
		}
				
		else{
			//For this case we'll need to use a pointer to traverse the list,
				//we'll also need a least used and a couple of counters:
			int loc_count = 0;
			int low_use = 99999999;
			int low_count = 0;
			BHL *loc_pointer = list->msh;
			
			//One thing about this method:
				//This method will remove the lowest used location. To calculate
				//this it will be looking for (age - use) and grab those closest to 0.
				//If multiples occur, it will replace the first one found.
			while(loc_pointer != NULL){
				//Search for lowest use:
				if((loc_pointer->age - loc_pointer->use) < low_use){
					low_use = (loc_pointer->age - loc_pointer->use);
					low_count = loc_count;
				}
				
				loc_count++;
				loc_pointer = loc_pointer->next_location;
			}
			
			//Once we've located the lowest used location, we'll need to retraverse the list
				//to find it again. After we've found it we'll immediatly break
				//from the loop:
			loc_count = 0;
			loc_pointer = list->msh;
			while(low_count != loc_count){
				if(low_count == loc_count)
					break;
				
				loc_count++;
				loc_pointer = loc_pointer->next_location;
			}
			
			//Now that we have the location, we can replace it in a similar manner as 
				//the two methods above:
			
			//Head of List scenario:
			if(loc_pointer == list->msh){
				//Attach the location to the list:
				new_location->next_location = list->msh->next_location;
				new_location->prev_location = NULL;
				
				//Allow the list to attach itself to the new_location
				list->msh->next_location->prev_location = new_location;
				
				//Connect the new location to the old location's perceptron:
				new_location->percep = list->msh->percep;
				
				//Adjust Head pointer of list:
				list->msh = new_location;
				
				//Disconnect the location pointer:
				loc_pointer->next_location = NULL;
				loc_pointer->prev_location = NULL;
				loc_pointer->percep = NULL;
				
				//Free it:
				free(loc_pointer);
			}
			
			//Tail scenario:
			else if(loc_pointer == list->lsh){
				//Attach the location to the list:
				new_location->prev_location = list->lsh->prev_location;
				new_location->next_location = NULL;
				
				//Allow the list to attack itself to the new_location:
				list->lsh->prev_location->next_location = new_location;
				
				//Connect the new location to the old location's perceptron:
				new_location->percep = list->lsh->percep;
				
				//Adjust the Tail pointer of the list:
				list->lsh = new_location;
				
				//Disconnect the location pointer:
				loc_pointer->next_location = NULL;
				loc_pointer->prev_location = NULL;
				loc_pointer->percep = NULL;
				
				//Free it:
				free(loc_pointer);
			}
				
			//Not the Head or Tail scenario:
			else{
				//Attach the location to the list:
				new_location->prev_location = loc_pointer->prev_location;
				new_location->next_location = loc_pointer->next_location;
				
				//Connect the new location to the old location's perceptron:
				new_location->percep = loc_pointer->percep;
				
				//Allow the list to attach itself to the new_location:
				loc_pointer->next_location->prev_location = new_location;
				loc_pointer->prev_location->next_location = new_location;
				
				//Disconnect the location pointer:
				loc_pointer->next_location = NULL;
				loc_pointer->prev_location = NULL;
				loc_pointer->percep = NULL;
				
				//Free it:
				free(loc_pointer);
			}
		}
	}

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	void Delete_History(
		BHT *table
	);
*/
void Delete_History( BHT *table ){
	//To delete the history table we'll simply
		//use our existing functions to delete the Perceptron List
		//and Branch History List for us.
	Delete_Perceptron( table->percep_list );
	Branch_Delete_List ( table->branch_list );
	
	return;
}

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	struct BHL *Search_Table(
		BHT *table, 
		char* branch_code
	);
*/
struct Branch_History_Location *Search_Table( BHT *table, char* branch_code ){
	//First we must check to see if the table contains an empty Branch
	//History List or not. If it does contain an empty Branch History
	//List we'll return NULL.
	
	if(table->branch_list->msh == NULL && table->branch_list->lsh == NULL)
		return NULL;
		
	//If not then we must check if the branch contains the location we are
		//interested in. To do this we'll need to loop through the list
		//until we find it.
		//If we do not, then we'll return NULL.
		//If we DO find it then we'll return that location.
	BHL *loc_pointer = table->branch_list->msh;
	BHL *pointer = table->branch_list->msh;
	
	//We'll need this variable to keep from acidentily sending back
		//the wrong location.
	bool found = 0;
	
	while(loc_pointer != NULL && pointer != NULL){
		//We must do several things in this loop:
			//1) Check if the loc_pointer's branch_code == the branch_code.
			//2) Update the age of all the locations in the Branch List.
			//3) Update the use of all the locations in the Branch List.
			//4) If we find the location we are interested in, reset its use to 0.
			
		if(strcmp(loc_pointer->branch_code, branch_code) == 0){
			found = 1;
			loc_pointer->use = 0;
		}
		
		if(found < 1){
			loc_pointer = loc_pointer->next_location;
		}
		
		pointer->age += 1;
		pointer->use += 1;
		
		pointer = pointer->next_location;
	}
	
	if(found < 1)
		return NULL;
	
	else
		return loc_pointer;
	
}
	
//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
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
*/	
void Test_File(char* in_file, char* out_file, int depth, int reg_size, signed char max_weight, signed char min_weight, 
				signed short int threshold, Replacement_Type replacement)
{
	//First we must generate the Branch History Table:
	BHT *table = NULL;
	
	table = Table( depth, reg_size, max_weight, min_weight, threshold, replacement );
	
	//Then we'll attempt to open the in_file for reading.
		//If we cannot open it, we'll warn the user of the error and exit
		//the program under exit(EXIT_FAILURE) conditions.
	FILE *testFile = fopen(in_file, "r");
	
	if(testFile == NULL){
		fclose(testFile);
			
		printf("ERROR: FILE ERROR. CHECK INPUT FILE NAME AND PATH.\n");
		printf("\tEXITING PROGRAM UNDER exit(EXIT_FAILURE) CONDITIONS.\n\n");
		
		exit(EXIT_FAILURE);	
	}
	
	//Assuming we have opened the file, the next thing we need to do is begin testing.
	
	//Testing will be in this form:
		//1) Grab the first segment of the line of the testFile. This line is supposed
			//to contain the branch_code.
		//2) Search the Branch History Table for a branch location that matches this branch_code.
			//2a) If the search returned NULL we'll need to generate a new Branch Location
				//and attach it to our Branch History Table and List.
		//3) Once we have the location, we'll then test the Perceptron for its prediction.
		//4) Once we have the Perceptron's prediction, we'll send the actual result which is
			//the third segment of the line of the testFile into the Perceptron to influence
			//its' training and Shift Register control.
		//5) We'll continue with this until the file has reached the end of the file.
		
	//We'll need a character pointer to grab each segment of the line:
	char* stream = NULL;
	
	//For now we'll use a 32-bit system for our Branch Addresses.
	int branch_size = 32;
	
	//We'll also need 1 bit for the direction of the branch.
	char result_size = 1;
	
	//We'll continue to loop until we reach the end of the file.
		//A break statement within the While Loop will ensure this.
	while(1){
		if(feof(testFile))
			break;
			
		//A few thing we will need within this loop:
			//1) A Branch History Location pointer
			//2) A Perceptron pointer so we can generate new Perceptrons if our
				//Perceptron List is empty.
			//3) a prediction booolean that will hold our Perceptron's prediction value.
			//4) an integer value that will hold the actual value of the branch decision
				//which will be acquired through the use of fgetc.
			//5) a boolean test value that will hold the branch's actual result after it
				//is acquired from the fgetc.
			//6) a Bit pointer so we can add new Bits to the perceptron's Shift Register.
			
		BHL *loc_pointer = NULL;
		Perceptron *percep_pointer = NULL;
		bool prediction = NULL;
		int check_value = 0;
		bool actual_result = 0;
		Bit *new_bit = NULL;
		
		//Grab the Branch Code:
		fgets(stream, branch_size, testFile);
		
		//Search the Table for the branch_code:
		loc_pointer = Search_Table( table, stream );
		
		//If the loc_pointer is still equal to NULL then we either have nothing in our
			//Branch History List, or didn't find the Location.
			//In both cases we'll need to generate it.
		if(loc_pointer == NULL){
			loc_pointer = (BHL*)malloc(sizeof(BHL));
			
			//Set age and use to 0:
			loc_pointer->age = 0;
			loc_pointer->use = 0;
			
			//Set the branch_code to the current stream's code:
			loc_pointer->branch_code = stream;
			//Set the target_code equal to the next, segment:
			loc_pointer->branch_location = fgets(stream, branch_size, testFile);
			
			//Check to see if we still have room for Perceptrons within our Perceptron List:
			if(table->percep_list->size < table->percep_list->depth){
				//If so, generate a new Perceptron:
				percep_pointer = Perceptron_Init( table->reg_size, table->max_weight, table->min_weight, table->threshold, table->depth );
				
				//Then attach the new location's perceptron pointer to this perceptron:
				loc_pointer->percep = percep_pointer;
				
				//Then attach the new perceptron to the table's perceptron list:
				Percep_Add_Percep( table->percep_list, percep_pointer );
			}
			
			//Add the new location to the Branch History List:
			Branch_Add_Location( table, loc_pointer );
		}
		
		//Once we have the location we need, it is time to run the Perceptron's testing:
			//First, we'll need to grab the branch's actual direction.
		
		check_value = fgetc(testFile);
		
		actual_result = check_value - 48;
		
		prediction = Decision( table->threshold, Sum_Weight(loc_pointer->percep), loc_pointer->percep);
		
		Perceptron_Training(prediction, actual_result, loc_pointer->percep);
		
		//After we have finished predicting and training we need to add the new bit to the perceptron's shift register:
		
		new_bit = (Bit*)malloc(sizeof(Bit));
		
		new_bit->bit_value = prediction;
		
		Shift_Add_Bit( loc_pointer->percep->shift_reg, new_bit );
		
		//We shall continue to loop through the file in this manner until it has finished:
	}
	
	//Once we have finished with the file we need to loop through the entire table and print out
		//the information each perceptron is holding onto.
	
	//To do that we need a Perceptron Pointer:
	Perceptron *percep_pointer = NULL;
	
	//We'll begin at the top of the list:
	percep_pointer = table->percep_list->msp;
	
	while(percep_pointer != NULL){
		Write_Output( percep_pointer );
		
		percep_pointer = percep_pointer->next_percep;
	}				
	
	//Then we'll delete the History Table:
	Branch_Delete_List( table->branch_list );
	Delete_Perceptron( table->percep_list );
	
	//Then we'll return from the function:
	return;
}
#endif
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
