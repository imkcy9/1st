// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: queue_enum.proto

#ifndef PROTOBUF_queue_5fenum_2eproto__INCLUDED
#define PROTOBUF_queue_5fenum_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3005001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)

namespace protobuf_queue_5fenum_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
inline void InitDefaults() {
}
}  // namespace protobuf_queue_5fenum_2eproto
namespace FIRST {
}  // namespace FIRST
namespace FIRST {

enum RequestType {
  REQUESTTYPE_GETAPITYPES = 0,
  REQUESTTYPE_GETAPIVERSION = 1,
  REQUESTTYPE_GETAPINAME = 2,
  REQUESTTYPE_CREATE = 3,
  REQUESTTYPE_RELEASE = 4,
  REQUESTTYPE_REGISTER = 5,
  REQUESTTYPE_CONFIG = 6,
  REQUESTTYPE_CONNECT = 7,
  REQUESTTYPE_DISCONNECT = 8,
  REQUESTTYPE_CLEAR = 9,
  REQUESTTYPE_PROCESS = 10,
  REQUESTTYPE_SUBSCRIBE = 11,
  REQUESTTYPE_UNSUBSCRIBE = 12,
  REQUESTTYPE_SUBSCRIBEQUOTE = 13,
  REQUESTTYPE_UNSUBSCRIBEQUOTE = 14,
  REQUESTTYPE_REQORDERINSERT = 15,
  REQUESTTYPE_REQQUOTEINSERT = 16,
  REQUESTTYPE_REQORDERACTION = 17,
  REQUESTTYPE_REQQUOTEACTION = 18,
  RequestType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  RequestType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool RequestType_IsValid(int value);
const RequestType RequestType_MIN = REQUESTTYPE_GETAPITYPES;
const RequestType RequestType_MAX = REQUESTTYPE_REQQUOTEACTION;
const int RequestType_ARRAYSIZE = RequestType_MAX + 1;

const ::google::protobuf::EnumDescriptor* RequestType_descriptor();
inline const ::std::string& RequestType_Name(RequestType value) {
  return ::google::protobuf::internal::NameOfEnum(
    RequestType_descriptor(), value);
}
inline bool RequestType_Parse(
    const ::std::string& name, RequestType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<RequestType>(
    RequestType_descriptor(), name, value);
}
enum QueryType {
  QUERYTYPE_REQQRYINSTRUMENT = 0,
  QUERYTYPE_REQQRYTRADINGACCOUNT = 1,
  QUERYTYPE_REQQRYINVESTORPOSITION = 2,
  QUERYTYPE_REQQRYORDER = 3,
  QUERYTYPE_REQQRYTRADE = 4,
  QUERYTYPE_REQQRYQUOTE = 5,
  QUERYTYPE_REQQRYINSTRUMENTCOMMISSIONRATE = 6,
  QUERYTYPE_REQQRYINSTRUMENTMARGINRATE = 7,
  QUERYTYPE_REQQRYSETTLEMENTINFO = 9,
  QUERYTYPE_REQQRYINVESTOR = 10,
  QUERYTYPE_REQQRYHISTORICALTICKS = 11,
  QUERYTYPE_REQQRYHISTORICALBARS = 12,
  QueryType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  QueryType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool QueryType_IsValid(int value);
const QueryType QueryType_MIN = QUERYTYPE_REQQRYINSTRUMENT;
const QueryType QueryType_MAX = QUERYTYPE_REQQRYHISTORICALBARS;
const int QueryType_ARRAYSIZE = QueryType_MAX + 1;

const ::google::protobuf::EnumDescriptor* QueryType_descriptor();
inline const ::std::string& QueryType_Name(QueryType value) {
  return ::google::protobuf::internal::NameOfEnum(
    QueryType_descriptor(), value);
}
inline bool QueryType_Parse(
    const ::std::string& name, QueryType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<QueryType>(
    QueryType_descriptor(), name, value);
}
enum ResponseType {
  RESPONSETYPE_ONCONNECTIONSTATUS = 0,
  RESPONSETYPE_ONRTNERROR = 1,
  RESPONSETYPE_ONLOG = 2,
  RESPONSETYPE_ONRTNDEPTHMARKETDATA = 3,
  RESPONSETYPE_ONRSPQRYINSTRUMENT = 4,
  RESPONSETYPE_ONRSPQRYTRADINGACCOUNT = 5,
  RESPONSETYPE_ONRSPQRYINVESTORPOSITION = 6,
  RESPONSETYPE_ONRSPQRYSETTLEMENTINFO = 7,
  RESPONSETYPE_ONRSPQRYORDER = 8,
  RESPONSETYPE_ONRSPQRYTRADE = 9,
  RESPONSETYPE_ONRSPQRYQUOTE = 10,
  RESPONSETYPE_ONRTNORDER = 11,
  RESPONSETYPE_ONRTNTRADE = 12,
  RESPONSETYPE_ONRTNQUOTE = 13,
  RESPONSETYPE_ONRTNQUOTEREQUEST = 14,
  RESPONSETYPE_ONRSPQRYHISTORICALTICKS = 15,
  RESPONSETYPE_ONRSPQRYHISTORICALBARS = 16,
  RESPONSETYPE_ONRSPQRYINVESTOR = 17,
  RESPONSETYPE_ONRTNINSTRUMENTSTATUS = 18,
  RESPONSETYPE_ONFILTERSUBSCRIBE = 19,
  ResponseType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  ResponseType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool ResponseType_IsValid(int value);
const ResponseType ResponseType_MIN = RESPONSETYPE_ONCONNECTIONSTATUS;
const ResponseType ResponseType_MAX = RESPONSETYPE_ONFILTERSUBSCRIBE;
const int ResponseType_ARRAYSIZE = ResponseType_MAX + 1;

const ::google::protobuf::EnumDescriptor* ResponseType_descriptor();
inline const ::std::string& ResponseType_Name(ResponseType value) {
  return ::google::protobuf::internal::NameOfEnum(
    ResponseType_descriptor(), value);
}
inline bool ResponseType_Parse(
    const ::std::string& name, ResponseType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<ResponseType>(
    ResponseType_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace FIRST

namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::FIRST::RequestType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::FIRST::RequestType>() {
  return ::FIRST::RequestType_descriptor();
}
template <> struct is_proto_enum< ::FIRST::QueryType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::FIRST::QueryType>() {
  return ::FIRST::QueryType_descriptor();
}
template <> struct is_proto_enum< ::FIRST::ResponseType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::FIRST::ResponseType>() {
  return ::FIRST::ResponseType_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_queue_5fenum_2eproto__INCLUDED
