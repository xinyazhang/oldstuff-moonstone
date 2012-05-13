// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// http://code.google.com/p/protobuf/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: kenton@google.com (Kenton Varda)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.

#include <google/protobuf/compiler/cpp/cpp_string_field.h>
#include <google/protobuf/compiler/cpp/cpp_helpers.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/stubs/strutil.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace cpp {

namespace {

void SetUnistrVariables(const FieldDescriptor* descriptor,
                        map<string, string>* variables) {
  SetCommonFieldVariables(descriptor, variables);
  (*variables)["default"] = DefaultValue(descriptor);
  (*variables)["default_variable"] = "::google::protobuf::internal::kEmptyUnistr"
  (*variables)["pointer_type"] = unistr::basic_type_char;
}

}  // namespace

// ===================================================================

UnistrFieldGenerator::
UnistrFieldGenerator(const FieldDescriptor* descriptor)
  : descriptor_(descriptor) {
  SetUnistrVariables(descriptor, &variables_);
}

UnistrFieldGenerator::~UnistrFieldGenerator() {}

void UnistrFieldGenerator::
GeneratePrivateMembers(io::Printer* printer) const {
  printer->Print(variables_, "::unistr* $name$_;\n");
#if 0
  if (!descriptor_->default_value_unistr().empty()) {
    printer->Print(variables_, "static const ::unistr $default_variable$;\n");
  }
#endif
}

void UnistrFieldGenerator::
GenerateAccessorDeclarations(io::Printer* printer) const {
  // If we're using UnistrFieldGenerator for a field with a ctype, it's
  // because that ctype isn't actually implemented.  In particular, this is
  // true of ctype=CORD and ctype=STRING_PIECE in the open source release.
  // We aren't releasing Cord because it has too many Google-specific
  // dependencies and we aren't releasing UnistrPiece because it's hardly
  // useful outside of Google and because it would get confusing to have
  // multiple instances of the UnistrPiece class in different libraries (PCRE
  // already includes it for their C++ bindings, which came from Google).
  //
  // In any case, we make all the accessors private while still actually
  // using a string to represent the field internally.  This way, we can
  // guarantee that if we do ever implement the ctype, it won't break any
  // existing users who might be -- for whatever reason -- already using .proto
  // files that applied the ctype.  The field can still be accessed via the
  // reflection interface since the reflection interface is independent of
  // the string's underlying representation.
  if (descriptor_->options().ctype() != FieldOptions::STRING) {
    printer->Outdent();
    printer->Print(
      " private:\n"
      "  // Hidden due to unknown ctype option.\n");
    printer->Indent();
  }

  printer->Print(variables_,
    "inline const ::unistr& $name$() const$deprecation$;\n"
    "inline void set_$name$(const ::unistr& value)$deprecation$;\n"
    "inline void set_$name$(const char* value)$deprecation$;\n"
    "inline void set_$name$(const $pointer_type$* value, size_t size)"
                 "$deprecation$;\n"
    "inline ::unistr* mutable_$name$()$deprecation$;\n"
    "inline ::unistr* release_$name$()$deprecation$;\n");

  if (descriptor_->options().ctype() != FieldOptions::STRING) {
    printer->Outdent();
    printer->Print(" public:\n");
    printer->Indent();
  }
}

