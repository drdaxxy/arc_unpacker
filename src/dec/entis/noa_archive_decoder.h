#pragma once

#include "dec/base_archive_decoder.h"

namespace au {
namespace dec {
namespace entis {

    class NoaArchiveDecoder final : public BaseArchiveDecoder
    {
    public:
        NoaArchiveDecoder();
        std::vector<std::string> get_linked_formats() const override;

    protected:
        bool is_recognized_impl(io::File &input_file) const override;

        std::unique_ptr<ArchiveMeta> read_meta_impl(
            const Logger &logger,
            io::File &input_file) const override;

        std::unique_ptr<io::File> read_file_impl(
            const Logger &logger,
            io::File &input_file,
            const ArchiveMeta &m,
            const ArchiveEntry &e) const override;

    public:
        std::string key;
    };

} } }
