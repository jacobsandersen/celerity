#include "HttpClient.h"

#include <curl/curl.h>

#include <iostream>
#include <sstream>

namespace celerity::net {
static size_t write_body(void* contents, size_t size, size_t nmemb, std::string* s) {
  s->append(static_cast<char*>(contents), size * nmemb);
  return size * nmemb;
}

bool HttpClient::get_url(const string_view& url, const map<string, string>& query_params, std::string* resp_body,
                         int64_t* resp_code) {
  CURL* curl = curl_easy_init();
  if (!curl) {
    return false;
  }

  std::stringstream ss;
  ss << url;

  bool first = true;
  for (const auto& entry : query_params) {
    if (first) {
      ss << "?" << entry.first << "=" << entry.second;
      first = false;
    } else {
      ss << "&" << entry.first << "=" << entry.second;
    }
  }

  CURLcode res;
  curl_easy_setopt(curl, CURLOPT_URL, ss.str().c_str());

  if (resp_body != nullptr) {
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_body);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp_body);
  }

  res = curl_easy_perform(curl);

  if (resp_code != nullptr) {
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, resp_code);
  }

  curl_easy_cleanup(curl);

  return res == CURLE_OK;
}
}  // namespace celerity::net