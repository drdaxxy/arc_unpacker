#ifndef FORMATS_LIZSOFT_SOTES_CONVERTER_H
#define FORMATS_LIZSOFT_SOTES_CONVERTER_H
#include "formats/converter.h"

namespace Formats
{
    namespace Lizsoft
    {
        class SotesConverter final : public Converter
        {
        protected:
            std::unique_ptr<File> decode_internal(File &) const override;
        };
    }
}

#endif
