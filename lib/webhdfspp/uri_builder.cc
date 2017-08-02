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

#include "uri_builder.h"

#include <sstream>

namespace webhdfspp {

URIBuilder &URIBuilder::Scheme(const std::string &scheme) {
  scheme_ = scheme;
  return *this;
}

URIBuilder &URIBuilder::Host(const std::string &host) {
  host_ = host;
  return *this;
}

URIBuilder &URIBuilder::Path(const std::string &path) {
  path_ = path;
  return *this;
}

URIBuilder &URIBuilder::Port(unsigned short port) {
  port_ = port;
  return *this;
}

URIBuilder &URIBuilder::Param(const std::string &key,
                              const std::string &value) {
  // TODO: Escape the value
  query_params_.push_back(key + "=" + value);
  return *this;
}

std::string URIBuilder::Build() const {
  std::stringstream ss;
  ss << scheme_ << "://" << host_ << ":" << port_ << path_;
  if (query_params_.size()) {
    ss << "?";
    ss << query_params_.front();
    for (unsigned i = 1; i < query_params_.size(); ++i) {
      ss << "&" << query_params_[i];
    }
  }
  return ss.str();
}

} // namespace webhdfspp