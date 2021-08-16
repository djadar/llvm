//inspired by Dowser
#include <map>

//List of operation and their penalties
static std::map<std::string, int> operations;

//fill the list of operation
static void fill_operation(){
	//basic index aritmetic instruction like addition and subtraction : biai 
    operations["biai"]=5;
    operations["biai2"]=1;
    //other index aritmetic instruction like division, shift and xor

    //Different constant values

    //constants used to access fields of structures

    //numerical values determined outside the loop

    //Non-inlined functions returning non-pointer values

    //Data movement instructions

    //load a pointer calculates outside the loop

    //GetElemptr instruction
    operations["getelemptr"]=1;

    //pointer cast operations
    operations["cast"]=100;
    
}

Value* checkAliasPointer(Value * v);

void InstructionsInLoop(Loop *L, unsigned nesAng);

void print_danger();