/* This file is Copyright University of Edinburgh 2018. For license details, see LICENSE. */

#include "translate/llvm/LLVMTranslationEngine.h"
#include "util/ComponentManager.h"

#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/Support/TargetSelect.h>

using namespace archsim::translate;
using namespace archsim::translate::translate_llvm;

RegisterComponent(TranslationEngine, LLVMTranslationEngine, "llvm", "LLVM-based translation engine");

bool LLVMTranslationEngine::Initialise()
{
	::llvm::InitializeNativeTarget();
	::llvm::InitializeNativeTargetAsmParser();
	::llvm::InitializeNativeTargetAsmPrinter();

	return true;
}

void LLVMTranslationEngine::Destroy()
{
}

bool LLVMTranslationEngine::Translate(TranslationManager& mgr)
{
	return true;
}
