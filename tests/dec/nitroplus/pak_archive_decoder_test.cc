#include "dec/nitroplus/pak_archive_decoder.h"
#include "test_support/catch.h"
#include "test_support/decoder_support.h"
#include "test_support/file_support.h"

using namespace au;
using namespace au::dec::nitroplus;

static const std::string dir = "tests/dec/nitroplus/files/pak/";

static void do_test(const std::string &path)
{
    const std::vector<std::shared_ptr<io::File>> expected_files
    {
        tests::stub_file("123.txt", "1234567890"_b),
        tests::stub_file("abc.txt", "abcdefghijklmnopqrstuvwxyz"_b),
    };
    const auto decoder = PakArchiveDecoder();
    const auto input_file = tests::file_from_path(dir + path);
    const auto actual_files = tests::unpack(decoder, *input_file);
    tests::compare_files(actual_files, expected_files, true);
}

TEST_CASE("Nitroplus PAK archives", "[dec]")
{
    SECTION("Uncompressed")
    {
        do_test("uncompressed.pak");
    }

    SECTION("Compressed")
    {
        do_test("compressed.pak");
    }
}