void UnistrFieldGenerator::
GenerateInlineAccessorDefinitions(io::Printer* printer) const {
  printer->Print(variables_,
    "inline const ::unistr& $classname$::$name$() const {\n"
    "  return *$name$_;\n"
    "}\n"
    "inline void $classname$::set_$name$(const ::unistr& value) {\n"
    "  set_has_$name$();\n"
    "  if ($name$_ == &$default_variable$) {\n"
    "    $name$_ = new ::unistr;\n"
    "  }\n"
    "  $name$_->assign(value);\n"
    "}\n"
    "inline void $classname$::set_$name$(const char* value) {\n"
    "  set_has_$name$();\n"
    "  if ($name$_ == &$default_variable$) {\n"
    "    $name$_ = new ::unistr;\n"
    "  }\n"
    "  $name$_->assign(value);\n"
    "}\n"
    "inline "
    "void $classname$::set_$name$(const $pointer_type$* value, size_t size) {\n"
    "  set_has_$name$();\n"
    "  if ($name$_ == &$default_variable$) {\n"
    "    $name$_ = new ::unistr;\n"
    "  }\n"
    "  $name$_->assign(reinterpret_cast<const char*>(value), size);\n"
    "}\n"
    "inline ::unistr* $classname$::mutable_$name$() {\n"
    "  set_has_$name$();\n"
    "  if ($name$_ == &$default_variable$) {\n");
  // Don't support unistr default value now.
#if 0
  if (descriptor_->default_value_unistr().empty()) {
#endif
    printer->Print(variables_,
      "    $name$_ = new ::unistr;\n");
#if 0
  } else {
    printer->Print(variables_,
      "    $name$_ = new ::unistr($default_variable$);\n");
  }
#endif
  printer->Print(variables_,
    "  }\n"
    "  return $name$_;\n"
    "}\n"
    "inline ::unistr* $classname$::release_$name$() {\n"
    "  clear_has_$name$();\n"
    "  if ($name$_ == &$default_variable$) {\n"
    "    return NULL;\n"
    "  } else {\n"
    "    ::unistr* temp = $name$_;\n"
    "    $name$_ = const_cast< ::unistr*>(&$default_variable$);\n"
    "    return temp;\n"
    "  }\n"
    "}\n");
}

void UnistrFieldGenerator::
GenerateNonInlineAccessorDefinitions(io::Printer* printer) const {
#if 0
  if (!descriptor_->default_value_unistr().empty()) {
    printer->Print(variables_,
      "const ::unistr $classname$::$default_variable$($default$);\n");
  }
#endif
}

void UnistrFieldGenerator::
GenerateClearingCode(io::Printer* printer) const {
#if 0
  if (descriptor_->default_value_string().empty()) {
#endif
    printer->Print(variables_,
      "if ($name$_ != &$default_variable$) {\n"
      "  $name$_->clear();\n"
      "}\n");
#if 0
  } else {
    printer->Print(variables_,
      "if ($name$_ != &$default_variable$) {\n"
      "  $name$_->assign($default_variable$);\n"
      "}\n");
  }
#endif
}

void UnistrFieldGenerator::
GenerateMergingCode(io::Printer* printer) const {
  printer->Print(variables_, "set_$name$(from.$name$());\n");
}

void UnistrFieldGenerator::
GenerateSwappingCode(io::Printer* printer) const {
  printer->Print(variables_, "std::swap($name$_, other->$name$_);\n");
}

void UnistrFieldGenerator::
GenerateConstructorCode(io::Printer* printer) const {
  printer->Print(variables_,
    "$name$_ = const_cast< ::unistr*>(&$default_variable$);\n");
}

void UnistrFieldGenerator::
GenerateDestructorCode(io::Printer* printer) const {
  printer->Print(variables_,
    "if ($name$_ != &$default_variable$) {\n"
    "  delete $name$_;\n"
    "}\n");
}

void UnistrFieldGenerator::
GenerateMergeFromCodedStream(io::Printer* printer) const {
  printer->Print(variables_,
    "DO_(::google::protobuf::internal::WireFormatLite::Read$declared_type$(\n"
    "      input, this->mutable_$name$()));\n");
  if (HasUtf8Verification(descriptor_->file()) &&
      descriptor_->type() == FieldDescriptor::TYPE_STRING) {
    printer->Print(variables_,
      "::google::protobuf::internal::WireFormat::VerifyUTF8Unistr(\n"
      "  this->$name$().data(), this->$name$().length(),\n"
      "  ::google::protobuf::internal::WireFormat::PARSE);\n");
  }
}

void UnistrFieldGenerator::
GenerateSerializeWithCachedSizes(io::Printer* printer) const {
  if (HasUtf8Verification(descriptor_->file()) &&
      descriptor_->type() == FieldDescriptor::TYPE_STRING) {
    printer->Print(variables_,
      "::google::protobuf::internal::WireFormat::VerifyUTF8Unistr(\n"
      "  this->$name$().data(), this->$name$().length(),\n"
      "  ::google::protobuf::internal::WireFormat::SERIALIZE);\n");
  }
  printer->Print(variables_,
    "::google::protobuf::internal::WireFormatLite::Write$declared_type$(\n"
    "  $number$, this->$name$(), output);\n");
}

