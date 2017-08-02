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

#include "io_service_impl.h"

#include <atomic>
#include <sstream>

namespace webhdfspp {

IoServiceImpl::IoServiceImpl() {}

IoServiceImpl::~IoServiceImpl() {}

IoService *IoService::New() { return new IoServiceImpl(); }

IoService::~IoService() {}

Status IoServiceImpl::Run() { return Status::OK(); }

void IoServiceImpl::Stop() {}

Status IoServiceImpl::DoNNRequest(const URIBuilder &uri,
                                  const std::string &method,
                                  rapidjson::Document *d) {
  CURL *handle = curl_easy_init();

  std::vector<char> content;
  char error_buffer[CURL_ERROR_SIZE];
  auto uri_str = uri.Build();

  curl_easy_setopt(handle, CURLOPT_URL, uri_str.c_str());
  curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, error_buffer);
  curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, method.c_str());
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION,
                   &IoServiceImpl::AppendToBuffer);
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, &content);
  CURLcode err = curl_easy_perform(handle);
  curl_easy_cleanup(handle);

  if (err) {
    return Status::Error(error_buffer);
  } else if (d->Parse(&content.front(), content.size()).HasParseError()) {
    return Status::InvalidArgument("Parse error");
  } else if (d->HasMember("RemoteException")) {
    const auto &exc = (*d)["RemoteException"];
    return Status::Exception(exc["javaClassName"].GetString(),
                             exc["message"].GetString());
  }

  return Status::OK();
}

Status IoServiceImpl::DoDNGet(
    const URIBuilder &uri,
    const std::function<size_t(const char *, size_t)> &on_data_arrived) {
  CURL *handle = curl_easy_init();

  char error_buffer[CURL_ERROR_SIZE];
  auto uri_str = uri.Build();

  curl_easy_setopt(handle, CURLOPT_URL, uri_str.c_str());
  curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, error_buffer);
  curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "GET");
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION,
                   &IoServiceImpl::DNGetCallback);
  curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, true);
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, &on_data_arrived);
  CURLcode err = curl_easy_perform(handle);
  curl_easy_cleanup(handle);

  if (err) {
    return Status::Error(error_buffer);
  }

  return Status::OK();
}

CURLcode IoServiceImpl::AppendToBuffer(char *in, size_t size, size_t nmemb,
                                       std::vector<char> *buf) {
  buf->insert(buf->end(), in, in + size * nmemb);
  return (CURLcode)(size * nmemb);
}

CURLcode IoServiceImpl::DNGetCallback(
    char *in, size_t size, size_t nmemb,
    std::function<size_t(const char *, size_t)> *on_data_arrived) {
  size_t consumed = (*on_data_arrived)(in, size * nmemb);
  return (CURLcode)consumed;
}

} // namespace webhdfspp