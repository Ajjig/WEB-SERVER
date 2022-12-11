/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   res.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roudouch <roudouch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 19:14:31 by roudouch          #+#    #+#             */
/*   Updated: 2022/12/11 23:21:44 by roudouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./res.hpp"

bool _is_exist(const std::string& name) {
    bool ret;
    
    std::ifstream f(name.c_str());
    ret = f.good();
    f.close();
    
    return ret;
}

// constructors and destructors
Respond::Respond(Request &req) {

    // remove last '/' if exist
    if (req.get_path().back() == '/')
        req.set_path(req.get_path().substr(0, req.get_path().size() - 1));
    
    std::string path = ROOT_PATH + req.get_path();

    this->req = req;
    if (_is_exist(path)) {
        this->status_code = 200;
    } else {
        this->status_code = 404;
    }
    this->list_is_allowed = true;
    this->init_body();
}

// setters and getters
std::string Respond::get_status_code() {
    return std::to_string(this->status_code);
}

std::string Respond::get_header() {
    // make header string from map header and return it 
    // first line of header is status code and status message look like this: HTTP/1.1 200 OK
    std::string status_code = this->header["status_code"];
    std::string http_version = this->header["http_version"];

    std::string header = http_version + " " + status_code + " " + this->status_msg()[status_code] + "\r\n";
    // other lines of header
    for (std::map<std::string, std::string>::iterator it = this->header.begin(); it != this->header.end(); ++it) {
        if (it->first != "status_code" && it->first != "status_msg" && it->first != "http_version") {
            header += it->first + ": " + it->second + "\r\n";
        }
    }
    header += "\r\n";
    return header;
}

std::string Respond::get_body() {
    return this->body;
}

// methods

std::string Respond::get_response() {
    std::string response = this->get_header() + this->get_body();
    return response;
};

void Respond::Get() {
    this->init_header();
    this->init_body();
}

void Respond::init_header() {
    
    std::string status_code = this->get_status_code();
    std::string date = this->get_date();
    std::string content_length = std::to_string(this->content_length);
    std::string content_type = this->get_type()[this->req.get_type_file()];
    std::string connection = this->req.get_headers()["Connection"];
    std::string http_version = this->req.get_http_version();

    // set header map
    this->header["Date"] = date;
    this->header["Content-Length"] = content_length;
    this->header["Content-Type"] = content_type;
    this->header["Connection"] = connection;
    this->header["Server"] = "Webserv/1.0";
    this->header["http_version"] = http_version;
    this->header["status_code"] = status_code;
    
}

void Respond::init_body() {

    std::string path;

    if (this->status_code == 404) {
        path = "./srcs/default/404/404.html";
    } else {
        path = ROOT_PATH + this->req.get_path();
    }

    // check if file or directory
    struct stat path_stat;
    stat(path.c_str(), &path_stat);
    if (S_ISDIR(path_stat.st_mode)) {
        // if directory
        if (this->list_is_allowed) {
            this->list_dir(path);
        } else {
            this->body = "Error: directory listing is not allowed";
            this->content_length = this->body.size();
        }
    } else {
        s_file file = this->read_file(path.c_str());
        this->body = file.str;
        this->content_length = file.size;
    }
}

void Respond::list_dir(std::string path) {
    std::vector<std::string> files = GetDirFiles(path);
    std::string body = "<html><head><title>Index of " + this->req.get_path() + "</title></head><body><h1>Index of " + this->req.get_path() + "</h1><hr><pre>";
    for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it) {
        // create link to file or directory
        std::string link = std::string(path) + "/" + *it;
        // remove ROOT_PATH from path
        link.erase(0, std::string(ROOT_PATH).size());
        // check if file or directory
        struct stat path_stat;
        stat(std::string(path + "/" + *it).c_str(), &path_stat);
        
        if (S_ISDIR(path_stat.st_mode)) {
            // if directory
            // handle /. directories
            if (*it == ".")
                continue;
            body += "<a href=\"" + link + "/\">" + *it + "/</a><br>";
        } else {
            // if file
            body += "<a href=\"" + link + "\">" + *it + "</a><br>";
        }
    }
    body += "</pre><hr></body></html>";
    this->body = body;
    this->content_length = this->body.size();
}

s_file Respond::read_file(std::string filename) {
    FILE* file_stream = fopen(filename.c_str(), "rb");
    if (file_stream == NULL) {
        return (s_file){NULL, 0, false};
    }
    string file_str;
    size_t file_size;

        fseek(file_stream, 0, SEEK_END);
        long file_length = ftell(file_stream);
        rewind(file_stream);
        
        char* buffer = new char[file_length];
        file_size = fread(buffer, 1, file_length, file_stream);
        file_str = string(buffer, file_size);
        delete[] buffer;

    fclose(file_stream);
    
    return (s_file){file_str, file_size, true};
}



std::string Respond::get_date() {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S %Z", timeinfo);
    std::string str(buffer);

    return str;
}


std::map<std::string, std::string> Respond::get_type()
{
    std::map<std::string, std::string> types;

	types["aac"]      = "audio/aac";
	types["avi"]      = "video/x-msvideo";
	types["azw"]      = "application/vnd.amazon.ebook";
	types["bin"]      = "application/octet-stream";
	types["bz"]       = "application/x-bzip";
	types["bz2"]      = "application/x-bzip2";
	types["csh"]      = "application/x-csh";
	types["css"]      = "text/css";
	types["doc"]      = "application/msword";
	types["epub"]     = "application/epub+zip";
	types["gif"]      = "image/gif";
	types["htm"]      = "text/html";
	types["html"]     = "text/html";
	types["ico"]      = "image/x-icon";
	types["ics"]      = "text/calendar";
	types["csv"]      = "text/csv";
	types["arc"]      = "application/octet-stream";
	types["abw"]      = "application/x-abiword";
	types["jpeg"]     = "image/jpeg";
	types["jpg"]      = "image/jpeg";
	types["midi"]     = "audio/midi";
	types["mpeg"]     = "video/mpeg";
	types["mpkg"]     = "application/vnd.apple.installer+xml";
	types["odp"]      = "application/vnd.oasis.opendocument.presentation";
	types["json"]     = "application/json";
	types["mid"]      = "audio/midi";
	types["oga"]      = "audio/ogg";
	types["ogv"]      = "video/ogg";
	types["mp4"]      = "video/mp4";
	types["ogx"]      = "application/ogg";
	types["png"]      = "image/png";
	types["pdf"]      = "application/pdf";
	types["odt"]      = "application/vnd.oasis.opendocument.text";
	types["js"]       = "application/js";
	types["jar"]      = "Temporary Redirect";
	types["rar"]      = "application/x-rar-compressed";
	types["rtf"]      = "application/rtf";
	types["sh"]       = "application/x-sh";
	types["ppt"]      = "application/vnd.ms-powerpoint";
	types["ods"]      = "application/vnd.oasis.opendocument.spreadsheet";
	types["svg"]      = "image/svg+xml";
	types["tar"]      = "application/x-tar";
	types["tif"]      = "image/tiff";
	types["tiff"]     = "image/tiff";
	types["swf"]      = "application/x-shockwave-flash";
	types["txt"]      = "text/plain";
	types["vsd"]      = "application/vnd.visio";
	types["ttf"]      = "application/x-font-ttf";
	types["weba"]     = "audio/webm";
	types["wav"]      = "audio/x-wav";
	types["webm"]     = "video/webm";
	types["woff"]     = "application/x-font-woff";
	types["xhtml"]    = "application/xhtml+xml";
	types["xls"]      = "application/vnd.ms-excel";
	types["webp"]     = "image/webp";
	types["zip"]      = "application/zip";
	types["3gp"]      = "video/3gpp audio/3gpp";
	types["xml"]      = "application/xml";
	types["xul"]      = "application/vnd.mozilla.xul+xml";
	types["3g2"]      = "video/3gpp2 audio/3gpp2";
	types["7z"]       = "application/x-7z-compressed";

	return types;
}

std::map<std::string, std::string> Respond::status_msg()
{
    std::map<std::string, std::string> msgs;

    msgs[ "100" ] = "Continue";
    msgs[ "101" ] = "Switching Protocols";
    msgs[ "102" ] = "Processing";
    msgs[ "103" ] = "Checkpoint";

    msgs[ "200" ] = "OK";
    msgs[ "201" ] = "Created";
    msgs[ "202" ] = "Accepted";
    msgs[ "203" ] = "Non-Authoritative Information";
    msgs[ "204" ] = "No Content";
    msgs[ "205" ] = "Reset Content";
    msgs[ "206" ] = "Partial Content";
    msgs[ "207" ] = "Multi-Status";
    msgs[ "208" ] = "Already Reported";

    msgs[ "300" ] = "Multiple Choices";
    msgs[ "301" ] = "Moved Permanently";
    msgs[ "302" ] = "Found";
    msgs[ "303" ] = "See Other";
    msgs[ "304" ] = "Not Modified";
    msgs[ "305" ] = "Use Proxy";
    msgs[ "306" ] = "Switch Proxy";
    msgs[ "307" ] = "Temporary Redirect";
    msgs[ "308" ] = "Permanent Redirect";

    msgs[ "400" ] = "Bad Request";
    msgs[ "401" ] = "Unauthorized";
    msgs[ "402" ] = "Payment Required";
    msgs[ "403" ] = "Forbidden";
    msgs[ "404" ] = "Not Found";
    msgs[ "405" ] = "Method Not Allowed";
    msgs[ "406" ] = "Not Acceptable";
    msgs[ "407" ] = "Proxy Authentication Required";
    msgs[ "408" ] = "Request Timeout";
    msgs[ "409" ] = "Conflict";
    msgs[ "410" ] = "Gone";
    msgs[ "411" ] = "Length Required";
    msgs[ "412" ] = "Precondition Failed";
    msgs[ "413" ] = "Payload Too Large";
    msgs[ "414" ] = "URI Too Long";
    msgs[ "415" ] = "Unsupported Media Type";
    msgs[ "416" ] = "Requested Range Not Satisfiable";
    msgs[ "417" ] = "Expectation Failed";
    msgs[ "418" ] = "I'm a teapot";
    msgs[ "421" ] = "Misdirected Request";
    msgs[ "422" ] = "Unprocessable Entity";
    msgs[ "423" ] = "Locked";
    msgs[ "424" ] = "Failed Dependency";
    msgs[ "426" ] = "Upgrade Required";
    msgs[ "428" ] = "Precondition Required";
    msgs[ "429" ] = "Too Many Request";
    msgs[ "431" ] = "Request Header Fields Too Large";
    msgs[ "451" ] = "Unavailable For Legal Reasons";

    msgs[ "500" ] = "Internal Server Error";
    msgs[ "501" ] = "Not Implemented";
    msgs[ "502" ] = "Bad Gateway";
    msgs[ "503" ] = "Service Unavailable";
    msgs[ "504" ] = "Gateway Timeout";
    msgs[ "505" ] = "HTTP Version Not Supported";
    msgs[ "506" ] = "Variant Also Negotiates";
    msgs[ "507" ] = "Insufficient Storage";
    msgs[ "508" ] = "Loop Detected";
    msgs[ "510" ] = "Not Extended";
    msgs[ "511" ] = "Network Authentication Required";

    return msgs;
}