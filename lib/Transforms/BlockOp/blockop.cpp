#define DEBUG_TYPE "opBlocker"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
//Manipulate loops
#include "llvm/Analysis/LoopInfo.h"

#include <vector>
using namespace std;
//
using namespace llvm;
namespace {
	struct BlockOp : public FunctionPass {
	std::map<std::string, int> opBlocker;
	static char ID;
	BlockOp() : FunctionPass(ID) {}

	//An LLVM pass must declare which other passes it requires to execute properly
	void getAnalysisUsage(AnalysisUsage &AU)const{
		AU.addRequired<LoopInfoWrapperPass>();
		AU.setPreservesAll();
	}

	void InstructionsInLoop(Loop *L, unsigned nesAng) {
		Loop::block_iterator bb;
		std::string s;
		for(bb = L->block_begin(); bb != L->block_end();++bb){
			for (BasicBlock::iterator i = (*bb)->begin(), e = (*bb)->end(); i != e; ++i) {
				s = i->getOpcodeName();
				if(llvm::isa<llvm::GetElementPtrInst>(i) && llvm::isa<llvm::StoreInst>(i->getNextNode())){
					s = "Dereferencement en écriture. ";

				}else if(llvm::isa<llvm::GetElementPtrInst>(i) && llvm::isa<llvm::LoadInst>(i->getNextNode())){
					s = "Dereferencement en lecture. ";
				}
				errs() << "L'opération sur pointeur est :" << s << "\n";
			}
   		}
	}

	virtual bool runOnFunction(Function &F) {
		//To recover the datastructures created by the pass, we can use the getAnalysis method
		LoopInfo &LI= getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
		
		errs() << "Function " << F.getName() + "\n";
		for (LoopInfo::iterator i = LI.begin(), e = LI.end(); i != e; ++i) {
			InstructionsInLoop(*i, 0);	
		}
		return false;
		}
	};
}

char BlockOp::ID = 0;
static RegisterPass<BlockOp> X("opBlocker", "Counts opcodes per functions");
