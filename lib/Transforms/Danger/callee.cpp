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

#include <vector>
#include <iostream>

void InstructionsInLoop(Loop *L, unsigned nesAng) {
		Loop::block_iterator bb;
		std::string s;
		for(bb = L->block_begin(); bb != L->block_end();++bb){
			for (BasicBlock::iterator i = (*bb)->begin(), e = (*bb)->end(); i != e; ++i) {
				s = i->getOpcodeName();
				//errs() << "Les opérandes de l'instruction "<<s<<" sont : "<< i->getNumOperands() << '\n';
				
				//getElementptr suivi d'un load ou d'un store
				/*if(llvm::isa<llvm::GetElementPtrInst>(i) && llvm::isa<llvm::StoreInst>(i->getNextNode())){
					s = "Dereferencement en écriture. ";
					errs() << "Les opérandes de l'instruction "<<s<<" sont : "<< i->getNumOperands() << '\n';
					if(llvm::isa<llvm::IntegerType>(i->getOperand(1)->getType()))
						errs() << "on a un entier"<<"\n";
				}*/
				/*else if(llvm::isa<llvm::GetElementPtrInst>(i) && llvm::isa<llvm::LoadInst>(i->getNextNode())){
					s = "Dereferencement en lecture. ";
				}*/
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
void fill_list(Function::iterator bb){
    for(BasicBlock::iterator i = bb->begin(), e = bb->end(); i != e; ++i) {
		if(llvm::isa<llvm::AllocaInst>(i)) {
			errs() << (i->getOperand(0))->getType() <<"\n";
		} 
	}
}

//Affiche les résultats
/*void print_danger(){
	
	/*std::map <std::string, int>::iterator i = danger.begin();
		std::map <std::string, int>::iterator e = danger.end();
		while (i != e) {
			errs() << i->first << ": " << i->second << "\n";
			i++;
		}
		errs() << "\n";
		danger.clear();
}*/