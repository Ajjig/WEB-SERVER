/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   req.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roudouch <roudouch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 16:02:05 by roudouch          #+#    #+#             */
/*   Updated: 2022/12/16 17:11:29 by roudouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../include/header.hpp"

class Request {
    private:
        std::string                         method;
        std::string                         uri;
        std::string                         path;
        std::string                         http_version;
        int                                 respond_status;
        std::map<std::string, std::string>  query;
        std::map<std::string, std::string>  headers;
        std::string                         file_name;
        std::string                         file_type;
        Server                              server;

    public:
        // constructors and destructors
        Request(){};
        //Request(std::string request);
        Request(std::string request, Server server);
        ~Request(){}
        
        // setters and getters
        Server                              &get_server(void);
        std::map<std::string, std::string>  get_headers(void);
        std::string                         &get_method(void);
        std::string                         &get_uri(void);
        std::string                         &get_http_version(void);
        int                                 get_respond_status(void);
        void                                set_headers(std::map<std::string, std::string> headers);
        std::string                         &get_path(void);
        // methods
        void                                parse_request(std::string request);
        std::string                         get_type_file(void);
        std::string                         get_file_name(void);
        void                                set_path(std::string path);
};


//GET /images/favicon.ico HTTP/1.1

//Host:              localhost
//User-Agent:        Mozilla/5.0 (Macintosh; Intel Mac OS X 10.14; rv:94.0) Gecko/20100101 Firefox/94.0
//Accept:            image/avif,image/webp,*/*
//Accept-Language:   en-US,en;q=0.5
//Accept-Encoding:   gzip, deflate
//Connection:        keep-alive
//Referer:           http://localhost/
//Cookie:            wp-settings-time-1=1668421195; wp-settings-1=libraryContent%3Dbrowse; connect.sid=s%3AAxTqshG3XtGmWG2l20pEoguEuPU_qbWo.UAnC6aJKO2PkMDLgmxyM0Knisn0ATGmv3qRAtyj1vaE; wordpress_test_cookie=WP+Cookie+check; wordpress_logged_in_0a0611b42c0de0ad85d9791f5fe3c9a6=rasheed%7C1668590739%7C1oGuEtIXQCAD3zdiSjOzQrF93tAXFFRd4WVzeGwzRZ5%7C73d5776c82a4028ff489101565eba7322699061fb2f677055b21a8668d63da76; wordpress_logged_in_86a9106ae65537651a8e456835b316ab=roudouch%7C1669546208%7C7fckdZ7EVZPpx5bDWqm9QmZS15bY2E2FMyVWXqMUWMh%7C359ff424397828c51a164507a29695bdac87e2d4a2b533911c7358cf67d671de; wp_lang=en_US
//Sec-Fetch-Dest:    image
//Sec-Fetch-Mode:    no-cors
//Sec-Fetch-Site:    same-origin
