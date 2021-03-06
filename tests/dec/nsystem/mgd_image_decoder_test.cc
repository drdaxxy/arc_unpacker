#include "dec/nsystem/mgd_image_decoder.h"
#include "test_support/catch.h"
#include "test_support/decoder_support.h"
#include "test_support/file_support.h"
#include "test_support/image_support.h"

using namespace au;
using namespace au::dec::nsystem;

static const std::string dir = "tests/dec/nsystem/files/mgd/";

static void do_test(
    const std::string &input_path, const std::string &expected_path)
{
    const auto decoder = MgdImageDecoder();
    const auto input_file = tests::file_from_path(dir + input_path);
    const auto expected_file = tests::file_from_path(dir + expected_path);
    const auto actual_image = tests::decode(decoder, *input_file);
    tests::compare_images(actual_image, *expected_file);
}

TEST_CASE("NSystem MGD images", "[dec]")
{
    SECTION("RGB")
    {
        // possible BGR-RGB issues. Need a non-monochrome sample file.
        do_test("GS_UD.MGD", "GS_UD-out.png");
    }

    SECTION("PNG")
    {
        do_test("saveload_p.MGD", "saveload_p-out.png");
    }
}
