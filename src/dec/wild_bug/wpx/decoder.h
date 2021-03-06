#pragma once

#include <memory>
#include "io/base_byte_stream.h"

namespace au {
namespace dec {
namespace wild_bug {
namespace wpx {

    class Decoder final
    {
    public:
        Decoder(io::BaseByteStream &input_stream);
        ~Decoder();

        std::string get_tag() const;
        const std::vector<u8> get_sections() const;
        bool has_section(u8 section_id) const;
        bstr read_plain_section(u8 section_id);
        bstr read_compressed_section(u8 section_id);
        bstr read_compressed_section(
            u8 section_id, s8 quant_size, const std::array<size_t, 8> &offsets);

    private:
        struct Priv;
        std::unique_ptr<Priv> p;
    };

} } } }
