#define DEBUG_TYPE "opCounter"
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
	struct CountOp : public FunctionPass {
	std::map<std::string, int> opCounter;
	static char ID;
	CountOp() : FunctionPass(ID) {}

	//An LLVM pass must declare which other passes it requires to execute properly
	void getAnalysisUsage(AnalysisUsage &AU)const{
		AU.addRequired<LoopInfoWrapperPass>();
		AU.setPreservesAll();
	}

	void countBlocksInLoop(Loop *L, unsigned nesAng) {
		unsigned numBlocks = 0;
		Loop::block_iterator bb;
		for(bb = L->block_begin(); bb != L->block_end();++bb)
			numBlocks++;
		errs() << "Loop level " << nesAng << " has " << numBlocks << " blocks\n";
		vector<Loop*> subLoops = L->getSubLoops();
		Loop::iterator j, f;
		for (j = subLoops.begin(), f = subLoops.end(); j != f; ++j)
			countBlocksInLoop(*j, nesAng + 1);
	}

	virtual bool runOnFunction(Function &F) {
		//To recover the datastructures created by the pass, we can use the getAnalysis method
		LoopInfo &LI= getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
		int loopCounter = 0;
		//
		errs() << F.getName() << '\n';
		for (LoopInfo::iterator i = LI.begin(), e = LI.end(); i != e; ++i) {
			Loop *L = *i;
			errs() << "Function " << F.getName() + "\n";
			for (LoopInfo::iterator i = LI.begin(), e = LI.end(); i != e; ++i)
				countBlocksInLoop(*i, 0);			
		}
		
		return false;
		}
	};
}

char CountOp::ID = 0;
static RegisterPass<CountOp> X("opCounter", "Counts opcodes per functions");