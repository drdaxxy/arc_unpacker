#include "dec/leaf/leafpack_group/p16_audio_decoder.h"

using namespace au;
using namespace au::dec::leaf;

bool P16AudioDecoder::is_recognized_impl(io::File &input_file) const
{
    return input_file.path.has_extension("P16");
}

res::Audio P16AudioDecoder::decode_impl(
    const Logger &logger, io::File &input_file) const
{
    input_file.stream.seek(0);
    const auto samples = input_file.stream.read_to_eof();
    res::Audio audio;
    audio.samples = samples;
    return audio;
}

static auto _ = dec::register_decoder<P16AudioDecoder>("leaf/p16");
