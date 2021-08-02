#define DEBUG_TYPE "danger"

//#include "callee.cpp"
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
#include "llvm/IR/Type.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/DerivedTypes.h"

#include <vector>
#include <iostream>


 #include "llvm/ADT/ArrayRef.h"
 #include "llvm/ADT/STLExtras.h"
 #include "llvm/ADT/StringRef.h"
 #include "llvm/IR/Type.h"
 #include "llvm/Support/Casting.h"
 #include "llvm/Support/Compiler.h"
 #include "llvm/Support/TypeSize.h"
 #include <cassert>
 #include <cstdint>

namespace {

	struct Danger : public FunctionPass {
	//list pointer-penalty : string, tableau[2]
	std::map<std::string, int> danger;
	static char ID;
	Danger() : FunctionPass(ID) {}

	//An LLVM pass must declare which other passes it requires to execute properly
	void getAnalysisUsage(AnalysisUsage &AU)const{
		AU.addRequired<LoopInfoWrapperPass>();
		AU.setPreservesAll();
	}

	virtual bool runOnFunction(Function &F) {
		
		//To recover the datastructures created by the pass, we can use the getAnalysis method
		LoopInfo &LI= getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
		
		errs() << "Function " << F.getName() + "\n";

		//On obtient les pointeurs initialisés
		for(Function::iterator bb = F.begin(), e = F.end(); bb != e; ++bb) {
			
			//fill_list(bb);
			for(BasicBlock::iterator i = bb->begin(), e = bb->end(); i != e; ++i) {
				/*if(llvm::isa<llvm::AllocaInst>(i)) {
					errs() << i->getOperand(0)->getPointerOperand() <<"\n";
				}*/
				errs()<<i->getOpcodeName()<<"\n";
				
				if(LoadInst *allocaInst = dyn_cast<LoadInst>(i)){
					//StringRef st = i->getOperand(0)->getName();
					//errs() << "ID = " << st << '\n';
				
				if(PointerType *pointerType=dyn_cast<PointerType>(allocaInst->getType()->getArrayElementType())){
						errs() << "1 un pointeur"<< allocaInst->getOperand(0)->getType()->getArrayElementType() <<"\n";
						errs() << "ID = " << allocaInst->getOperand(0)->getName() << '\n';
					}
						
				}
				if(llvm::isa<llvm::LoadInst>(i) && llvm::isa<llvm::LoadInst>(i->getNextNode())){
					
					if(i->getValueName() == (i->getNextNode())->getOperand(0)->getValueName()){
						errs() << "good" << i->getValueName() << '\n';
						if((i->getNextNode())->getValueName() == ((i->getNextNode())->getNextNode())->getOperand(0)->getValueName()){
							errs() << "good2" << (i->getNextNode())->getValueName() << '\n';
						}
					}
				}
				
			}
					
		}
	
		//On parcourt les boucles à la recherche d'instructions de déreferencement de pointeur
		//on attribue et incrémente les poids des opérations du les pointeurs
		
		unsigned num = 0;
		for (LoopInfo::iterator i = LI.begin(), e = LI.end(); i != e; ++i) {
			errs() << "Loop " << num <<"\n";
			//InstructionsInLoop(*i, 0);
			num++;	
		}
		
		//On affiche la liste danger
		//print_danger();

		return false;
		}
	};
}

char Danger::ID = 0;
static RegisterPass<Danger> X("Danger", "Get vulnerable buffers per functions");
