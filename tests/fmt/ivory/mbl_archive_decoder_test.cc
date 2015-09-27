#include "fmt/ivory/mbl_archive_decoder.h"
#include "test_support/archive_support.h"
#include "test_support/catch.hh"
#include "test_support/file_support.h"

using namespace au;
using namespace au::fmt::ivory;

static void do_test(const std::string &path)
{
    std::vector<std::shared_ptr<File>> expected_files
    {
        tests::stub_file("abc.txt", "abc"_b),
        tests::stub_file("テスト", "AAAAAAAAAAAAAAAA"_b),
    };

    MblArchiveDecoder decoder;
    auto actual_files = au::tests::unpack_to_memory(path, decoder);

    tests::compare_files(expected_files, actual_files, true);
}

static void do_test_encrypted(
    const std::string &input_arc_path,
    const std::string &expected_file_path,
    const std::string &plugin_name)
{
    std::vector<std::shared_ptr<File>> expected_files
    {
        tests::file_from_path(expected_file_path),
    };

    MblArchiveDecoder decoder;
    decoder.set_plugin(plugin_name);
    auto actual_files = au::tests::unpack_to_memory(input_arc_path, decoder);

    tests::compare_files(expected_files, actual_files, false);
}

TEST_CASE("Ivory MBL v1 archives", "[fmt]")
{
    do_test("tests/fmt/ivory/files/mbl/mbl-v1.mbl");
}

TEST_CASE("Ivory MBL v2 archives", "[fmt]")
{
    do_test("tests/fmt/ivory/files/mbl/mbl-v2.mbl");
}

TEST_CASE("Ivory MBL dialogs (Candy Toys encryption)", "[fmt]")
{
    do_test_encrypted(
        "tests/fmt/ivory/files/mbl/mg_data-candy.mbl",
        "tests/fmt/ivory/files/mbl/MAIN-candy",
        "candy");
}

TEST_CASE("Ivory MBL dialogs (Wanko to Kurasou encryption)", "[fmt]")
{
    do_test_encrypted(
        "tests/fmt/ivory/files/mbl/mg_data-wanko.mbl",
        "tests/fmt/ivory/files/mbl/TEST-wanko",
        "wanko");
}