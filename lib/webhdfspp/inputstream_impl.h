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

#ifndef LIB_WEBHDFSPP_INPUTSTREAM_IMPL_H_
#define LIB_WEBHDFSPP_INPUTSTREAM_IMPL_H_

#include "io_service_impl.h"

namespace webhdfspp {

class InputStreamImpl : public InputStream {
public:
  InputStreamImpl(const std::pair<std::string, short> &nn,
                  const std::string &path,
                  std::shared_ptr<IoServiceImpl> io_service);
  virtual Status PositionRead(size_t max_read_bytes, size_t offset,
                              const std::function<size_t(const char *, size_t)>
                                  &on_data_arrived) override;

private:
  std::pair<std::string, short> nn_;
  std::string path_;
  std::shared_ptr<IoServiceImpl> io_service_;
};

} // namespace webhdfspp

#endif