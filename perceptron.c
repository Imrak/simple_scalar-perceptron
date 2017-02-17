//Includes Required:
#include "perceptron.h"

//-----------------------------------------------------------------------------//
//
//	                           STRUCTURES
//
//----------------------------------------------------------------------------//
/*
	typedef struct Percep_Data{
		int hit_count;
		
		int miss_count;
		
		char *in_file;
		
		char *out_file;
		
		int train_count;
		
		unsigned char depth;
	
		signed char max_weight;
		
		signed char min_weight;
		
		unsigned char threshold;
		
		unsigned short int links;
		
	}Percep_Data;

	typedef struct Perceptron{
		Shift_Register shift_reg;
		
		Weight_Register weight_reg;
		
		Percep_Data percep_data;
		
		struct Perceptron *next_percep;
		
		struct Perceptron *prev_percep;
		
	}Perceptron;
	
	typedef struct Percep_List{
		Perceptron *msp;
		
		Perceptron *lsp;
		
		int size;
		
		int depth;
		
	}Percep_List;
*/

//-----------------------------------------------------------------------------//
//
//								FUNCTIONS	  
//                      
//----------------------------------------------------------------------------//
/*
	void Data_Init(
		Percep_Data *percep_data
	);
*/
struct Percep_Data *Data_Init( unsigned char depth, signed char max_weight, 
				signed char min_weight, signed char threshold, unsigned short int links ){
	struct Percep_Data *percep_data = NULL;
	
	percep_data = (Percep_Data*)malloc(sizeof(Percep_Data));
	
	percep_data->hit_count = 0;
	
	percep_data->miss_count = 0;
	
	percep_data->in_file = NULL;
	
	percep_data->out_file = NULL;
	
	percep_data->train_count = 0;
	
	percep_data->depth = depth;
	
	percep_data->max_weight = max_weight;
	
	percep_data->min_weight = min_weight;
	
	percep_data->threshold = threshold;
	
	percep_data->links = links;
	
	percep_data->train_value = 1;
	
	return percep_data;
}

