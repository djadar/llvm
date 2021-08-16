#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
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

	struct Memoire : public FunctionPass{
		//list pointer-penalty : string, tableau[2]
		
		static char ID;
		Memoire() : FunctionPass(ID) {}

		
		//An LLVM pass must declare which other passes it requires to execute properly
		void getAnalysisUsage(AnalysisUsage &AU)const{
			//AAResultsWrapperPass::getAnalysisUsage(AU);
			//AliasAnalysis::AnalysisUsage(AU);
			AU.addRequired<LoopInfoWrapperPass>();
			AU.addRequired<AAResultsWrapperPass>();
			AU.setPreservesAll();
		}

	void InstructionsInLoop(Loop *L, unsigned nesAng) {
			
			Loop::block_iterator bb;
			for(bb = L->block_begin(); bb != L->block_end();++bb){
				for (BasicBlock::iterator i = (*bb)->begin(), e = (*bb)->end(); i != e; ++i) {
					
					errs()<<"L'instruction est : "<<i->getOpcodeName() <<" et a "<< i->getNumOperands()<< " opérandes "<<"\n";

					if(GetElementPtrInst *allocaInst = dyn_cast<GetElementPtrInst>(i)){

						if(PointerType *pointerType=dyn_cast<PointerType>(allocaInst->getOperand(0)->getType())){
							errs()<<"OKAY0 \n";
						}
						if(IntegerType *pointerType=dyn_cast<IntegerType>(allocaInst->getOperand(1)->getType())){
							errs()<<"OKAY1 \n";
						}
						/*if(PointerType *pointerType=dyn_cast<PointerType>(allocaInst->getOperand(2)->getType())){
							errs()<<"OKAY2 \n";
						}*/
					}	
				}
			}
		}
	
		virtual bool runOnFunction(Function &F) {
			

			//To recover the datastructures created by the pass, we can use the getAnalysis method
			LoopInfo &LI= getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
			AliasAnalysis &aliasAnalysis = getAnalysis<AAResultsWrapperPass>().getAAResults();
			
			errs() << "Function " << F.getName() + "\n";
			//AliasAnalysis::InitializeAliasAnalysis(this);
	
			
			/*for(Function::iterator bb = F.begin(), e = F.end(); bb != e; ++bb) {
				Value * v1;
				Value * v2;
				//fill_list(bb);
				for(BasicBlock::iterator i = bb->begin(), e = bb->end(); i != e; ++i) {
					errs()<<"on teste \n";
					if(StoreInst *storeInst = dyn_cast<StoreInst>(i))
						v1 = i->getOperand(1);
					
					if(LoadInst *loadInst = dyn_cast<LoadInst>(i))
						v2 = i->getOperand(0);
												
				}	
				errs()<<"on teste "<<aliasAnalysis.alias(v1,v2)<<"\n";		
			}*/
			unsigned num = 0;
			for (LoopInfo::iterator i = LI.begin(), e = LI.end(); i != e; ++i) {
				errs() << "Loop " << num <<"\n";
				InstructionsInLoop(*i, 0);
				num++;	
			}

			return false;
		}
	};
}

char Memoire::ID = 0;
static RegisterPass<Memoire> X("memory", "Get vulnerable buffers per functions");