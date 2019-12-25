// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/compiler/cpp/cpp_test_large_enum_value.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_google_2fprotobuf_2fcompiler_2fcpp_2fcpp_5ftest_5flarge_5fenum_5fvalue_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_google_2fprotobuf_2fcompiler_2fcpp_2fcpp_5ftest_5flarge_5fenum_5fvalue_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3011000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3011002 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_google_2fprotobuf_2fcompiler_2fcpp_2fcpp_5ftest_5flarge_5fenum_5fvalue_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_google_2fprotobuf_2fcompiler_2fcpp_2fcpp_5ftest_5flarge_5fenum_5fvalue_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_google_2fprotobuf_2fcompiler_2fcpp_2fcpp_5ftest_5flarge_5fenum_5fvalue_2eproto;
namespace protobuf_unittest {
class TestLargeEnumValue;
class TestLargeEnumValueDefaultTypeInternal;
extern TestLargeEnumValueDefaultTypeInternal _TestLargeEnumValue_default_instance_;
}  // namespace protobuf_unittest
PROTOBUF_NAMESPACE_OPEN
template<> ::protobuf_unittest::TestLargeEnumValue* Arena::CreateMaybeMessage<::protobuf_unittest::TestLargeEnumValue>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace protobuf_unittest {

enum TestLargeEnumValue_EnumWithLargeValue : int {
  TestLargeEnumValue_EnumWithLargeValue_VALUE_1 = 1,
  TestLargeEnumValue_EnumWithLargeValue_VALUE_MAX = 2147483647
};
bool TestLargeEnumValue_EnumWithLargeValue_IsValid(int value);
constexpr TestLargeEnumValue_EnumWithLargeValue TestLargeEnumValue_EnumWithLargeValue_EnumWithLargeValue_MIN = TestLargeEnumValue_EnumWithLargeValue_VALUE_1;
constexpr TestLargeEnumValue_EnumWithLargeValue TestLargeEnumValue_EnumWithLargeValue_EnumWithLargeValue_MAX = TestLargeEnumValue_EnumWithLargeValue_VALUE_MAX;
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* TestLargeEnumValue_EnumWithLargeValue_descriptor();
template<typename T>
inline const std::string& TestLargeEnumValue_EnumWithLargeValue_Name(T enum_t_value) {
  static_assert(::std::is_same<T, TestLargeEnumValue_EnumWithLargeValue>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function TestLargeEnumValue_EnumWithLargeValue_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    TestLargeEnumValue_EnumWithLargeValue_descriptor(), enum_t_value);
}
inline bool TestLargeEnumValue_EnumWithLargeValue_Parse(
    const std::string& name, TestLargeEnumValue_EnumWithLargeValue* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<TestLargeEnumValue_EnumWithLargeValue>(
    TestLargeEnumValue_EnumWithLargeValue_descriptor(), name, value);
}
// ===================================================================

class TestLargeEnumValue :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:protobuf_unittest.TestLargeEnumValue) */ {
 public:
  TestLargeEnumValue();
  virtual ~TestLargeEnumValue();

  TestLargeEnumValue(const TestLargeEnumValue& from);
  TestLargeEnumValue(TestLargeEnumValue&& from) noexcept
    : TestLargeEnumValue() {
    *this = ::std::move(from);
  }

  inline TestLargeEnumValue& operator=(const TestLargeEnumValue& from) {
    CopyFrom(from);
    return *this;
  }
  inline TestLargeEnumValue& operator=(TestLargeEnumValue&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const TestLargeEnumValue& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const TestLargeEnumValue* internal_default_instance() {
    return reinterpret_cast<const TestLargeEnumValue*>(
               &_TestLargeEnumValue_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(TestLargeEnumValue& a, TestLargeEnumValue& b) {
    a.Swap(&b);
  }
  inline void Swap(TestLargeEnumValue* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline TestLargeEnumValue* New() const final {
    return CreateMaybeMessage<TestLargeEnumValue>(nullptr);
  }

  TestLargeEnumValue* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<TestLargeEnumValue>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const TestLargeEnumValue& from);
  void MergeFrom(const TestLargeEnumValue& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(TestLargeEnumValue* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "protobuf_unittest.TestLargeEnumValue";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_google_2fprotobuf_2fcompiler_2fcpp_2fcpp_5ftest_5flarge_5fenum_5fvalue_2eproto);
    return ::descriptor_table_google_2fprotobuf_2fcompiler_2fcpp_2fcpp_5ftest_5flarge_5fenum_5fvalue_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  typedef TestLargeEnumValue_EnumWithLargeValue EnumWithLargeValue;
  static constexpr EnumWithLargeValue VALUE_1 =
    TestLargeEnumValue_EnumWithLargeValue_VALUE_1;
  static constexpr EnumWithLargeValue VALUE_MAX =
    TestLargeEnumValue_EnumWithLargeValue_VALUE_MAX;
  static inline bool EnumWithLargeValue_IsValid(int value) {
    return TestLargeEnumValue_EnumWithLargeValue_IsValid(value);
  }
  static constexpr EnumWithLargeValue EnumWithLargeValue_MIN =
    TestLargeEnumValue_EnumWithLargeValue_EnumWithLargeValue_MIN;
  static constexpr EnumWithLargeValue EnumWithLargeValue_MAX =
    TestLargeEnumValue_EnumWithLargeValue_EnumWithLargeValue_MAX;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  EnumWithLargeValue_descriptor() {
    return TestLargeEnumValue_EnumWithLargeValue_descriptor();
  }
  template<typename T>
  static inline const std::string& EnumWithLargeValue_Name(T enum_t_value) {
    static_assert(::std::is_same<T, EnumWithLargeValue>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function EnumWithLargeValue_Name.");
    return TestLargeEnumValue_EnumWithLargeValue_Name(enum_t_value);
  }
  static inline bool EnumWithLargeValue_Parse(const std::string& name,
      EnumWithLargeValue* value) {
    return TestLargeEnumValue_EnumWithLargeValue_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // @@protoc_insertion_point(class_scope:protobuf_unittest.TestLargeEnumValue)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_google_2fprotobuf_2fcompiler_2fcpp_2fcpp_5ftest_5flarge_5fenum_5fvalue_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// TestLargeEnumValue

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf_unittest

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::protobuf_unittest::TestLargeEnumValue_EnumWithLargeValue> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::protobuf_unittest::TestLargeEnumValue_EnumWithLargeValue>() {
  return ::protobuf_unittest::TestLargeEnumValue_EnumWithLargeValue_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_google_2fprotobuf_2fcompiler_2fcpp_2fcpp_5ftest_5flarge_5fenum_5fvalue_2eproto
