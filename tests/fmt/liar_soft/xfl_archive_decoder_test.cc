#include "fmt/liar_soft/xfl_archive_decoder.h"
#include "test_support/archive_support.h"
#include "test_support/catch.hh"
#include "test_support/file_support.h"

using namespace au;
using namespace au::fmt::liar_soft;

TEST_CASE("LiarSoft XFL archives", "[fmt]")
{
    std::vector<std::shared_ptr<File>> expected_files
    {
        tests::stub_file("abc.txt", "123"_b),
        tests::stub_file("another.txt", "abcdefghij"_b),
    };

    XflArchiveDecoder decoder;
    auto actual_files = au::tests::unpack_to_memory(
        "tests/fmt/liar_soft/files/xfl/test.xfl", decoder);

    au::tests::compare_files(expected_files, actual_files, true);
}