#ifndef CELERITY_NET_HTTPCLIENT_H
#define CELERITY_NET_HTTPCLIENT_H

#include <map>
#include <string>

using std::string, std::string_view, std::map;

namespace celerity::net {
class HttpClient {
 public:
  static bool get_url(const string_view&, const map<string, string>&, std::string*, int64_t*);
};
}  // namespace celerity::net

#endif