//-----------------------------------------------------------------------------//
//
//	                        
//----------------------------------------------------------------------------//
/*
	struct Perceptron *Perceptron_Init(
		Perceptron *perceptron
	);
*/
struct Perceptron *Perceptron_Init( unsigned char depth, signed char max_weight, signed char min_weight, 
									signed char threshold, unsigned short int links){
		//We'll begin by creating a pointer to a Perceptron structure. This will be the structure we return.								
	struct Perceptron *percep = NULL;
	
		//Allocate the space in memory required for the entire Perceptron.
	percep = (Perceptron*)malloc(sizeof(Perceptron));
	
	//If there isn't enough memory to generate a perceptron, we'll warn the user and then
		//exit the program using the exit(EXIT_FAILURE) code.
	if(percep == NULL){
		printf("ERROR: OUT OF MEMORY WHEN ATTEMPTING TO MAKE PERCEPTRON!\n");
		printf("\tEXITING PROGRAM UNDER : exit(EXIT_FAILURE)\n\n");
		exit(EXIT_FAILURE);
	}
	
		//Initialize the Shift Register.
	percep->shift_reg = Shift_Init( depth );
	
		//Initialize the Weight Register.
	percep->weight_reg = Weight_Init( depth );
	
		//Initialize the Perceptron's Data.
	percep->percep_data = Data_Init( depth, max_weight, min_weight, threshold, links );
	
		//Set the Perceptron's Next and Previous pointer to NULL.
	percep->next_percep = NULL;
	
	percep->prev_percep = NULL;
	
		//Now, we'll use a for loop to generate the Bits and Weights required for our Perceptron's
			//Shift and Weight registers. When this is returned it will have everything required to
			//immediatly begin.
	int i = 0;
	
	for(i = 0; i < depth; i++){
		//We must first create both Bit and Weight pointers.
		Bit *new_bit = NULL;
		Weight *new_weight = NULL;
		
		//Allocate the memory for the Bit and Weights:
		new_bit = (Bit*)malloc(sizeof(Bit));
		new_weight = (Weight*)malloc(sizeof(Weight));
		
		//If there isn't enough memory to generate the bits and weights required, we need to warn the
			//user. Additionally we'll actually exit the program under exit(EXIT_FAILURE) conditions
			//so that they know they need to change some of the code.
		if(new_bit == NULL){
			printf("ERROR: OUT OF MEMORY WHEN ATTEMPTING TO MAKE NEW BIT!\n");
			printf("\tEXITING PROGRAM UNDER : exit(EXIT_FAILURE)\n\n");
			exit(EXIT_FAILURE);
		}
		
		if(new_weight == NULL){
			printf("ERROR: OUT OF MEMORY WHEN ATTEMPTING TO MAKE NEW WEIGHT!\n");
			printf("\tEXITING PROGRAM UNDER : exit(EXIT_FAILURE)\n\n");
			exit(EXIT_FAILURE);
		}
		
		//Then we generate the Bit values = 0, and the random weight values.
			//The random weight values must be between our max & min weights, so
			//we'll use the equation of min - (rand % range) + 1 to give us the correct weight
			//values.
		
		new_bit->bit_value = 0;
		
		//Due to the nature of the random numbers, we are more likely to generate negative
			//values than positive ones. As such, every [ EVEN ] weight we make, will be
			//a forced positive, and every [ ODD ] weight we make will be a forced negative.
			//This way the randomness at the beginning will be evened out.
		if(i % 2 == 0){
			new_weight->weight_value = min_weight - (rand() % (max_weight - min_weight) + 1);
			
			if(new_weight->weight_value < 0)
				new_weight->weight_value *= -1;
		}
		
		else{
			new_weight->weight_value = min_weight - (rand() % (max_weight - min_weight) + 1);
			
			if(new_weight->weight_value > 0)
				new_weight->weight_value *= -1;
		}
		
		//Now that the bit and weight are set. We'll add them to their respective Registers:
		Shift_Add_Bit( percep->shift_reg, new_bit);
		Weight_Add_Weight( percep->weight_reg, new_weight);
	}
	
	//The last thing we will do is set our weigh'd Bit value. For now, the Least Significant Bit will
		//always be considered a 1.
	percep->shift_reg->lsb->bit_value = 1;
	
	//Finally, we'll return the perceptron.
	return percep;
}

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	void Per_List_Init( 
		Percep_List *per_list 
	);
*/
struct Percep_List *Per_List_init( int depth ){
	Percep_List *pList = NULL;
	
	pList = (Percep_List*)malloc(sizeof(Percep_List));
	
	pList->depth = depth;
	
	pList->lsp = NULL;
	
	pList->msp = NULL;
	
	pList->size = 0;
	
	return pList;
}

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	void Percep_Add_Percep(
		Percep_List *percep_list,
		Perceptron *new_percep
	);
*/
void Percep_Add_Percep( Percep_List *percep_list, Perceptron *new_percep ){
	//First we must check that the Perceptron List isn't empty.
		//If it is we simply attach the new_perceptron as the first in the list.
	if( percep_list->msp == NULL && percep_list->lsp == NULL ){
		percep_list->msp = new_percep;
		
		percep_list->lsp = new_percep;
	}
	
	//Second we much check that the Perceptron List is not at maximum capacity.
		//If it is we'll display a user warning, and continue with the program.
	else if( percep_list->size == percep_list->depth ){
		printf("ERROR: PERCEPTRON LIST AT MAXIMUM CAPACITY. NO NEW PERCEPTRONS WILL BE ADDED.\n");
		printf("\tEXITING FUNCTION. CONTINUING PROGRAM.\n\n");
		return;
	}
	
	//Third we simply add the new perceptron to the top of the Perceptron List.
	else{
		//Start by making the old msp's previous perceptron equal the new perceptron:
		percep_list->msp->prev_percep = new_percep;
		
		//Then attach the new_perceptron to the list:
		new_percep->next_percep = percep_list->msp;
		
		//Then move the msp pointer to the new perceptron:
		percep_list->msp = new_percep;
	}
	
	//Before we leave we'll increase the size of the perceptron list:
	percep_list->size += 1;
	
	return;
}

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	void Remove_Perceptron(
		Percep_List *percep_list,
		Perceptron *target_percep
	);
