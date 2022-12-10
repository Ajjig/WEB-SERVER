/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roudouch <roudouch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 12:08:04 by roudouch          #+#    #+#             */
/*   Updated: 2022/12/09 19:02:26 by roudouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <string>
# include <vector>
# include <map>
# include <iostream>
# include "../../include/header.hpp"

    class cgi {
        public:
            cgi(request *req);
    };

#endif