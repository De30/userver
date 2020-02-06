#include <storages/postgres/io/bytea.hpp>
#include <storages/postgres/tests/test_buffers.hpp>
#include <storages/postgres/tests/util_pgtest.hpp>

namespace pg = storages::postgres;
namespace io = pg::io;
namespace tt = io::traits;

namespace static_test {

static_assert(tt::kIsByteaCompatible<std::string>, "");
static_assert(tt::kIsByteaCompatible<::utils::string_view>, "");
static_assert(tt::kIsByteaCompatible<std::vector<char>>, "");
static_assert(tt::kIsByteaCompatible<std::vector<unsigned char>>, "");
static_assert(!tt::kIsByteaCompatible<std::vector<bool>>, "");
static_assert(!tt::kIsByteaCompatible<std::vector<int>>, "");

}  // namespace static_test

namespace {

using namespace std::string_literals;

const pg::UserTypes types;

const std::string kFooBar = "foo\0bar"s;

TEST(PostgreIO, Bytea) {
  {
    pg::test::Buffer buffer;
    std::string bin_str{kFooBar};
    EXPECT_NO_THROW(io::WriteBuffer<io::DataFormat::kBinaryDataFormat>(
        types, buffer, pg::Bytea(bin_str)));
    EXPECT_EQ(kFooBar.size(), buffer.size());
    auto fb =
        pg::test::MakeFieldBuffer(buffer, io::DataFormat::kBinaryDataFormat,
                                  io::BufferCategory::kPlainBuffer);
    std::string tgt_str;
    EXPECT_NO_THROW(io::ReadBuffer<io::DataFormat::kBinaryDataFormat>(
        fb, pg::Bytea(tgt_str)));
    EXPECT_EQ(bin_str, tgt_str);
  }
  {
    pg::test::Buffer buffer;
    ::utils::string_view bin_str{kFooBar.data(), kFooBar.size()};
    EXPECT_NO_THROW(io::WriteBuffer<io::DataFormat::kBinaryDataFormat>(
        types, buffer, pg::Bytea(bin_str)));
    EXPECT_EQ(kFooBar.size(), buffer.size());
    auto fb =
        pg::test::MakeFieldBuffer(buffer, io::DataFormat::kBinaryDataFormat,
                                  io::BufferCategory::kPlainBuffer);
    ::utils::string_view tgt_str;
    EXPECT_NO_THROW(io::ReadBuffer<io::DataFormat::kBinaryDataFormat>(
        fb, pg::Bytea(tgt_str)));
    EXPECT_EQ(bin_str, tgt_str);
  }
  {
    pg::test::Buffer buffer;
    std::vector<char> bin_str{kFooBar.begin(), kFooBar.end()};
    EXPECT_NO_THROW(io::WriteBuffer<io::DataFormat::kBinaryDataFormat>(
        types, buffer, pg::Bytea(bin_str)));
    EXPECT_EQ(kFooBar.size(), buffer.size());
    auto fb =
        pg::test::MakeFieldBuffer(buffer, io::DataFormat::kBinaryDataFormat,
                                  io::BufferCategory::kPlainBuffer);
    std::vector<char> tgt_str;
    EXPECT_NO_THROW(io::ReadBuffer<io::DataFormat::kBinaryDataFormat>(
        fb, pg::Bytea(tgt_str)));
    EXPECT_EQ(bin_str, tgt_str);
  }
  {
    pg::test::Buffer buffer;
    std::vector<unsigned char> bin_str{kFooBar.begin(), kFooBar.end()};
    EXPECT_NO_THROW(io::WriteBuffer<io::DataFormat::kBinaryDataFormat>(
        types, buffer, pg::Bytea(bin_str)));
    EXPECT_EQ(kFooBar.size(), buffer.size());
    auto fb =
        pg::test::MakeFieldBuffer(buffer, io::DataFormat::kBinaryDataFormat,
                                  io::BufferCategory::kPlainBuffer);
    std::vector<unsigned char> tgt_str;
    EXPECT_NO_THROW(io::ReadBuffer<io::DataFormat::kBinaryDataFormat>(
        fb, pg::Bytea(tgt_str)));
    EXPECT_EQ(bin_str, tgt_str);
  }
}

POSTGRE_TEST_P(ByteaRoundtrip) {
  ASSERT_TRUE(conn.get()) << "Expected non-empty connection pointer";
  pg::ResultSet res{nullptr};
  EXPECT_NO_THROW(res = conn->Execute("select $1", pg::Bytea(kFooBar)));
  std::string tgt_str;
  EXPECT_NO_THROW(res[0][0].To(pg::Bytea(tgt_str)));
  EXPECT_EQ(kFooBar, tgt_str);
}

}  // namespace