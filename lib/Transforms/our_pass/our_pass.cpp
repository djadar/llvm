#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace { 
    struct OurPass : FunctionPass{
        static char ID;
        OurPass() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override {
            errs() << "OurPass: ";
            errs().write_escaped(F.getName()) << '\n';
            return false;
        }
    };// End of OurPass
} //namespace end

char OurPass::ID = 0;

static RegisterPass<OurPass> X("OurPass", "A simple Hello World pass",false,false);

static RegisterStandardPasses Y(
    PassManagerBuilder::EP_EarlyAsPossible,
    [](const PassManagerBuilder &Builder,
       legacy::PassManagerBase &PM) { PM.add(new OurPass()); });