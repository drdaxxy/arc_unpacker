#include "dec/twilight_frontier/tfwa_audio_decoder.h"
#include "test_support/audio_support.h"
#include "test_support/catch.h"
#include "test_support/decoder_support.h"
#include "test_support/file_support.h"

using namespace au;
using namespace au::dec::twilight_frontier;

static const std::string dir = "tests/dec/twilight_frontier/files/tfwa/";

static void do_test(
    const std::string &input_path, const std::string &expected_path)
{
    const auto decoder = TfwaAudioDecoder();
    const auto input_file = tests::file_from_path(dir + input_path);
    const auto expected_file = tests::file_from_path(dir + expected_path);
    const auto actual_audio = tests::decode(decoder, *input_file);
    tests::compare_audio(actual_audio, *expected_file);
}

TEST_CASE("Twilight Frontier TFWA audio", "[dec]")
{
    do_test("2592.wav", "2592-out.wav");
}
