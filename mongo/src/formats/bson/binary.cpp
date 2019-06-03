#include <formats/bson/binary.hpp>

#include <bson/bson.h>

#include <formats/bson/exception.hpp>

#include <formats/bson/wrappers.hpp>

namespace formats::bson {

Document FromBinaryString(utils::string_view binary) {
  impl::MutableBson native(reinterpret_cast<const uint8_t*>(binary.data()),
                           binary.size());

  if (!native.Get()) {
    throw ParseException("malformed BSON: invalid document length");
  }

  bson_error_t validation_error;
  if (!bson_validate_with_error(
          native.Get(),
          static_cast<bson_validate_flags_t>(BSON_VALIDATE_UTF8 |
                                             BSON_VALIDATE_UTF8_ALLOW_NULL |
                                             BSON_VALIDATE_EMPTY_KEYS),
          &validation_error)) {
    throw ParseException("malformed BSON: ") << validation_error.message;
  }

  return Document(native.Extract());
}

BsonString ToBinaryString(const formats::bson::Document& doc) {
  return BsonString(doc.GetBson());
}

BsonString::BsonString(impl::BsonHolder impl) : impl_(std::move(impl)) {}

std::string BsonString::ToString() const {
  return std::string(reinterpret_cast<const char*>(Data()), Size());
}

utils::string_view BsonString::GetView() const {
  return utils::string_view(reinterpret_cast<const char*>(Data()), Size());
}

const uint8_t* BsonString::Data() const { return bson_get_data(impl_.get()); }

size_t BsonString::Size() const { return impl_->len; }

}  // namespace formats::bson