void UnistrFieldGenerator::
GenerateSerializeWithCachedSizesToArray(io::Printer* printer) const {
  if (HasUtf8Verification(descriptor_->file()) &&
      descriptor_->type() == FieldDescriptor::TYPE_STRING) {
    printer->Print(variables_,
      "::google::protobuf::internal::WireFormat::VerifyUTF8Unistr(\n"
      "  this->$name$().data(), this->$name$().length(),\n"
      "  ::google::protobuf::internal::WireFormat::SERIALIZE);\n");
  }
  printer->Print(variables_,
    "target =\n"
    "  ::google::protobuf::internal::WireFormatLite::Write$declared_type$ToArray(\n"
    "    $number$, this->$name$(), target);\n");
}

void UnistrFieldGenerator::
GenerateByteSize(io::Printer* printer) const {
  printer->Print(variables_,
    "total_size += $tag_size$ +\n"
    "  ::google::protobuf::internal::WireFormatLite::$declared_type$Size(\n"
    "    this->$name$());\n");
}

// ===================================================================

RepeatedUnistrFieldGenerator::
RepeatedUnistrFieldGenerator(const FieldDescriptor* descriptor)
  : descriptor_(descriptor) {
  SetUnistrVariables(descriptor, &variables_);
}

RepeatedUnistrFieldGenerator::~RepeatedUnistrFieldGenerator() {}

void RepeatedUnistrFieldGenerator::
GeneratePrivateMembers(io::Printer* printer) const {
  printer->Print(variables_,
    "::google::protobuf::RepeatedPtrField< ::unistr> $name$_;\n");
}

void RepeatedUnistrFieldGenerator::
GenerateAccessorDeclarations(io::Printer* printer) const {
  // See comment above about unknown ctypes.
  if (descriptor_->options().ctype() != FieldOptions::STRING) {
    printer->Outdent();
    printer->Print(
      " private:\n"
      "  // Hidden due to unknown ctype option.\n");
    printer->Indent();
  }

  printer->Print(variables_,
    "inline const ::unistr& $name$(int index) const$deprecation$;\n"
    "inline ::unistr* mutable_$name$(int index)$deprecation$;\n"
    "inline void set_$name$(int index, const ::unistr& value)$deprecation$;\n"
    "inline void set_$name$(int index, const char* value)$deprecation$;\n"
    "inline "
    "void set_$name$(int index, const $pointer_type$* value, size_t size)"
                 "$deprecation$;\n"
    "inline ::unistr* add_$name$()$deprecation$;\n"
    "inline void add_$name$(const ::unistr& value)$deprecation$;\n"
    "inline void add_$name$(const char* value)$deprecation$;\n"
    "inline void add_$name$(const $pointer_type$* value, size_t size)"
                 "$deprecation$;\n");

  printer->Print(variables_,
    "inline const ::google::protobuf::RepeatedPtrField< ::unistr>& $name$() const"
                 "$deprecation$;\n"
    "inline ::google::protobuf::RepeatedPtrField< ::unistr>* mutable_$name$()"
                 "$deprecation$;\n");

  if (descriptor_->options().ctype() != FieldOptions::STRING) {
    printer->Outdent();
    printer->Print(" public:\n");
    printer->Indent();
  }
}

void RepeatedUnistrFieldGenerator::
GenerateInlineAccessorDefinitions(io::Printer* printer) const {
  printer->Print(variables_,
    "inline const ::unistr& $classname$::$name$(int index) const {\n"
    "  return $name$_.Get(index);\n"
    "}\n"
    "inline ::unistr* $classname$::mutable_$name$(int index) {\n"
    "  return $name$_.Mutable(index);\n"
    "}\n"
    "inline void $classname$::set_$name$(int index, const ::unistr& value) {\n"
    "  $name$_.Mutable(index)->assign(value);\n"
    "}\n"
    "inline void $classname$::set_$name$(int index, const char* value) {\n"
    "  $name$_.Mutable(index)->assign(value);\n"
    "}\n"
    "inline void "
    "$classname$::set_$name$"
    "(int index, const $pointer_type$* value, size_t size) {\n"
    "  $name$_.Mutable(index)->assign(\n"
    "    reinterpret_cast<const char*>(value), size);\n"
    "}\n"
    "inline ::unistr* $classname$::add_$name$() {\n"
    "  return $name$_.Add();\n"
    "}\n"
    "inline void $classname$::add_$name$(const ::unistr& value) {\n"
    "  $name$_.Add()->assign(value);\n"
    "}\n"
    "inline void $classname$::add_$name$(const char* value) {\n"
    "  $name$_.Add()->assign(value);\n"
    "}\n"
    "inline void "
    "$classname$::add_$name$(const $pointer_type$* value, size_t size) {\n"
    "  $name$_.Add()->assign(reinterpret_cast<const char*>(value), size);\n"
    "}\n");
  printer->Print(variables_,
    "inline const ::google::protobuf::RepeatedPtrField< ::unistr>&\n"
    "$classname$::$name$() const {\n"
    "  return $name$_;\n"
    "}\n"
    "inline ::google::protobuf::RepeatedPtrField< ::unistr>*\n"
    "$classname$::mutable_$name$() {\n"
    "  return &$name$_;\n"
    "}\n");
}