*/
void Remove_Perceptron( Percep_List *percep_list, Perceptron *target_percep ){
	//First we'll check that we weren't given a empty Perceptron List.
		//If we were, we'll display an error and exit the function.
	if( percep_list->msp == NULL && percep_list->lsp == NULL ){
		printf("ERROR: EMPTY PERCEPTRON LIST PASSED TO REMOVE PERCEPTRON FUNCTION.\n");
		printf("\tEXITING FUNCTION. CONTINUING PROGRAM.\n\n");
		return;
	}
	
	//Second we'll begin by finding the target perceptron.
		//To find the target perceptron we must first loop through the Perceptron List.
		//We'll do this using a perceptron pointer:
	Perceptron *percep_pointer = NULL;
	
	//We'll use a boolean type variable to tell us if we found the target or not.
	char found = 0;
	
	//We'll start at the top of the list:
	percep_pointer = percep_list->msp;
	
	//And continue to loop through it until the end.
	while( percep_pointer != NULL ){
		if(percep_pointer == target_percep){
			found = 1;
			break;
		}
		
		percep_pointer = percep_pointer->next_percep;
	}
	
	//Now that we know if we found it or not, we'll check to see if we found it.
		//If we did not, then we'll display a user error and exit the function.
	if(!found){
		printf("ERROR: TARGET PERCEPTRON NOT FOUND IN PERCEPTRON LIST.\n");
		printf("\tEXITING FUNCTION. CONTINUING PROGRAM.\n\n");
		return;
	}
	
	//Then we need to check that the size of the Perceptron List is not 1.
		//The Perceptron List must maintain at least 1 Perceptron while the program
		//is running. If this is true, then we'll display an error and continue
		//the program.
	else if(percep_list->size < 2){
		printf("ERROR: PERCEPTRON LIST MUST MAINTAIN AT LEAST 1 PERCEPTRON DURING PROGRAM.\n");
		printf("\tEXITING FUNCTION. CONTINUING PROGRAM.\n\n");
		return;
	}
	
	//If we make it all the way to this step, then it means our Perceptron was found, and
		//our List is large enough that we can delete it. We'll begin removing it from
		//the list:
	else{
		//First we must check if the target perceptron is the msp:
		if(target_percep == percep_list->msp){
			//If this is true, we simply need to adjust the msp pointer
				//and then we can remove the target.
			percep_list->msp = target_percep->next_percep;
			
			//We'll then detach the target perceptron:
			target_percep->next_percep = NULL;
			target_percep->prev_percep = NULL;
		}
		
		//Second we must check if the target perceptron is the lsp:
		else if(target_percep == percep_list->lsp){
			//If this is true, we simply need to adjust the lsp pointer
				//and then we can remove the target.
			percep_list->lsp = target_percep->prev_percep;
			
			//We'll then detach the target perceptron:
			target_percep->next_percep = NULL;
			target_percep->prev_percep = NULL;
		}
		
		//If the target is netiher the msp or the lsp, then it is somewhere in the middle.
			//We need to adjust the perceptrons around it carefully so that we can
			//correctly free the target perceptron.
		else{
			//We'll begin by setting the target's previous perceptron to the target's
				//next perceptron:
			target_percep->prev_percep->next_percep = target_percep->next_percep;
			
			//Then we'll set the target's next perceptron to the target's previous
				//perceptron:
			target_percep->next_percep->prev_percep = target_percep->prev_percep;
			
			//Then we'll detach the target:
			target_percep->next_percep = NULL;
			target_percep->prev_percep = NULL;
		}
		
		//Before we can free the target perceptron, we must first ensure
			//that we delete the perceptron's Shift and Weight Registers
			//as well. Or we could have memory leaks.
		Delete_Shift_Register( target_percep->shift_reg );
		Delete_Weight_Register( target_percep->weight_reg );
		
		//Once those have been deleted, we can then safetly free the
			//target perceptron and decrement the size of the
			//Perceptron List.
		free(target_percep);
		
		percep_list->size -= 1;
	}
	
	return;
}

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	void Delete_Perceptron(
		Percep_List *percep_list
	);
