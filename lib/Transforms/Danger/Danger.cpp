#include "llvm-c/Core.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include <cstdint>
#define DEBUG_TYPE "danger"

//#include "callee.cpp"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
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
#include "llvm-c/Core.h"
#include "danger.h"

namespace {

	struct Danger : public ModulePass{
		//list pointer-penalty : string, tableau[2] : pointer penalty size
		
		static char ID;
		Danger() : ModulePass(ID) {}

		
		//An LLVM pass must declare which other passes it requires to execute properly
		void getAnalysisUsage(AnalysisUsage &AU)const override{
			//AAResultsWrapperPass::getAnalysisUsage(AU);
			AU.addRequired<AAResultsWrapperPass>();
			AU.addRequired<LoopInfoWrapperPass>();
			AU.setPreservesAll();
		}

		//create a secure Malloc function
		// This creates a prototype of the function, so that LLVM knows about it
		// when we are hooking the function into a .ll file.
		//
		// Because we instrument a .ll file before attaching our actually instrumentation files (a separate .ll)
		// we need to have the function signature ready to go. (One problem is LLVM creates arguments lazily, so we
		// would not know about them, but by doing this we avoid that problem.)
		//
		void setupHooks(Module& M){
			auto &Context = M.getContext();

			Type* voidTy = Type::getVoidTy(Context);
			Type* intTy = Type::getInt32Ty(Context);
			vector<Type *>  args;
			args.push_back(intTy);
			args.push_back(intTy);
			// Specify the return value, arguments, and if there are variable numbers of arguments.
			FunctionType* funcTy = FunctionType::get(voidTy, args, false);
			Function::Create(funcTy, llvm::GlobalValue::ExternalLinkage)->setName(SECURE_MALLOC);
		}

		void InstrumentEnterFunction(llvm::CallBase *CB, Module& M){ 
			// Create the actual function
			// If we have a function already, then the below is very useful
			//
			// FunctionType* funcTy = M.getFunction(InstrumentingFunctionName)->getFunctionType();
			//
			// However, we are hooking into a function that we will merge later, so we instead build our function type
			// Both methods will allow us to then modify the function arguments.
			//
			// Build out the function type
			auto &Context = M.getContext();
			// The functions return type
			Type* voidTy = Type::getVoidTy(Context);
			// The start of our parameters
			Type* intTy = Type::getInt32Ty(Context);
			// push back all of the parameters
			std::vector<llvm::Type*> params;
			params.push_back(intTy);
			params.push_back(intTy);
			// Specify the return value, arguments, and if there are variable numbers of arguments.
			FunctionType* funcTy = FunctionType::get(voidTy, params, false);
			// Create a Constant that grabs our function
			M.getOrInsertFunction(SECURE_MALLOC, funcTy);
			
			/*// We determine where we want to add our instrumentation.
			// In this instance, we want to instrument the first basic block, and
			// put the instruction at the front. Every function has at least an entry:
			// block in the LLVM IR, so this should be valid.
			BasicBlock *BB = &FunctionToInstrument.front();
			Instruction *I = &BB->front();
			
			// In order to set the arguments of the instrumenting function, we are going to
			// get all of our instrumenting functions arguments, and then modify them.
			std::vector<Value*> args;
			for(unsigned int i=0; i< funcTy->getNumParams(); ++i){
				Type* t = funcTy->getParamType(i);
				// We get the argument, and then we can re-assign its value
				// In this case, we are looking at our obController to see the function name in the hashmap, and then its value
				//
				// TODO: For now I know this is a constant, but perhaps this could change in the future. 
				llvm::Value* foo = 0;
				//Value *newValue = dyn_cast<llvm::ConstantInt>(foo);
				Value *newValue = ConstantInt::get(t,0x1234);
				args.push_back(newValue);
				errs() << "getNumParams()" << i << "\n";
			}

			// Create our function call
			CallInst::Create(hook,args)->insertBefore(I);*/
			//
			
			const static llvm::StringRef TargetFunName = "malloc";
			// Only find direct function calls.
			//errs()<<CB->getCalledFunction()->getName().str()<<" ok \n";
			if (!CB->isIndirectCall() && CB->getCalledFunction() &&
				CB->getCalledFunction()->getName().str() ==
					TargetFunName) {
				errs() << "found a direct call to '" << TargetFunName
							<< "'!\n";

				std::map <Value*, int>::iterator i = danger.begin();
				std::map <Value*, int>::iterator e = danger.end();
				
				
				while (i != e) {
					//errs() << i->first << " : " << i->second << "\n";
					Value* v1 = (Value *)i->first;
						
					if(CB->hasArgument(v1)){
						errs()<<"******************"<<'\n';
						errs()<<"Argument IDENTIFIE"<<'\n';
						//remplacons les malloc(taille) par des secure_malloc(taille,penalty)

						//1429 1472 1503
						//CB->setCalledFunction(fn);
						errs()<<"--Okay--"<<'\n';
						//Attribute pn = 
						vector<Value *>  args;
						args.push_back(CB->getOperand(0));
						Type *voidTy = Type::getInt32Ty(Context);
						Value *newValue = ConstantInt::get(intTy,i->second);
						args.push_back(newValue);
						//CB->addParamAttr(2,args);
						//CallInst::Create(funcTy,voidTy,args,"",CB);
						//CB->setCalledFunction(funcTy);
						
						errs()<<"--Okay--"<<'\n';
						errs()<<"******************"<<'\n';
					}
					i++;
				} 
				
			}

		}

		/*void CreateSecureMalloc(llvm::CallBase *CB,Module &M){
			//
			Type *voidTy = Type::getVoidTy(M.getContext()); 
			Type *int64Ty = Type::getInt64Ty(M.getContext());
			Type *int32Ty = Type::getInt32Ty(M.getContext());
			llvm::FunctionCallee securemalloc = M.getOrInsertFunction("secure_malloc", voidTy,int64Ty,int32Ty);
				
		}*/
		
		virtual bool runOnModule(Module &M) override{
			

			//To recover the datastructures created by the pass, we can use the getAnalysis method
			//for mnipulating loops
			LoopInfo &LI= getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
			//For checking alis analysis
			AliasAnalysis &aliasAnalysis = getAnalysis<AAResultsWrapperPass>().getAAResults();

			setupHooks(M);

			for (auto &F : M){
				errs() << "Function " << F.getName() + "\n";
				//AliasAnalysis::InitializeAliasAnalysis(this);
		
				//We visit loops searching for instructions dereferencing pointers
				//we assign and increment penalties of operations on those pointers

				unsigned num = 0;
				for (LoopInfo::iterator i = LI.begin(), e = LI.end(); i != e; ++i) {
					errs() << "Loop " << num <<"\n";
					InstructionsInLoop(*i, aliasAnalysis );
					num++;	
				}
				
				for (auto &BB : F) {
						for (auto &I : BB) {
							if (auto *CB = llvm::dyn_cast<llvm::CallBase>(&I)) {
								//inspectMalloc(CB,M);
								InstrumentEnterFunction(CB,M);
								}
						}
					}
				//
				//Printing danger list
				print_danger();
			}
			
			
			return true;
		}
	};
} // namespace

char Danger::ID = 0;
static RegisterPass<Danger> X("Danger", "Get vulnerable buffers per functions");
