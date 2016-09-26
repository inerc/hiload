#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <string>
#include <sstream>
#include <QDateTime>
#include <fstream>
#include <sys/stat.h>
#include <QLocale>

class RequestHandler
{
    std::map<std::string, std::string> ext_to_mime =
    {
    { "txt", "text/plain" },
    { "html", "text/html" },
    { "css", "text/css" },
    { "js", "application/javascript" },
    { "jpg", "image/jpeg" },
    { "jpeg", "image/jpeg" },
    { "png", "image/png" },
    { "gif", "image/gif" },
    { "swf", "application/x-shockwave-flash" }
    };

    const char* not_found =
    "<html>"
    "<head><title>Not Found</title></head>"
    "<body><h1>404 Not Found</h1></body>"
    "</html>";
    const char* forbidden =
    "<html>"
    "<head><title>Forbidden</title></head>"
    "<body><h1>403 Forbidden</h1></body>"
    "</html>";
    const char* not_allowed =
    "<html>"
    "<head><title>Not Allowed</title></head>"
    "<body><h1>404 Method Not Allowed</h1></body>"
    "</html>";
    const char* not_implemented =
    "<html>"
    "<head><title>Not Implemented</title></head>"
    "<body><h1>501 Not Implemented</h1></body>"
    "</html>";

    std::string root_dir;

    std::string url_decode(const std::string &url);
    void GET(const std::string &url, const std::string &protocol,
    std::function<void (const std::string&)> writeCallback);
    void HEAD(const std::string &url, const std::string &protocol,
    std::function<void (const std::string&)> writeCallback);
    void NotAllowed(const std::string &protocol,
    std::function<void (const std::string&)> writeCallback);
    void NotImplemented(const std::string &protocol,
    std::function<void (const std::string&)> writeCallback);
    bool is_directory(const std::string& path);
    bool file_exists(const std::string &path);
    std::string message_headers();
    std::string get_extension(const std::string &url);
    size_t file_size(const std::string& path);
    std::string file_headers(size_t length, const std::string& ext);
    std::string build_headers(const std::string &protocol, const std::string &code,
    const std::string &headers);
    std::string get_code(int code);
public:
    RequestHandler(const std::string& dir);
    void handle(const std::string& requests, std::function<void (const std::string&)> writeCallback );
};

#endif // REQUESTHANDLER_H
