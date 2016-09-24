#include "requesthandler.h"

RequestHandler::RequestHandler(const std::string& dir) : root_dir(dir) {}

void RequestHandler::handle(const std::string& requests, std::function<void (const std::string&)> writeCallback ) {
    std::stringstream ss(requests);
    std::string met, url, prot;
    ss>>met>>url>>prot;
    url = url_decode(url);

    if (met == "GET") {
    GET(url, prot, writeCallback);
    } else if (met == "HEAD") {
    HEAD(url, prot, writeCallback);
    } else if (met == "POST" || met == "PUT" || met == "PATCH" ||
    met == "DELETE" || met == "TRACE" || met == "CONNECT" ||
    met == "OPTIONS") {
    NotAllowed(prot, writeCallback);
    } else {
    NotImplemented(prot, writeCallback);
    }
}
std::string RequestHandler::url_decode(const std::string &url) {
      std::string res;

      for (size_t i = 0; i < url.length(); ++i) {
      if (url[i] == '%') {
      int val;
      sscanf(url.substr(i + 1, 2).c_str(), "%x", &val);
      res += (char)val;
      i += 2;
      } else if (url[i] == '+') {
      res += ' ';
      } else {
      res += url[i];
      }
      }
      size_t pos;
      while ((pos = res.find("/../")) != std::string::npos) {
      res.erase(pos, 4);
      }
      if ((pos = res.find('?')) != std::string::npos) {
      res = res.substr(0, pos);
      }
      return res;
      }

std::string RequestHandler::message_headers() {
    std::ostringstream headers;
    headers << "Server: Srver01\r\n"
            << "Date: " << QDateTime::currentDateTimeUtc().toString("ddd, dd MMM yyyy hh:mm:ss").toStdString() << " GMT\r\n"
            << "Connection: close\r\n";
    return headers.str();
}

void RequestHandler::GET(const std::string &url, const std::string &protocol,
         std::function<void (const std::string&)> writeCallback){
    std::string path = root_dir + url;
    bool dir;
    if ((dir = is_directory(path))) {
    if (path.back() != '/') path += '/';
    path += "index.html";
       }

    std::string headers(message_headers());
    if(file_exists(path)){
        std::string ext = dir ? "html" : get_extension(path);
        headers+=file_headers(file_size(path), ext);
        writeCallback(build_headers(protocol, get_code(200), headers));
        char buffer[4096];
        std::fstream fs(path);
        while (size_t count = (size_t)fs.readsome(buffer, 4096)) {
            writeCallback(std::string(buffer, count));
        }
    } else {
        headers+=file_headers(strlen(dir ? forbidden : not_found), "html");
        writeCallback(build_headers(protocol, get_code(dir ? 403:404), headers));
        writeCallback(dir ? forbidden: not_found);
    }

}


void RequestHandler::HEAD(const std::string &url, const std::string &protocol,
        std::function<void (const std::string&)> writeCallback){

}

bool RequestHandler::is_directory(const std::string& path) {
struct stat s;
return (stat(path.c_str(), &s) == 0) && (s.st_mode & S_IFDIR);
}


void RequestHandler::NotAllowed(const std::string &protocol,
        std::function<void (const std::string&)> writeCallback){

}


void RequestHandler::NotImplemented(const std::string &protocol,
        std::function<void (const std::string&)> writeCallback){

}

bool RequestHandler::file_exists(const std::string &path) {
std::ifstream in(path);
return in.good();
}


std::string RequestHandler::get_extension(const std::string &url) {
size_t pos = url.find_last_of('.');
return (pos != std::string::npos) ? url.substr(pos + 1) : "txt";
}


size_t RequestHandler::file_size(const std::string& path) {
std::ifstream in(path, std::ifstream::ate | std::ifstream::binary);
return (size_t)in.tellg();
}

std::string RequestHandler::file_headers(size_t length, const std::string& ext){
std::ostringstream headers;
headers << "Content-Length: "<< length << "\r\n"
<< "Content-Type: " << ext_to_mime[ext]<< "\r\n";
return headers.str();
}


std::string RequestHandler::build_headers(const std::string &protocol, const std::string &code,
const std::string &headers) {
std::stringstream response;
response << protocol << ' ' << code << "\r\n" << headers << "\r\n";
return response.str();
}

std::string RequestHandler::get_code(int code) {
switch (code) {
case 200:
return "200 OK";
case 403:
return "403 Forbidden";
case 404:
return "404 Not Found";
case 405:
return "405 Method Not Allowed";
case 501:
return "501 Not Implemented";
default:
return "";
}
}
