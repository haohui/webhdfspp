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

#ifndef LIB_WEBHDFSPP_IO_SERVICE_IMPL_H_
#define LIB_WEBHDFSPP_IO_SERVICE_IMPL_H_

#include "uri_builder.h"
#include "webhdfspp/webhdfspp.h"
#include <curl/curl.h>
#include <rapidjson/document.h>

namespace webhdfspp {

class IoServiceImpl : public IoService,
                      public std::enable_shared_from_this<IoServiceImpl> {
public:
  IoServiceImpl();
  ~IoServiceImpl();
  virtual Status Run() override;
  virtual void Stop() override;

  Status DoNNRequest(const URIBuilder &uri, const std::string &method,
                     rapidjson::Document *doc);
  Status
  DoDNGet(const URIBuilder &uri,
          const std::function<size_t(const char *, size_t)> &on_data_arrived);

private:
  static CURLcode AppendToBuffer(char *in, size_t size, size_t nmemb,
                                 std::vector<char> *buf);
  static CURLcode
  DNGetCallback(char *in, size_t size, size_t nmemb,
                std::function<size_t(const char *, size_t)> *on_data_arrived);
};
} // namespace webhdfspp

#endif