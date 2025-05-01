/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eiglesia <eiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:00:59 by eiglesia          #+#    #+#             */
/*   Updated: 2025/05/01 02:16:12 by eiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <limits.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}
size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	a;

	if (!dst || !src)
		return (0);
	if (size == 0)
		return (ft_strlen(src));
	a = 0;
	while (a < (size - 1) && src[a] != '\0')
	{
		dst[a] = src[a];
		a++;
	}
	dst[a] = '\0';
	return (ft_strlen(src));
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	a;
	size_t	i;

	a = 0;
	i = 0;
	while (dst[a] != '\0' && a < size)
		a++;
	if (size > a)
		size = (size - a) - 1;
	else
		return (size + ft_strlen(src));
	while (size > 0 && src[i] != '\0')
	{
		dst[a] = src[i];
		a++;
		i++;
		size--;
	}
	dst[a] = '\0';
	return (ft_strlen(dst) + ft_strlen(src) - i);
}

void	ft_bzero(void *s, size_t n)
{
	while (n > 0)
	{
		n--;
		((unsigned char *)s)[n] = '\0';
	}
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*a;

	if (nmemb != 0 && (nmemb * size) / nmemb != size)
		return (NULL);
	a = malloc(nmemb * size);
	if (!a)
		return (NULL);
	ft_bzero(a, nmemb * size);
	return (a);
}

void	*ft_realloc(void *ptr, size_t size)
{
	char	*str;

	if (ptr == NULL)
		return (ft_calloc(size, sizeof(char)));
	if (size == 0)
		return (free(ptr), NULL);
	str = ft_calloc(size, sizeof(char));
	if (str == NULL)
		return (free(ptr), NULL);
	ft_strlcpy(str, ptr, size);
	return (free(ptr), str);
}

int	isline(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (0);
	while (str[i] && str[i] != '\n')
		i++;
	return (str[i] / 10);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (unsigned char)c)
			return ((char *)s);
		s++;
	}
	if (c == 0)
		return ((char *)s);
	return (NULL);
}

char	*read_ff(int fd, char **gnl)
{
	int		bytes_readed;
	char	*string;

	bytes_readed = 1;
	string = malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (string == NULL)
		return (NULL);
	while (bytes_readed > 0 && isline(gnl[0]) == 0)
	{
		bytes_readed = read(fd, gnl[0], BUFFER_SIZE);
		string = ft_realloc(string, ft_strlen(string) + ft_strlen(gnl[0]) + 1);
		if (string == NULL)
			return (NULL);
		ft_strlcat(string, gnl[0], ft_strlen(string) + ft_strlen(gnl[0]) + 1);
	}
	if (gnl[0] && isline(gnl[0]) == 1)
		ft_strlcpy(gnl[0], ft_strchr(gnl[0], '\n') + 1, ft_strlen(ft_strchr(gnl[0], '\n') + 1));
	return (string);
}



char	*get_next_line(int fd)
{
	static char	gnl[FOPEN_MAX][BUFFER_SIZE + 1];
	char		*string;

	if (fd < 0 || BUFFER_SIZE > SSIZE_MAX)
		return (NULL);
	if (!gnl[fd] || isline(gnl[fd]) == 0)
		string = read_ff(fd, &(gnl[fd]));
	if (string == NULL)
		return (NULL);
	return (extract_line(string));
}
