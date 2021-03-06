#pragma once

#include "io/base_byte_stream.h"

namespace au {
namespace dec {
namespace nscripter {

    class NsaEncryptedStream final : public io::BaseByteStream
    {
    public:
        NsaEncryptedStream(io::BaseByteStream &parent_stream, const bstr &key);
        ~NsaEncryptedStream();

        uoff_t size() const override;
        uoff_t pos() const override;
        std::unique_ptr<BaseByteStream> clone() const override;

    protected:
        void read_impl(void *destination, const size_t size) override;
        void write_impl(const void *source, const size_t size) override;
        void seek_impl(const uoff_t offset) override;
        void resize_impl(const uoff_t new_size) override;

    private:
        std::unique_ptr<io::BaseByteStream> parent_stream;
        const bstr key;
    };

} } }
