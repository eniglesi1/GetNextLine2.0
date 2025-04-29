/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eiglesia <eiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:00:59 by eiglesia          #+#    #+#             */
/*   Updated: 2025/04/29 19:20:38 by eiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_list	*ft_newnode(int fd)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->fd = fd;
	new->content = NULL;
	new->next = NULL;
	new->prev = NULL;
	
}

int	ft_isused_fd(t_list *gnl, int fd)
{
	while (gnl != NULL)
	{
		if (fd == gnl->fd)
			return (1);
		gnl = gnl->next;
	}
	return (0);
}

char *get_next_line(int fd)
{
	static t_list	*gnl;
	char			*line;
	int				bytes_readed;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = NULL;
	gnl = NULL;
	bytes_readed = 0;
	if (ft_isused_fd(gnl, fd) == 0)
	{
		gnl = ft_newnode(fd);
		if (gnl = NULL)
			return (NULL);
	}
	else
	{
		while (gnl->fd != fd)
			gnl = gnl->next;
	}
	if (gnl->content == NULL)
		bytes_readed = read_from_file(gnl);
	line = extract_line(gnl);
	return (line);
}