*/
void Delete_Perceptron( Percep_List *percep_list ){
	//To delete the entire Perceptron List we'll start at
		//the msp position and work our way down the list.
		//We must ensure that we also delete each perceptron's Shift and
		//Weight registers as we go to ensure proper memory deallocation.
	Perceptron *percep_pointer = percep_list->msp;
	
	while( percep_pointer != NULL ){
		percep_list->msp = percep_list->msp->next_percep;
		
		percep_pointer->next_percep = NULL;
		percep_pointer->prev_percep = NULL;
		
		Delete_Shift_Register( percep_pointer->shift_reg );
		Delete_Weight_Register( percep_pointer->weight_reg );
		
		free(percep_pointer);
		
		percep_pointer = percep_list->msp;
	}
	
	return;
}

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	int Sum_Weights(
		Perceptron *percep
	);
*/
int Sum_Weight( Perceptron *percep ){
	//We'll use a loaded summation program with a weight bias of +1 at all times.
	int sum = 1;
	
	//To sum the weight's of the Perceptron we must look at the Bit that corresponds to
		//each Weight. If the Bit is a 0 then we will subtract the weight_value.
		//If the Bit is a 1, then we will add the weight_value.
	
	//To correctly do this we must loop through both the Shift and Weight registers
		//at the exact same time. So we'll use a bit_pointer and a weight_pointer.
	Bit *bit_pointer = percep->shift_reg->msb;
	Weight *weight_pointer = percep->weight_reg->msw;
	
	//The code is designed so that the Shift and Weight registers maintain the same size.
		//As such we can continue a while loop until both pointers reach the ends
		//of their respective lists.
	while(bit_pointer != NULL && weight_pointer != NULL){
		//First, if the bit_pointer's bit_value == 0, we'll subtract the
			//weight_value from the sum.
		if(bit_pointer->bit_value == 0)
			sum -= weight_pointer->weight_value;
		
		//Otherwise, we'll add to it.
		else
			sum += weight_pointer->weight_value;
			
		bit_pointer = bit_pointer->next_bit;
		
		weight_pointer = weight_pointer->next_weight;
	}
	
	//Once we've finished looping and summing, we'll return the sum.
	
	return sum;
}

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	char Decision(
		int threshold,
		int sum,
		bool actual
	);
*/
char Decision( int threshold, int sum, Perceptron *percep ){
	char decision = 0;
	
	//First we check if the sum we got from our Sum_Weight function is less than
		//the threshold value. If it is, decision maintains its' value of 0.
	if(sum < threshold)
		decision = 0;
	
	else
		decision = 1;
	
	//Then we'll return the decision:
	return decision;
}

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	void Perceptron_Training(
		bool decision,
		Perceptron *percep
	);
