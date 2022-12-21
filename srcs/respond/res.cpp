/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   res.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roudouch <roudouch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 19:14:31 by roudouch          #+#    #+#             */
/*   Updated: 2022/12/21 22:46:28 by roudouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/header.hpp"

bool _exist(const std::string& name) {
    bool ret;

    std::ifstream f(name.c_str()); // open file
    ret = f.good(); // check if file is open
    f.close(); // close file
    return ret;
}

bool compare_paths(std::string path, std::string location) {

    // if there is no / at the end of the location, add it
    if (location[location.length() - 1] != '/') {
        location += '/';
    }

    //std::cout << "===> path: {" << path << "}==> location: {" << location << "}" << std::endl;
        if (path == location) {
        return true;
    }

    // Split the path into a vector
    std::vector<std::string> pathVector;
    std::string temp = "";
    for (size_t i = 0; i < path.length(); i++) {
        if (path[i] == '/') {
            pathVector.push_back(temp);
            temp = "";
        } else {
            temp += path[i];
        }
    }

    // Split the location into a vector
    std::vector<std::string> locationVector;
    temp = "";
    for (size_t i = 0; i < location.length(); i++) {
        if (location[i] == '/') {
            locationVector.push_back(temp);
            temp = "";
        } else {
            temp += location[i];
        }
    }
    
    // Compare the vectors
    if (pathVector.size() < locationVector.size()) {
        return false;
    }

    for (size_t i = 0; i < locationVector.size() || i < pathVector.size(); i++) {
        if (pathVector[i] != locationVector[i]) {
            return false;
        }
    }

    return true;
}

bool Respond::set_location() {
    // check if there is a location that match the path
    std::vector<Location> locations = req.get_server().getLocations();
    this->l_path = "";
    
    for (size_t i = 0; i < locations.size(); i++) {
        if (compare_paths(this->req.get_path(), req.get_server().getLocationPaths()[i])) {
            this->location = locations[i];
            this->l_path = req.get_server().getLocationPaths()[i];
            return true;
        }
    }
    return false;

}

// constructors and destructors
Respond::Respond(Request &req) {

    std::cout << "\n=============================respond==============================\n\n";
    
    // print request header 
    std::cout << req.get_header_as_string() << std::endl;
    
    this->req = req;
    bool is_match = set_location();

    //std::cout << "\n==========info location===========\n" << std::endl;
    //std::cout << "there is a location: " << is_match << std::endl;
    //std::cout << "location_path: " << l_path << std::endl;
    //std::cout << "is_allowd: " << this->location.isAllowed("GET") << std::endl;
    //std::cout << "Root: " << this->location.getRoot() << std::endl;
    //std::cout << "\n==================================\n" << std::endl;

    // check http version if it's not 1.1 return 505
    if (this->req.get_http_version() != "HTTP/1.1") {
        
        this->status_code = 505;
        std::string code = std::to_string(this->status_code);
        this->default_page_error(code, this->status_msg()[code]);
        
    }
    else if (is_match && this->l_path == "/cgi-bin") {
        std::cout << "\n============= cgi =============" << this->req.get_uri() << std::endl;
        try {
    
            Cgi cgi(this->req.get_server(), this->req.get_uri());
            std::string res = cgi.get_body();
            this->status_code = cgi.get_status_code();

            if (this->status_code == 200) {
                this->body = res;
                this->content_length = this->body.size();
                this->content_type = "text/html";
                init_header();
            } else if (this->status_code == 404) {
                std::string code = std::to_string(this->status_code);
                this->default_page_error(code, this->status_msg()[code]);
            }
            
            
        } catch (std::exception &e) {
            std::string error = ("\033[101m" + std::string(e.what()) + " \033[49m\033[39m");
            perror(error.c_str());
            this->status_code = 500;
            std::string code = std::to_string(this->status_code);
            this->default_page_error(code, this->status_msg()[code]);
        }
        std::cout << "\n============= end =============\n" << std::endl;
    }
    else {
        
        if (not is_match) {

            this->status_code = 404;
            std::string code = std::to_string(this->status_code);
            this->default_page_error(code, this->status_msg()[code]);

        } else {
            this->ROOT_PATH = this->location.getRoot();
            this->list_is_allowed = this->location.isAutoindex();
            this->status_code = 200;
            
            try {
                if (!this->is_allowed_method(this->req.get_method())) {
                    this->status_code = 405;
                    std::string code = std::to_string(this->status_code);
                    this->default_page_error(code, this->status_msg()[code]);
                    return;
                }
                else if (this->req.get_method() == "GET") {
                    this->Get();
                }
                else if (this->req.get_method() == "POST") {
                    this->Post();
                }
                else if (this->req.get_method() == "DELETE") {
                    this->Delete();
                }
                else {
                    this->status_code = 405;
                    std::string code = std::to_string(this->status_code);
                    this->default_page_error(code, this->status_msg()[code]);
                }
                //else if (this->req.get_method() == "DELETE") {
                //    this->Delete();   
                //}
            } catch (std::exception &e) {
                std::cout << "\033[91mERROR: " << e.what() << "\n\033[39m" << std::endl;
                this->status_code = 500;
                std::string code = std::to_string(this->status_code);
                this->default_page_error(code, this->status_msg()[code]);
            }
        }
    }
    this->logs();
    std::cout << "\n===============================end===============================\n";
}

void Respond::Delete() {
    std::string path = this->ROOT_PATH;
    
    // get file name from path
    std::string file_name = this->req.get_path();
    std::string::size_type pos = file_name.find_last_of('/');
    if (pos != std::string::npos) {
        file_name.erase(0, pos + 1);
    }
    path += file_name;

    std::cout << "path: " << path << std::endl;
    
    if (remove(path.c_str()) != 0) {
        this->status_code = 404;
    } else {
        this->status_code = 200;
    }
    std::string code = std::to_string(this->status_code);
    this->default_page_error(code, this->status_msg()[code]);
}

void Respond::Post() {
    // get file name from path
    std::string file_name = this->req.get_path();
    std::string::size_type pos = file_name.find_last_of('/');
    if (
        pos != std::string::npos ||
        static_cast<size_t>(std::stoi(this->req.get_headers()["Content-Length"])) != this->req.get_body().size() ||
        this->req.get_body().empty())
    {
        file_name.erase(0, pos + 1);
        // create file for body of request
        std::string path = this->ROOT_PATH + file_name;
        std::ofstream file(path, std::ios::out | std::ios::trunc);
        
        if (file) {
            file << this->req.get_body();
            file.close();
            this->status_code = 200;
            std::string code = std::to_string(this->status_code);
            this->default_page_error(code, this->status_msg()[code]);
        } else {
            // server error
            this->status_code = 400;
            std::string code = std::to_string(this->status_code);
            this->default_page_error(code, this->status_msg()[code]);
        }
    }
    else {
        this->status_code = 400;
        std::string code = std::to_string(this->status_code);
        this->default_page_error(code, this->status_msg()[code]);
        return;
    }
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

    std::string header = this->get_header();

    //std::cout << "\n============= Header =============\n" << std::endl;
    //std::cout << header << std::endl;
    //std::cout << "============= end =============\n" << std::endl;
    

    std::string response = header + this->get_body();
    return response;
};

void Respond::Get() {
    this->init_body();
    this->init_header();
}

void Respond::init_header() {

    std::string status_code = this->get_status_code();
    std::string date = this->get_date();
    std::string content_length = std::to_string(this->content_length);
    std::string connection = this->req.get_headers()["Connection"];
    std::string http_version = this->req.get_http_version();

    // set header map
    this->header["Date"] = date;
    this->header["Content-Length"] = content_length;
    this->header["Content-Type"] = this->content_type;
    this->header["Connection"] = connection;
    this->header["Server"] = "Webserv/1.0";
    this->header["http_version"] = http_version;
    this->header["status_code"] = status_code;
    // add auto index
    if (this->list_is_allowed) {
        this->header["autoindex"] = "on";
    }
    if (this->location.isRedirect()) {
        this->header["Location"] = this->location.getRedirectUrl();
    }
}


void Respond::init_body() {

    std::string path;

    if (this->status_code == 404) {
        // if 404
        std::string code = std::to_string(this->status_code);
        this->default_page_error(code, this->status_msg()[code]);
        return;
        
    }
    // check is the location has redirect
    else if (this->location.isRedirect()) {
        this->status_code = this->location.getRedirectCode();
        std::string code = std::to_string(this->status_code);
        this->default_page_error(code, this->status_msg()[code]);
        init_header();
        return;
    }
    // if 200 get path
    path = this->ROOT_PATH;

    // get path of the request
    std::string req_path = this->req.get_path();

    // remove location path from request path
    std::string location = l_path;
    for (size_t i = 0; i < location.size(); i++) {
        req_path.erase(0, 1);
    }
    
    // add req path to root path
    // check if there is / at the start of req path if there is remove it
    if (req_path[0] == '/') {
        req_path.erase(0, 1);
    }
    //if (path[path.size() - 1] == '/') {
    //    path.erase(path.size() - 1, 1);
    //}
    path += req_path;

    // get extension of the file
    extension = path.substr(path.find_last_of("."), path.size());

    // check there is no extension
    std::cout << "path: " << path << std::endl;
    
    if (extension == path) {
        // set extension to empty
        extension = "";
    } else {
        // remove . from extension
        extension.erase(0, 1);
    }
    
    std::cout << "extension: " << extension << std::endl;
    
    // check if there is index file and it's exist
    bool file_exist = false;
    if (extension == "") {
        
        std::string index_path = "";
        std::string index = "";
        // check if there is index file
        for (size_t i = 0; i < this->location.getIndexes().size(); i++) {
            index = this->location.getIndexes()[i];
            // check if there is / in the end of the path
            if (path[path.size() - 1] == '/') {
                index_path = path + index;
            } else {
                index_path = path + "/" + index;
            }
            std::cout << "===============> index_path: " << index_path << std::endl;
            
            if (_exist(index_path.c_str())) {
                path = index_path;
                file_exist = true;
                break;
            }
        }
        // update extension
        extension = index_path.substr(index_path.find_last_of("."), index_path.size());
        // check if there is extension
        if (extension != path) {
            // remove . from extension
            extension.erase(0, 1);
            // set content type
            this->content_type = this->get_type()[extension];
        }

        // send redirect to index file
        if (file_exist) {
            this->status_code = 301;
            this->header["Location"] = index;
            return;
        }
        
    } else {
        // if there is extension
        if (_exist(path.c_str())) {
            file_exist = true;
            this->content_type = this->get_type()[extension];
        }
    }
    
    if (file_exist) {
        // if file exist
        s_file file = this->read_file(path.c_str());
        this->body = file.str;
        this->content_length = file.size;
    } else {
        // check if the path is directory
        struct stat sb;
        stat(path.c_str(), &sb);
        if (S_ISDIR(sb.st_mode)) {
            // if it's directory, check if it's allowed to list directory content or not
            if (this->list_is_allowed) {
                // if it's allowed to list directory content
                this->list_dir(path);
            } else {
                // if it's not allowed to list directory content, return 404
                this->status_code = 404;
                std::string code = std::to_string(this->status_code);
                this->default_page_error(code, this->status_msg()[code]);
            }
        } else {
            // if it's not directory, return 404
            this->status_code = 404;
            std::string code = std::to_string(this->status_code);
            this->default_page_error(code, this->status_msg()[code]);
        }
    }
}

void Respond::default_page_error(std::string code, std::string msg) {
    // read default template
    std::string default_template = this->read_file("./srcs/default/default.html").str;

    // replace code and msg in default template with the code and msg 
    // the code is ${code} and the msg is ${msg}
    while (1) {
        std::string::size_type pos = default_template.find("${code}");
        if (pos != std::string::npos) {
            default_template.replace(pos, 7, code);
        } else {
            break;
        }
    }
    while (1) {
       std::string::size_type pos = default_template.find("${msg}");
        if (pos != std::string::npos) {
            default_template.replace(pos, 6, msg);
        } else {
            break;
        }
    }
    this->body = default_template;
    this->content_length = this->body.size();
    this->content_type = "text/html";
    init_header();
}

bool Respond::is_allowed_method(std::string method) {
    return this->location.isAllowed(method);
}

void Respond::list_dir(std::string path) {
    std::vector<std::string> files = GetDirFiles(path);
    std::string body = "<html><head><title>Index of " + this->req.get_path() + "</title></head><body><h1>Index of " + this->req.get_path() + "</h1><hr><pre>";
    for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it) {
        // create link to file or directory
        // if there is a / in the end of the path
        std::string link = std::string(path);
        if (path[path.size() - 1] != '/') {
            link += "/" + *it;
        } else {
            link += *it;
        }
        // remove ROOT_PATH from path
        link.erase(0, std::string(this->location.getRoot()).size());
        
        link = this->l_path + "/" + link;
        // check if file or directory
        struct stat path_stat;
        stat(std::string(path + "/" + *it).c_str(), &path_stat);

        if (S_ISDIR(path_stat.st_mode)) {
            // if directory
            // handle /. directories
            if (*it == ".") {
                body += "<a href=\"/\">" + *it + "/</a><br>";
                continue;
            }
            else if (*it == "..") {
                body += "<a href=\"../\">" + *it + "/</a><br>";
                continue;
            }
            body += "<a href=\"" + link + "/\">" + *it + "/</a><br>";
        } else {
            // if file
            body += "<a href=\"" + link + "\">" + *it + "</a><br>";
        }
    }
    body += "</pre><hr></body></html>";
    this->body = body;
    this->content_length = body.size();
    this->content_type = "text/html";
}

s_file Respond::read_file(std::string filename) {
    FILE* file_stream = fopen(filename.c_str(), "rb");
    if (file_stream == NULL) {
        throw std::runtime_error("Error opening file"); // if file not found
    }
    string file_str;
    size_t file_size;
        fseek(file_stream, 0, SEEK_END); // Reposition stream position indicator to the end of the stream
        long file_length = ftell(file_stream); // return the number of bytes from the beginning of the file.
        rewind(file_stream); // Reposition stream position indicator to the beginning of the stream

        char* buffer = new char[file_length]; // Allocate memory for the entire content
        file_size = fread(buffer, 1, file_length, file_stream); // Read the file into the buffer
        file_str = string(buffer, file_size); // Copy buffer to string
        delete[] buffer; // Deallocate buffer memory
    fclose(file_stream);
    return (s_file){file_str, static_cast<int>(file_size), true};
}

void Respond::logs() {
    std::string method = this->req.get_method();
    std::string uri = "\e[0;37m" + this->req.get_path() + "\e[0m";
    std::string http_version = "\e[0;35m" + this->req.get_http_version() + "\e[0m";
    std::string respond_status = (this->status_code == 200 ? "\e[0;32m" : "\e[0;31m") + std::to_string(this->status_code) + "\e[0m";;
    std::string date = "\e[0;36m" + this->get_date() + "\e[0m";

    std::cout << ("\033[43m " +  method + " \e[0m : ") << respond_status << " " << uri << " " << http_version << " " << date << std::endl;
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
    msgs[ "520" ] = "Unknown Error";

    return msgs;
}