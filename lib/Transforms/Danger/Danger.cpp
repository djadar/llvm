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

#include "danger.h"

namespace {

	struct Danger : public FunctionPass{
		//list pointer-penalty : string, tableau[2]
		
		static char ID;
		Danger() : FunctionPass(ID) {}

		
		//An LLVM pass must declare which other passes it requires to execute properly
		void getAnalysisUsage(AnalysisUsage &AU)const{
			//AAResultsWrapperPass::getAnalysisUsage(AU);
			//AliasAnalysis::AnalysisUsage(AU);
			AU.addRequired<LoopInfoWrapperPass>();
			//AU.addRequired<AliasAnalysis>();
			AU.setPreservesAll();
		}

		/*void *getAdjustedAnalysispointer(const void* ID)override{
			if (ID==&Danger::ID)
				return (AliasAnalysis*)this;
			return this;
		}*/

		virtual bool runOnFunction(Function &F) {
			

			//To recover the datastructures created by the pass, we can use the getAnalysis method
			//for mnipulating loops
			LoopInfo &LI= getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
			//For checking alis analysis
			AliasAnalysis &aliasAnalysis = getAnalysis<AAResultsWrapperPass>().getAAResults();

			errs() << "Function " << F.getName() + "\n";
			//AliasAnalysis::InitializeAliasAnalysis(this);
	
			
			//On parcourt les boucles à la recherche d'instructions de déreferencement de pointeur
			//on attribue et incrémente les poids des opérations du les pointeurs
			
			unsigned num = 0;
			for (LoopInfo::iterator i = LI.begin(), e = LI.end(); i != e; ++i) {
				errs() << "Loop " << num <<"\n";
				InstructionsInLoop(*i, 0);
				num++;	
			}
			
			//On affiche la liste danger
			print_danger();
			
			return false;
		}
	};
}

char Danger::ID = 0;
static RegisterPass<Danger> X("Danger", "Get vulnerable buffers per functions");