*/
void Perceptron_Training( char decision, char actual, Perceptron *percep ){
	//First we must check to see if the decision was a 1 or a 0.
		//A 1 means that the Perceptron predicted correctly. 
		//A 0 means the Perceptron predicted incorrectly.
	
	//If the Perceptron predicted correctly, then we need not train.
	if(decision)
		return;
		
	//If the Perceptron did not predict correctly, it must train.
	else{
		//We'll increment the perceptron's train counter by +1.
		percep->percep_data->train_count += 1;
		
		//We the must cycle through both the Weight and Shift Registers simultaneously.
			//To do that we'll need both a bit_pointer and a weight_pointer.
		Bit *bit_pointer = percep->shift_reg->msb;
		Weight *weight_pointer = percep->weight_reg->msw;
		
		//We'll then while loop through the Registers to train the weights accordingly.
			//If the bit_value matches the [ actual ] we'll [ increase ] the corresponding
			//weight value by the train_value within the Perceptron.
			//If the bit_value does not match, then we'll [ decrease ] the corresponding
			//weight value by the train_value within the Perceptron.
		while(bit_pointer != NULL && weight_pointer != NULL){
			if(bit_pointer->bit_value == actual)
				weight_pointer->weight_value += percep->percep_data->train_value;
			
			else
				weight_pointer->weight_value -= percep->percep_data->train_value;
				
			bit_pointer = bit_pointer->next_bit;
			weight_pointer = weight_pointer->next_weight;
		}
	}
}

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	void Set_Files( 
		char *in_file, 
		char *out_file, 
		Perceptron *percep
	);
*/
void Set_Files( char *in_file, char *out_file, Perceptron *percep ){
	percep->percep_data->in_file = in_file;
	
	percep->percep_data->out_file = out_file;
}

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	void Test_From_File(
		unsigned char depth,
		unsigned short int list,
		signed char max_weight,
		signed char min_weight,
		signed char threshold,
		char *in_file,
		char *out_file
	);
*/
void Test_From_File( unsigned char depth, unsigned short int list, signed char max_weight, signed char min_weight, signed char threshold,
					char *in_file, char *out_file ){
	//First generate the Perceptron List;
	Percep_List *pList = NULL;
	
	pList = (Percep_List*)malloc(sizeof(Percep_List));
	
	pList = Per_List_init( depth );
	
	//Then generate a Perceptron until we have reached maximum List Size:
		//NOTE: For now the size should be 1.
	int i = 0;
	
	for(i = 0; i < list; i++){
		Perceptron *perceptron = NULL;
		
		perceptron = Perceptron_Init( depth, max_weight, min_weight, threshold, list );
		
		Set_Files( in_file, out_file, perceptron );
		
		Percep_Add_Percep( pList, perceptron );
	}
	
	//Once the List has been created with the Perceptron(s), we are then ready to begin testing.
	
	//To test:
		//We must first open the in_file for reading purposes.
		//We must loop through the file, one character at a time, until we have reached the end
		//	of the file.
		//For each 'bit' that we read from the file, we'll send it through the training system 
		//	of the Perceptron.
		//This means we'll run the Summation Function, send it into the Decision Function,
		//	and then Send that into the Training Function.
		
		//We then must generate a new Bit whose value is equal to the 'Bit' that we have
		//	read in from the file.
		//We will then add this bit to our Shift Register.
		
		//Once the entire file is completed. We'll then generate the output.
	FILE *testFile;
	
	//Check that the file actually exists. If it doesn't we'll display and error and
		//then exit the program under exit(EXIT_FAILURE).
	testFile = fopen(in_file, "r");
	if(testFile == NULL){
		fclose(testFile);
		
		printf("ERROR: FILE ERROR. CHECK INPUT FILE NAME AND PATH.\n");
		printf("\tEXITING PROGRAM UNDER exit(EXIT_FAILURE) CONDITIONS.\n\n");
		
		exit(EXIT_FAILURE);	
	}
	
		//The while loop we shall use grabs 1 character at a time. As such we'll use
			//fgetc(testfile). Since the file should only contain 1 or 0, we'll
			//use a boolean variable to hold their values.
	char test_value = 0;
	
	char prediction = 0;
	
		//To check for the end of the file we'll use an integer to ensure all works well.
	int check_value;
	while( !feof(testFile) ){
		check_value = fgetc(testFile);
		//printf("%c", check_value+1);
		//Not comes the long process.
		//First we need to set test_value to check_value.
		test_value = check_value - 48;
		if((check_value - 48) == -49)
			break;
		
		//Then we run the testing.
			//Since our Sum, Decision, and Training functions are all self-sufficienct and capable
			//or outputting exactly what we need for the next function, we'll just send these functions
			//directly into the other functions as part of their parameters.
		
			//The most important thing is to get the depth correctly.
			//First we want the Summation.
			//Then we want the Decision.
			//Then we want the Training.
			//Which means, we call the functions in reverse order:
		prediction = Decision(threshold, Sum_Weight(pList->msp), test_value, pList->msp);
		Perceptron_Training(prediction,test_value,pList->msp);
		
		//After the Training has run, its time to update our Perceptron's Shift Register.
		
		//First we make a new Bit.
		Bit *new_bit = NULL;
		
		new_bit = (Bit*)malloc(sizeof(Bit));
		
		//Then set the new_bit's bit_value to the current test_value.
		new_bit->bit_value = test_value;
		
		//Then we add the new_bit to our Perceptron's Shift Register.
		Shift_Add_Bit( pList->msp->shift_reg, new_bit );
		
		//This will continue, until we've completed the file.
		
	}
	
	//Once the file is completed, its time to call our output file Function.
	Write_Output( pList->msp );
	
	//After all data has been written to the Output File, it is time to delete our Perceptron:
	Delete_Perceptron( pList );
	
	//Once that is completed, its time to close the file, and return from this function.
	fclose(testFile);
	
	return;
						
}

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	void Write_Output(
		Perceptron *percep
	);
