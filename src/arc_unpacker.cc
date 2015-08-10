#include <boost/filesystem.hpp>
#include "arc_unpacker.h"
#include "log.h"
#include "util/format.h"
#include "util/range.h"

using namespace au;
using namespace au::fmt;

namespace
{
    struct PathInfo
    {
        std::string input_path;
        std::string base_name;
    };

    struct Options
    {
        std::string format;
        boost::filesystem::path output_dir;
        std::vector<std::unique_ptr<PathInfo>> input_paths;
        bool overwrite;
    };
}

static void add_help_option(ArgParser &arg_parser)
{
    arg_parser.add_help("-h, --help", "Shows this message.");
}

static bool should_show_help(ArgParser &arg_parser)
{
    return arg_parser.has_flag("-h") || arg_parser.has_flag("--help");
}

static void add_rename_option(ArgParser &arg_parser, Options &options)
{
    arg_parser.add_help(
        "-r, --rename",
        "Renames existing target files.\nBy default, they're overwritten.");

    options.overwrite = true;
    if (arg_parser.has_flag("-r") || arg_parser.has_flag("--rename"))
        options.overwrite = false;
}

static void add_output_folder_option(
    ArgParser &arg_parser, Options &options)
{
    arg_parser.add_help(
        "-o, --out=FOLDER",
        "Where to put the output files.");

    if (arg_parser.has_switch("-o"))
        options.output_dir = arg_parser.get_switch("-o");
    else if (arg_parser.has_switch("--out"))
        options.output_dir = arg_parser.get_switch("--out");
    else
        options.output_dir = "./";
}

static void add_format_option(ArgParser &arg_parser, Options &options)
{
    arg_parser.add_help(
        "-f, --fmt=FORMAT",
        "Disables guessing and selects given format.");

    if (arg_parser.has_switch("-f"))
        options.format = arg_parser.get_switch("-f");
    if (arg_parser.has_switch("--fmt"))
        options.format = arg_parser.get_switch("--fmt");
}

static bool add_input_paths_option(
    ArcUnpacker &arc_unpacker,
    ArgParser &arg_parser,
    Options &options)
{
    const std::vector<std::string> stray = arg_parser.get_stray();
    for (auto i : util::range(1, stray.size()))
    {
        std::string path = stray[i];
        if (boost::filesystem::is_directory(path))
        {
            for (boost::filesystem::recursive_directory_iterator it(path);
                it != boost::filesystem::recursive_directory_iterator();
                it++)
            {
                std::unique_ptr<PathInfo> pi(new PathInfo);
                pi->input_path = it->path().string();
                pi->base_name = pi->input_path.substr(path.size());
                while (pi->base_name.size() > 0 &&
                    (pi->base_name[0] == '/' || pi->base_name[0] == '\\'))
                {
                    pi->base_name = pi->base_name.substr(1);
                }
                options.input_paths.push_back(std::move(pi));
            }
        }
        else
        {
            std::unique_ptr<PathInfo> pi(new PathInfo);
            pi->input_path = path;
            pi->base_name
                = boost::filesystem::path(path).filename().string();
            options.input_paths.push_back(std::move(pi));
        }
    }

    if (options.input_paths.size() < 1)
    {
        Log.info( "Error: required more arguments.\n\n");
        arc_unpacker.print_help(stray[0]);
        return false;
    }

    return true;
}

struct ArcUnpacker::Priv
{
    Options options;
    ArgParser &arg_parser;
    TransformerFactory factory;
    std::string version;

    Priv(ArgParser &arg_parser, const std::string &version)
        : arg_parser(arg_parser), version(version)
    {
    }
};

ArcUnpacker::ArcUnpacker(ArgParser &arg_parser, const std::string &version)
    : p(new Priv(arg_parser, version))
{
    add_output_folder_option(arg_parser, p->options);
    add_format_option(arg_parser, p->options);
    add_rename_option(arg_parser, p->options);
    add_help_option(arg_parser);
}

ArcUnpacker::~ArcUnpacker()
{
}

