#include "dec/shiina_rio/ogv_audio_decoder.h"
#include "algo/range.h"
#include "err.h"

using namespace au;
using namespace au::dec::shiina_rio;

static const bstr magic = "OGV\x00"_b;

bool OgvAudioDecoder::is_recognized_impl(io::File &input_file) const
{
    return input_file.stream.read(magic.size()) == magic;
}

std::unique_ptr<io::File> OgvAudioDecoder::decode_impl(
    const Logger &logger, io::File &input_file) const
{
    input_file.stream.seek(magic.size());
    input_file.stream.skip(4);
    input_file.stream.skip(4); // sort of file size
    if (input_file.stream.read(4) != "fmt\x20"_b)
        throw err::CorruptDataError("Expected fmt chunk");
    input_file.stream.skip(input_file.stream.read_le<u32>());

    if (input_file.stream.read(4) != "data"_b)
        throw err::CorruptDataError("Expected data chunk");
    input_file.stream.skip(4);
    const auto data = input_file.stream.read_to_eof();

    auto output_file = std::make_unique<io::File>(input_file.path, data);
    output_file->guess_extension();
    return output_file;
}

static auto _ = dec::register_decoder<OgvAudioDecoder>("shiina-rio/ogv");
