#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
using namespace std;
using namespace llvm;
#include "llvm/ADT/StringRef.h"
//Manipulate loops
#include "llvm/Analysis/LoopInfo.h"
#include "danger.h"
#include <vector>
#include <iostream>

//List of pointers and their penalties

	static std::map<Value *, int> danger;

	

	//verify if the pointer doesn't alias another already in the map and increment penalty
	Value* checkAliasPointer(Value * v){
		std::map <Value*, int>::iterator i = danger.begin();
		std::map <Value*, int>::iterator e = danger.end();
		
		errs()<<"RECHERDE D'ALIAS"<<'\n';
		while (i != e) {
			//errs() << i->first << " : " << i->second << "\n";
			errs()<<aliasAnalysis.alias(v,i->first)<<"\n";	
			if(aliasAnalysis.isMustAlias(v,i->first))
				errs()<<"okay\n";
			i++;
		}
		errs() << "\n";	
		
	}

	//Construct the final map pointer-penalty
	void fill_list(Value *pointer, int penality){
		//if the pointeris already in the list
		if (danger.find(pointer) != danger.end()){
			danger[pointer] = danger[pointer] + penality;
		}
		//if not
		else {
			//We verify if another pointer alisa him
			Value* V = checkAliasPointer(pointer));
			if (V)
				danger[V] = danger[V] + penality;
			else
				danger[pointer] = penality;
		}
			
	}

	void InstructionsInLoop(Loop *L, unsigned nesAng) {
			
			Loop::block_iterator bb;

			//Fill map operations
			fill_operation();

			//Identification of operation schemes
			for(bb = L->block_begin(); bb != L->block_end();++bb){
				for (BasicBlock::iterator i = (*bb)->begin(), e = (*bb)->end(); i != e; ++i) {
					
					errs()<<"L'instruction est : "<<i->getOpcodeName() <<" et a "<< i->getNumOperands()<< "opérandes"<<"\n";
					
					//getElementptr suivi d'un load ou d'un store - dereferencement en écriture et lecture
					if(GetElementPtrInst *inst = dyn_cast<GetElementPtrInst>(i)){
						if(llvm::isa<llvm::StoreInst>(i->getNextNode())){
							
							errs() << "dereferencement en écriture"<< '\n';

							//if(PointerType *pointerType=dyn_cast<PointerType>(i->getOperand(1)->getType()->getPointerElementType() ))
								fill_list(inst->getOperand(0), operations["biai"]);
								checkAliasPointer()
						}
						else if(llvm::isa<llvm::LoadInst>(i->getNextNode())){
							
							errs() << "dereferencement en lecture"<< '\n';
							fill_list(inst->getOperand(0), operations["biai"]);
						}
					}
					//2 load suivi d'un store
					/*if(llvm::isa<llvm::LoadInst>(i) && llvm::isa<llvm::LoadInst>(i->getNextNode())){
						
						if(i->getValueName() == (i->getNextNode())->getOperand(0)->getValueName()){
							//errs() << "good" << i->getValueName() << '\n';
							if((i->getNextNode())->getValueName() == ((i->getNextNode())->getNextNode())->getOperand(0)->getValueName()){
								//errs() << "good2" << (i->getNextNode())->getValueName() << '\n';
							}
						}
					}*/
					//errs() << "L'opération sur pointeur est :" << s << "\n";
				}
			}
		}


	//Affiche les résultats
	void print_danger(){
		
		std::map <Value*, int>::iterator i = danger.begin();
		std::map <Value*, int>::iterator e = danger.end();
		
		errs()<<"OPERATION"<<'\n';
		while (i != e) {
			errs() << i->first << " : " << i->second << "\n";
			i++;
		}
		errs() << "\n";
		danger.clear();
	}
