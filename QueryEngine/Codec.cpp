#include "Codec.h"
#include "LLVMGlobalContext.h"

#include <glog/logging.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>

FixedWidthInt::FixedWidthInt(const size_t byte_width) : byte_width_{byte_width} {}

llvm::Instruction* FixedWidthInt::codegenDecode(llvm::Value* byte_stream,
                                                llvm::Value* pos,
                                                llvm::Module* module) const {
  auto& context = getGlobalLLVMContext();
  auto f = module->getFunction("fixed_width_int_decode");
  CHECK(f);
  llvm::Value* args[] = {byte_stream, llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), byte_width_), pos};
  return llvm::CallInst::Create(f, args);
}

FixedWidthUnsigned::FixedWidthUnsigned(const size_t byte_width) : byte_width_{byte_width} {}

llvm::Instruction* FixedWidthUnsigned::codegenDecode(llvm::Value* byte_stream,
                                                     llvm::Value* pos,
                                                     llvm::Module* module) const {
  auto& context = getGlobalLLVMContext();
  auto f = module->getFunction("fixed_width_unsigned_decode");
  CHECK(f);
  llvm::Value* args[] = {byte_stream, llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), byte_width_), pos};
  return llvm::CallInst::Create(f, args);
}

DiffFixedWidthInt::DiffFixedWidthInt(const size_t byte_width, const int64_t baseline)
    : byte_width_{byte_width}, baseline_{baseline} {}

llvm::Instruction* DiffFixedWidthInt::codegenDecode(llvm::Value* byte_stream,
                                                    llvm::Value* pos,
                                                    llvm::Module* module) const {
  auto& context = getGlobalLLVMContext();
  auto f = module->getFunction("diff_fixed_width_int_decode");
  CHECK(f);
  llvm::Value* args[] = {byte_stream,
                         llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), byte_width_),
                         llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), baseline_),
                         pos};
  return llvm::CallInst::Create(f, args);
}

FixedWidthReal::FixedWidthReal(const bool is_double) : is_double_(is_double) {}

llvm::Instruction* FixedWidthReal::codegenDecode(llvm::Value* byte_stream,
                                                 llvm::Value* pos,
                                                 llvm::Module* module) const {
  auto f = module->getFunction(is_double_ ? "fixed_width_double_decode" : "fixed_width_float_decode");
  CHECK(f);
  llvm::Value* args[] = {byte_stream, pos};
  return llvm::CallInst::Create(f, args);
}
