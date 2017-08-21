#include <iostream>
#include <webhdfspp/webhdfspp.h>

using namespace webhdfspp;
using namespace std::placeholders;

static size_t print(const char *data, size_t size, size_t *read_bytes) {
  std::cout << std::string(data, size);
  *read_bytes = size;
  return size;
}

int main(int, char *argv[]) {
  Options options;
  options.namenodes.push_back(std::make_pair(argv[1], 50070));
  FileSystem *fs = nullptr;
  std::shared_ptr<IoService> io_service(IoService::New());
  Status err = FileSystem::New(options, io_service, &fs);
  if (!err.ok()) {
    std::cerr << "Cannot create the filesystem: " << err.ToString()
              << std::endl;
    return 1;
  }

  std::string path(argv[2]);
  FileStatus stat;
  err = fs->GetFileStatus(path, &stat);
  if (!err.ok()) {
    std::cerr << "GetFileStatus Error: " << err.ToString() << std::endl;
    return 1;
  }
  std::unique_ptr<InputStream> is;
  err = fs->Open(path, &is);
  if (!err.ok()) {
    std::cerr << "Cannot open the file:" << err.ToString() << std::endl;
    return 1;
  }

  size_t offset = 0;
  while (offset < stat.length) {
    size_t read_bytes = 0;
    err = is->PositionRead(InputStream::kUnlimitedReadBytes, offset,
                           std::bind(print, _1, _2, &read_bytes));
    offset += read_bytes;
  }
  delete fs;
  return 0;
}