﻿#include "dec/kaguya/link5_archive_decoder.h"
#include "algo/binary.h"
#include "algo/locale.h"
#include "test_support/catch.h"
#include "test_support/decoder_support.h"
#include "test_support/file_support.h"
#include "virtual_file_system.h"

using namespace au;
using namespace au::dec::kaguya;

TEST_CASE("Atelier Kaguya LINK5 archives", "[dec]")
{
    const auto arc_name = "tst"_b;

    const std::vector<std::shared_ptr<io::File>> &expected_files =
    {
        tests::stub_file("123.txt", "1234567890"_b),
        tests::stub_file("abc.txt", "abcdefghijklmnopqrstuvwxyz"_b),
        tests::stub_file("魔法.txt", "expecto patronum"_b),
    };

    io::File input_file;
    input_file.stream.write("LINK5"_b);
    input_file.stream.write(arc_name);
    input_file.stream.write("??"_b);
    for (const auto &file : expected_files)
    {
        const auto data = file->stream.seek(0).read_to_eof();
        const auto name = algo::utf8_to_sjis(file->path.str());
        input_file.stream.write_le<u32>(data.size() + 16 + name.size());
        input_file.stream.write<u8>(0);
        input_file.stream.write("????????"_b);
        input_file.stream.write<u8>(name.size());
        input_file.stream.write("??"_b);
        input_file.stream.write(name);
        input_file.stream.write(data);
    }
    input_file.stream.write_le<u32>(0);

    const auto decoder = Link5ArchiveDecoder();
    const auto actual_files = tests::unpack(decoder, input_file);
    tests::compare_files(actual_files, expected_files, true);
}