void RepeatedUnistrFieldGenerator::
GenerateClearingCode(io::Printer* printer) const {
  printer->Print(variables_, "$name$_.Clear();\n");
}

void RepeatedUnistrFieldGenerator::
GenerateMergingCode(io::Printer* printer) const {
  printer->Print(variables_, "$name$_.MergeFrom(from.$name$_);\n");
}

void RepeatedUnistrFieldGenerator::
GenerateSwappingCode(io::Printer* printer) const {
  printer->Print(variables_, "$name$_.Swap(&other->$name$_);\n");
}

void RepeatedUnistrFieldGenerator::
GenerateConstructorCode(io::Printer* printer) const {
  // Not needed for repeated fields.
}

void RepeatedUnistrFieldGenerator::
GenerateMergeFromCodedStream(io::Printer* printer) const {
  printer->Print(variables_,
    "DO_(::google::protobuf::internal::WireFormatLite::Read$declared_type$(\n"
    "      input, this->add_$name$()));\n");
  if (HasUtf8Verification(descriptor_->file()) &&
      descriptor_->type() == FieldDescriptor::TYPE_STRING) {
    printer->Print(variables_,
      "::google::protobuf::internal::WireFormat::VerifyUTF8Unistr(\n"
      "  this->$name$(0).data(), this->$name$(0).length(),\n"
      "  ::google::protobuf::internal::WireFormat::PARSE);\n");
  }
}

void RepeatedUnistrFieldGenerator::
GenerateSerializeWithCachedSizes(io::Printer* printer) const {
  printer->Print(variables_,
    "for (int i = 0; i < this->$name$_size(); i++) {\n");
  if (HasUtf8Verification(descriptor_->file()) &&
      descriptor_->type() == FieldDescriptor::TYPE_STRING) {
    printer->Print(variables_,
      "::google::protobuf::internal::WireFormat::VerifyUTF8Unistr(\n"
      "  this->$name$(i).data(), this->$name$(i).length(),\n"
      "  ::google::protobuf::internal::WireFormat::SERIALIZE);\n");
  }
  printer->Print(variables_,
    "  ::google::protobuf::internal::WireFormatLite::Write$declared_type$(\n"
    "    $number$, this->$name$(i), output);\n"
    "}\n");
}

void RepeatedUnistrFieldGenerator::
GenerateSerializeWithCachedSizesToArray(io::Printer* printer) const {
  printer->Print(variables_,
    "for (int i = 0; i < this->$name$_size(); i++) {\n");
  if (HasUtf8Verification(descriptor_->file()) &&
      descriptor_->type() == FieldDescriptor::TYPE_STRING) {
    printer->Print(variables_,
      "  ::google::protobuf::internal::WireFormat::VerifyUTF8Unistr(\n"
      "    this->$name$(i).data(), this->$name$(i).length(),\n"
      "    ::google::protobuf::internal::WireFormat::SERIALIZE);\n");
  }
  printer->Print(variables_,
    "  target = ::google::protobuf::internal::WireFormatLite::\n"
    "    Write$declared_type$ToArray($number$, this->$name$(i), target);\n"
    "}\n");
}

void RepeatedUnistrFieldGenerator::
GenerateByteSize(io::Printer* printer) const {
  printer->Print(variables_,
    "total_size += $tag_size$ * this->$name$_size();\n"
    "for (int i = 0; i < this->$name$_size(); i++) {\n"
    "  total_size += ::google::protobuf::internal::WireFormatLite::$declared_type$Size(\n"
    "    this->$name$(i));\n"
    "}\n");
}

}  // namespace cpp
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
