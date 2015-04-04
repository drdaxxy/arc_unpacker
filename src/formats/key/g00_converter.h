#ifndef FORMATS_KEY_G00_CONVERTER_H
#define FORMATS_KEY_G00_CONVERTER_H
#include "formats/converter.h"

namespace Formats
{
    namespace Key
    {
        class G00Converter final : public Converter
        {
        protected:
            std::unique_ptr<File> decode_internal(File &) const override;
        };
    }
}

#endif