*/
void Write_Output( Perceptron *percep ){
	//First we'll open the file we need.
	FILE *outFile = NULL;
	
	outFile = fopen( percep->percep_data->out_file, "a");
	if(outFile == NULL){
		fclose(outFile);
		
		printf("ERROR: FILE ERROR. CHECK OUTPUT FILE NAME AND PATH.\n");
		printf("\tEXITING PROGRAM UNDER exit(EXIT_FAILURE) CONDITIONS.\n\n");
		
		exit(EXIT_FAILURE);	
	}
	
	//Print the information:
	fprintf(outFile, "Hit Count\t|\t%.0f\n", percep->percep_data->hit_count);
	fprintf(outFile, "Miss Count\t|\t%.0f\n", percep->percep_data->miss_count);
	fprintf(outFile, "Trained\t|\t%d\n", percep->percep_data->train_count);
	fprintf(outFile, "Accuracy\t|\t%.6f\n", percep->percep_data->hit_count / (percep->percep_data->hit_count + percep->percep_data->miss_count));
	fprintf(outFile, "--------------------\n\n");
	
	
	//Close the File:
	fclose(outFile);
	
	return;
}

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	void Test_From_Array(
		bool *array_pointer,
		unsigned char depth,
		unsigned short int list,
		signed char max_weight,
		signed char min_weight,
		signed char threshold,
		char *out_file
	);
