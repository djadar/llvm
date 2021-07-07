#define DEBUG_TYPE "danger"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
using namespace std;
using namespace llvm;


namespace {

	struct Danger : public FunctionPass {
	//std::map<std::string, int> danger;
	static char ID;
	Danger() : FunctionPass(ID) {}

	/*std::string identification(BasicBlock::iterator *i){
		std::string s;
		if(llvm::isa<llvm::GetElementPtrInst>(*i) && llvm::isa<llvm::StoreInst>((*i)->getNextNode())){
					s = "Dereferencement en écriture. ";

				}else if(llvm::isa<llvm::GetElementPtrInst>(*i) && llvm::isa<llvm::LoadInst>(++(*i))){
					s = "Dereferencement en lecture. ";

				}
	}*/

	virtual bool runOnFunction(Function &F) {
		errs() << "Function " << F.getName() << '\n';
		for (Function::iterator bb = F.begin(), e = F.end(); bb != e; ++bb) {
			std::string s;
			for (BasicBlock::iterator i = bb->begin(), e = bb->end(); i != e; ++i) {
				
				s = i->getOpcodeName();
				if(llvm::isa<llvm::GetElementPtrInst>(i) && llvm::isa<llvm::StoreInst>(i->getNextNode())){
					s = "Dereferencement en écriture. ";

				}else if(llvm::isa<llvm::GetElementPtrInst>(i) && llvm::isa<llvm::LoadInst>(i->getNextNode())){
					s = "Dereferencement en lecture. ";

				}
				
				errs() << "L'opération sur pointeur est :" << s << "\n";
				


			}
		}
		/*std::map <std::string, int>::iterator i = danger.begin();
		std::map <std::string, int>::iterator e = danger.end();
		while (i != e) {
			errs() << i->first << ": " << i->second << "\n";
			i++;
		}
		errs() << "\n";
		danger.clear();*/
		return false;
		}
	};
}

char Danger::ID = 0;
static RegisterPass<Danger> X("Danger", "Counts opcodes per functions");
