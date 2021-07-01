#define DEBUG_TYPE "danger"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
using namespace llvm;
namespace {
	struct Danger : public FunctionPass {
	//std::map<std::string, int> danger;
	static char ID;
	Danger() : FunctionPass(ID) {}
	virtual bool runOnFunction(Function &F) {
		errs() << "Function " << F.getName() << '\n';
		for (Function::iterator bb = F.begin(), e = F.end(); bb != e; ++bb) {
			for (BasicBlock::iterator i = bb->begin(), e = bb->end(); i != e; ++i) {
				//errs() << "Une instruction " << i->getOpcodeName()<< '\n';
				//BasicBlock::iterator j = i + 1;
				if(llvm::isa<llvm::GetElementPtrInst>(i) && llvm::isa<llvm::StoreInst>(i->getNextNode())){
					errs() << "Dereferencement en écriture trouvé !!!\n ";
				}
				/*if(llvm::isa<llvm::GetElementPtrInst>(i) && llvm::isa<llvm::LoadInst>(++i)){
					errs() << "Dereferencement en lecture trouvé !!!\n ";
				}*/
				/*if(danger.find(i->getOpcodeName()) == danger.end()) {
					danger[i->getOpcodeName()] = 1;
				} else {
					danger[i->getOpcodeName()] += 1;
				}*/

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
