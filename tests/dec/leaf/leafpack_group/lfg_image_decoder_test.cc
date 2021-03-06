#include "dec/leaf/leafpack_group/lfg_image_decoder.h"
#include "test_support/catch.h"
#include "test_support/decoder_support.h"
#include "test_support/file_support.h"
#include "test_support/image_support.h"

using namespace au;
using namespace au::dec::leaf;

static const std::string dir = "tests/dec/leaf/files/lfg/";

static void do_test(
    const std::string &input_path, const std::string &expected_path)
{
    const auto decoder = LfgImageDecoder();
    const auto input_file = tests::file_from_path(dir + input_path);
    const auto expected_file = tests::file_from_path(dir + expected_path);
    const auto actual_image = tests::decode(decoder, *input_file);
    tests::compare_images(actual_image, *expected_file);
}

TEST_CASE("Leaf LFG images", "[dec]")
{
    SECTION("Vertical coding")
    {
        do_test("OP_L6.LFG", "OP_L6-out.png");
    }

    SECTION("Horizontal coding")
    {
        do_test("MAX_C09.LFG", "MAX_C09-out.png");
    }
}