*/
void Test_From_Array( int *array_pointer, int array_size, unsigned char depth, unsigned short int list, signed char max_weight, signed char min_weight, signed char threshold, char *out_file ){
	//First generate the Perceptron List;
	Percep_List *pList = NULL;
	
	pList = (Percep_List*)malloc(sizeof(Percep_List));
	
	pList = Per_List_init( depth );
	
	//Then generate a Perceptron until we have reached maximum List Size:
		//NOTE: For now the size should be 1.
	int i = 0;
	
	for(i = 0; i < list; i++){
		Perceptron *perceptron = NULL;
		
		perceptron = Perceptron_Init( depth, max_weight, min_weight, threshold, list );
		
		Set_Files( NULL, out_file, perceptron );
		
		Percep_Add_Percep( pList, perceptron );
	}
	
	//Once the List has been created with the Perceptron(s), we are then ready to begin testing.
	
	//To test:
		//We will loop through the entirety of the array to get the array size.
		//Then we will loop through the array again, this time doing the actual testing.
		//Each value in the array is the equivalent to a branch result.
		//We must loop through the file, one character at a time, until we have reached the end
		//	of the file.
		//For each 'bit' that we read from the file, we'll send it through the training system 
		//	of the Perceptron.
		//This means we'll run the Summation Function, send it into the Decision Function,
		//	and then Send that into the Training Function.
		
		//We then must generate a new Bit whose value is equal to the 'Bit' that we have
		//	read in from the file.
		//We will then add this bit to our Shift Register.
		
		//Once the entire file is completed. We'll then generate the output.
	
	int prediction = 0;
	
	for(i = 0; i < array_size; i++){
		
		prediction = Decision(threshold, Sum_Weight( pList->msp ), *(array_pointer + i), pList->msp );
		
		Perceptron_Training( prediction, *(array_pointer + i), pList->msp );
		
		//First we make a new Bit.
		Bit *new_bit = NULL;
		
		new_bit = (Bit*)malloc(sizeof(Bit));
		
		//Then set the new_bit's bit_value to the current test_value.
		new_bit->bit_value = *(array_pointer + i);
		
		//Then we add the new_bit to our Perceptron's Shift Register.
		Shift_Add_Bit( pList->msp->shift_reg, new_bit );
		
		//This will continue, until we've completed the array.
	}
	
		//Once the file is completed, its time to call our output file Function.
	Write_Output( pList->msp );
	
	//After all data has been written to the Output File, it is time to delete our Perceptron:
	Delete_Perceptron( pList );
	
	return;
}

