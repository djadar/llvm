add_llvm_library( LLVMOurPass MODULE 
  our_pass.cpp

  
  PLUGIN_TOOL
  opt
  )