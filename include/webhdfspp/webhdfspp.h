/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef WEBHDFSPP_WEBHDFSPP_H_
#define WEBHDFSPP_WEBHDFSPP_H_

#include "status.h"

#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace webhdfspp {

class non_copyable {
protected:
  non_copyable() = default;
  non_copyable(non_copyable const &) = delete;
  void operator=(non_copyable const &) = delete;
};

class IoService : non_copyable {
public:
  static IoService *New();
  virtual Status Run() = 0;
  virtual void Stop() = 0;
  virtual ~IoService();
};

class InputStream;
class OutputStream;

struct Options {
  std::vector<std::pair<std::string, short>> namenodes;
};

struct FileStatus {
  enum FileType {
    FILE,
    DIRECTORY,
    SYMLINK,
  };
  unsigned long long accessTime;
  unsigned long long blockSize;
  unsigned long long childrenNum;
  unsigned long long fileId;
  std::string group;
  unsigned long long length;
  unsigned long long modificationTime;
  std::string owner;
  std::string pathSuffix;
  unsigned permission;
  unsigned replication;
  unsigned storagePolicy;
  FileType type;
};

class InputStream : non_copyable {
public:
  static const size_t kUnlimitedReadBytes = -1;
  virtual Status PositionRead(
      size_t max_read_bytes, size_t offset,
      const std::function<size_t(const char *, size_t)> &on_data_arrived) = 0;
  virtual ~InputStream();
};

class OutputStream : non_copyable {
public:
  virtual Status Append(void *buf, size_t nbyte, size_t written_bytes) = 0;
  virtual ~OutputStream();
};

class FileSystem : non_copyable {
public:
  virtual ~FileSystem();
  static Status New(const Options &options,
                    std::shared_ptr<IoService> io_service, FileSystem **fsptr);
  virtual Status Delete(const std::string &path, bool recursive) = 0;
  virtual Status GetFileStatus(const std::string &path,
                               FileStatus *fileStatus) = 0;
  virtual Status Exists(const std::string &path, bool *res) = 0;
  virtual Status Open(const std::string &path,
                      std::unique_ptr<InputStream> *isptr) = 0;
  virtual Status Create(const std::string &path, bool overwrite,
                        OutputStream **osptr) = 0;
};

} // namespace webhdfspp
#endif