//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
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
*/
void Daisy_Chain( int *array_pointer, int array_size, unsigned char depth, unsigned short int list, signed char max_weight, signed char min_weight, signed char threshold, char *in_file, char *out_file ){
	//The Daisy Chain function has two possibilities:
		//1) The user passes in only a File.
		//2) The user passes in an array.
		
	//If the user passes in NULL or 0 for the array_pointer or array_size, this function will default to running the perceptron via the file method.
		//If the user passes in something to both the array_pointer and the array_size, then this function will act based on array testing protocol.
	if(array_pointer == NULL || array_size == 0){
			//First generate the Perceptron List;
		Percep_List *pList = NULL;
		
		pList = (Percep_List*)malloc(sizeof(Percep_List));
		
		pList = Per_List_init( list );
		
		//Then generate a Perceptron until we have reached maximum List Size:
			//NOTE: For now the size should be 1.
		int i = 0;
		
		for(i = 0; i < list; i++){
			Perceptron *perceptron = NULL;
			
			perceptron = Perceptron_Init( depth, max_weight, min_weight, threshold, list );
			
			Set_Files( in_file, out_file, perceptron );
			
			Percep_Add_Percep( pList, perceptron );
		}
		
		//Once the List has been created with the Perceptron(s), we are then ready to begin testing.
	
		//To test:
			//We must first open the in_file for reading purposes.
			//We must loop through the file, one character at a time, until we have reached the end
			//of the file.
			//For each 'bit' that we read from the file, we'll send it through the training system 
			//of the Perceptron.
			//This means we'll run the Summation Function, send it into the Decision Function,
			//we will then send the result of the Decision Function as the 'actual' value for the
			//next perceptron in the chain. It will also be the value that gets placed into their
			//Shift Register.
			
			//This will continue until the entire chain has been touched upon. After that the
			//final Perceptron's Decision will determine if training is conducted or not.
			
			//If Training is conducted then the actual result will be passed to all Perceptrons.
			
			//We then must generate a new Bit whose value is equal to the 'Bit' that we have
			//read in from the file for the first Perceptron, but use the decision of all other perceptrons
			//as the new Bit for their Shift Registers.
			
			//Once the entire file is completed. We'll then generate the output.
			
		FILE *testFile;
		
		//Check that the file actually exists. If it doesn't we'll display and error and
			//then exit the program under exit(EXIT_FAILURE).
		testFile = fopen(in_file, "r");
		
		if(testFile == NULL){
			fclose(testFile);
			
			printf("ERROR: FILE ERROR. CHECK INPUT FILE NAME AND PATH.\n");
			printf("\tEXITING PROGRAM UNDER exit(EXIT_FAILURE) CONDITIONS.\n\n");
			
			exit(EXIT_FAILURE);	
		}
		
			//The while loop we shall use grabs 1 character at a time. As such we'll use
				//fgetc(testfile). Since the file should only contain 1 or 0, we'll
				//use a boolean variable to hold their values.
		char test_value = 0;
		
		char prediction = 0;
		
			//To check for the end of the file we'll use an integer to ensure all works well.
		int check_value;
		
			//We need a Perceptron pointer to move ourselves down the Perceptron List:
		Perceptron *percep_pointer = NULL;
		
		while( !feof(testFile) ){
			percep_pointer = pList->msp;
			check_value = fgetc(testFile);
			//Not comes the long process.
			//First we need to set test_value to check_value.
			test_value = check_value - 48;
			
			if(check_value - 48 < 0)
				break;
			
			//We'll calculate the first prediction outside the other perceptron's loops:
			if(percep_pointer != NULL){
				prediction = Decision(threshold, Sum_Weight(percep_pointer), test_value, percep_pointer);
				percep_pointer = percep_pointer->next_percep;
			}
			
			//We Must loop through, sending the decision results of one Perceptron into the next, until
				//we get to the last Perceptron:
			while(percep_pointer != NULL){
				//But first, we must generate the new bit and add it to the perceptron's shift register:
				Bit *new_bit = NULL;
				
				new_bit = (Bit*)malloc(sizeof(Bit));
				
				new_bit->bit_value = prediction;
				
				Shift_Add_Bit( percep_pointer->shift_reg, new_bit );
				
				prediction = Decision(threshold, Sum_Weight(percep_pointer), prediction, percep_pointer);
				
				percep_pointer = percep_pointer->next_percep;
			}
			
			
			//Once all other Perceptrons, except the last, is completed its time to see what it predicts:
			//prediction = Decision(threshold, Sum_Weight(percep_pointer), prediction, percep_pointer);
			
			//Now that we have the final prediction, it is time to reloop through all the perceptrons so that
				//They can be trained.
			percep_pointer = pList->msp;
			
			while(percep_pointer != NULL){
				Perceptron_Training(prediction,test_value,percep_pointer);
				
				percep_pointer = percep_pointer->next_percep;
			}
			
			//Lastly we'll generate the Bit to be added to the first perceptron based on the actual result:
			Bit *new_bit = NULL;
			
			new_bit = (Bit*)malloc(sizeof(Bit));
			
			new_bit->bit_value = test_value;
			
			Shift_Add_Bit( pList->msp->shift_reg, new_bit );
			
			//This will continue until all is over.*/
		}
		
		//Once the file is completed, its time to call our output file Function.
		Write_Output( pList->lsp );
		
		//After all data has been written to the Output File, it is time to delete our Perceptron:
		Delete_Perceptron( pList );
		
		//Once that is completed, its time to close the file, and return from this function.
		fclose(testFile);
	}
	
	//Array Method:
	else{
		
		
	}
	
	
	
}
//-----------------------------------------------------------------------------//
//
//
//----------------------------------------------------------------------------//
/*
	Hash Perceptron
*/


struct Perceptron *Hash_Percep( int address, PList *list ){
	int location = addredd % list->size;
	
	int count = 0;
	
	Perceptron *percep_pointer = list->msp;
	
	while(percep_pointer != NULL && count != location){
		percep_pointer = percep_pointer->next_percep;
		count++;
	}
	
	return percep_pointer;
}