void ArcUnpacker::print_help(const std::string &path_to_self)
{
    Log.info("arc_unpacker v" + p->version + "\n");
    Log.info("Extracts images and sounds from various visual novels.\n\n");
    Log.info("Usage: " + path_to_self + " \\\n");
    Log.info("       [options] [fmt_options] input_path [input_path...]\n\n");
    Log.info(
R"(Depending on the format, files will be saved either in a subdirectory
(archives), or aside the input files (images, music etc.). If no output
directory is provided, files are going to be saved inside current working
directory.

[options] can be:

)");

    p->arg_parser.print_help();
    p->arg_parser.clear_help();
    Log.info("\n");

    auto format = p->options.format;
    if (format != "")
    {
        std::unique_ptr<Transformer> transformer = p->factory.create(format);

        if (transformer != nullptr)
        {
            transformer->add_cli_help(p->arg_parser);
            Log.info(
                "[fmt_options] specific to " + p->options.format + ":\n\n");
            p->arg_parser.print_help();
            return;
        }
    }

    Log.info(
R"([fmt_options] depend on chosen format and are required at runtime.
See --help --fmt=FORMAT to get detailed help for given transformer.

Supported FORMAT values:

)");

    const int columns = 5;
    auto formats = p->factory.get_formats();
    int max_y = (formats.size() + columns - 1) / columns;
    for (auto y : util::range(max_y))
    {
        for (auto x : util::range(columns))
        {
            size_t i = x * max_y + y;
            if (i < formats.size())
            {
                auto &format = formats[i];
                Log.info(util::format("- %-14s", format.c_str()));
            }
        }
        Log.info("\n");
    }
    Log.info("\n");
}

void ArcUnpacker::unpack(
    Transformer &transformer, File &file, const std::string &base_name) const
{
    FileSaverHdd file_saver(p->options.output_dir, p->options.overwrite);
    unpack(transformer, file, base_name, file_saver);
}

void ArcUnpacker::unpack(
    Transformer &transformer,
    File &file,
    const std::string &base_name,
    FileSaver &file_saver) const
{
    FileSaverCallback file_saver_proxy([&](std::shared_ptr<File> saved_file)
    {
        saved_file->name =
            FileNameDecorator::decorate(
                transformer.get_file_naming_strategy(),
                base_name,
                saved_file->name);
        file_saver.save(saved_file);
    });

    transformer.parse_cli_options(p->arg_parser);
    transformer.unpack(file, file_saver_proxy);
}

std::unique_ptr<Transformer> ArcUnpacker::guess_transformer(File &file) const
{
    std::vector<std::unique_ptr<Transformer>> transformers;

    size_t max_format_size = 0;
    for (auto &format : p->factory.get_formats())
        if (format.size() > max_format_size)
            max_format_size = format.size();

    for (auto &format : p->factory.get_formats())
    {
        auto current_transformer = p->factory.create(format);
        Log.info(
            util::format(
                util::format("Trying %%-%ds", max_format_size + 2).c_str(),
                (format + ": ").c_str()));

        if (current_transformer->is_recognized(file))
        {
            Log.info("recognized\n");
            transformers.push_back(std::move(current_transformer));
        }
        else
        {
            Log.info("not recognized\n");
        }
    }

    if (transformers.size() == 0)
    {
        Log.info("File not recognized.\n");
        return nullptr;
    }
    else if (transformers.size() == 1)
    {
        return std::move(transformers[0]);
    }
    else
    {
        Log.info("File was recognized by multiple engines.\n");
        Log.info("Please provide --fmt and proceed manually.\n");
        return nullptr;
    }
}

bool ArcUnpacker::guess_transformer_and_unpack(
    File &file, const std::string &base_name) const
{
    std::unique_ptr<Transformer> transformer(nullptr);

    if (p->options.format == "")
    {
        transformer = guess_transformer(file);
        if (transformer == nullptr)
            return false;
    }
    else
    {
        transformer = p->factory.create(p->options.format);
    }

    Log.info("Unpacking...\n");
    try
    {
        unpack(*transformer, file, base_name);
        Log.info("Unpacking finished successfully.\n");
        return true;
    }
    catch (std::exception &e)
    {
        Log.info("Error: " + std::string(e.what()) + "\n");
        Log.info("Unpacking finished with errors.\n");
        return false;
    }
}

bool ArcUnpacker::run()
{
    if (should_show_help(p->arg_parser))
    {
        auto path_to_self = p->arg_parser.get_stray()[0];
        print_help(path_to_self);
        return true;
    }
    else
    {
        if (!add_input_paths_option(
            *this, p->arg_parser, p->options))
        {
            return false;
        }

        bool result = true;
        for (auto &path_info : p->options.input_paths)
        {
            File file(path_info->input_path, io::FileMode::Read);

            auto tmp = boost::filesystem::path(path_info->base_name);
            std::string base_name
                = tmp.stem().string() + "~" + tmp.extension().string();

            result &= guess_transformer_and_unpack(file, base_name);
        }
        return result;
    }
}
