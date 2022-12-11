/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roudouch <roudouch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 17:18:58 by roudouch          #+#    #+#             */
/*   Updated: 2022/12/11 17:19:05 by roudouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dir.hpp"

std::vector<std::string> GetDirFiles(std::string path)
{
    std::vector<std::string> files;
    struct dirent *entry;
	DIR *dir = opendir(path.c_str());

	if (dir == NULL) 
	{
	  return files;
	}
	while ((entry = readdir(dir)) != NULL) 
	{
		files.push_back(entry->d_name);
	}
	closedir(dir);
	
	return files;
}