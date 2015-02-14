#include <cassert>
#include "file_io.h"
#include "fs.h"
#include "logger.h"
#include "output_files.h"

struct OutputFilesHdd::Internals
{
    std::string output_dir;

    std::string get_full_path(
        const std::string output_dir, const std::string file_name)
    {
        assert(file_name != "");
        if (output_dir == "")
            return file_name;
        return output_dir + "/" + file_name;
    }
};

OutputFilesHdd::OutputFilesHdd(std::string output_dir)
    : internals(new Internals)
{
    internals->output_dir = output_dir;
}

OutputFilesHdd::~OutputFilesHdd()
{
}

void OutputFilesHdd::save(VFFactory save_proc) const
{
    try
    {
        assert(save_proc != nullptr);

        log("Reading... ");

        std::unique_ptr<VirtualFile> file(save_proc());
        assert(file != nullptr);

        std::string full_path = internals->get_full_path(
            internals->output_dir, file->name);
        assert(full_path != "");

        mkpath(dirname(full_path));

        FileIO output_io(full_path, "wb");
        file->io.seek(0);
        output_io.write_from_io(file->io, file->io.size());
        log("ok (saved in %s)\n", full_path.c_str());
    }
    catch (std::runtime_error &e)
    {
        log("error (%s)\n", e.what());
    }
}



struct OutputFilesMemory::Internals
{
    std::vector<std::unique_ptr<VirtualFile>> files;
};

OutputFilesMemory::OutputFilesMemory() : internals(new Internals)
{
}

OutputFilesMemory::~OutputFilesMemory()
{
}

const std::vector<VirtualFile*> OutputFilesMemory::get_saved() const
{
    std::vector<VirtualFile*> files;
    for (std::unique_ptr<VirtualFile>& f : internals->files)
        files.push_back(f.get());
    return files;
}

void OutputFilesMemory::save(VFFactory save_proc) const
{
    assert(save_proc != nullptr);
    std::unique_ptr<VirtualFile> file(save_proc());
    assert(file != nullptr);
    internals->files.push_back(std::move(file));
}