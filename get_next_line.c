/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Your Name <you@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 12:00:00 by you               #+#    #+#             */
/*   Updated: 2025/05/03 10:00:00 by you              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h" // Provides utility function declarations and BUFFER_SIZE
#include <limits.h>         // For SSIZE_MAX (used in original for BUFFER_SIZE check)
                            // FOPEN_MAX from <stdio.h> (included in get_next_line.h)

char	*get_next_line(int fd)
{
	static char	gnl_static_buffers[FOPEN_MAX][BUFFER_SIZE + 1];
	char		*current_processing_buffer;
	char		*line_to_return;
	int			bytes_read;
	char		read_chunk[BUFFER_SIZE + 1];
    size_t      buffer_content_len; // Stores length of content in current_processing_buffer

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= FOPEN_MAX)
		return (NULL);

	current_processing_buffer = ft_calloc(1, sizeof(char)); // Initial empty string
	if (!current_processing_buffer) 
		return (NULL);

	// If there's content in the static buffer, move it to current_processing_buffer
	if (gnl_static_buffers[fd][0] != '\0')
	{
        buffer_content_len = ft_strlen(gnl_static_buffers[fd]);
		char *temp = ft_realloc(current_processing_buffer, buffer_content_len + 1);
		if (!temp) { free(current_processing_buffer); return (NULL); }
		current_processing_buffer = temp;
		ft_strlcpy(current_processing_buffer, gnl_static_buffers[fd], buffer_content_len + 1);
		gnl_static_buffers[fd][0] = '\0'; // Clear static buffer as its content is now moved
	}

	// Read from fd and append to current_processing_buffer until a newline is found or EOF
	while (ft_strchr(current_processing_buffer, '\n') == NULL)
	{
		bytes_read = read(fd, read_chunk, BUFFER_SIZE);
		if (bytes_read < 0) // Read error
		{
			free(current_processing_buffer);
			gnl_static_buffers[fd][0] = '\0'; // Per instruction: clear gnl[fd] on error
			return (NULL);
		}
		if (bytes_read == 0) // EOF reached
			break;
		
		read_chunk[bytes_read] = '\0'; // Null-terminate the chunk read

        buffer_content_len = ft_strlen(current_processing_buffer);
		char *temp = ft_realloc(current_processing_buffer, buffer_content_len + bytes_read + 1);
		if (!temp) { free(current_processing_buffer); return (NULL); } // Realloc failure
		current_processing_buffer = temp;
        // Size for ft_strlcat is the total allocated size of the destination buffer
		ft_strlcat(current_processing_buffer, read_chunk, buffer_content_len + bytes_read + 1);
	}

	// If current_processing_buffer is empty at this point (e.g. EOF from start and no remainder), no line.
	if (current_processing_buffer[0] == '\0')
	{
		free(current_processing_buffer);
		return (NULL);
	}

	// current_processing_buffer is guaranteed to be non-empty here.
	line_to_return = extract_line(current_processing_buffer);

	if (line_to_return == NULL) // Indicates malloc failure within extract_line
	{
        // In this specific case of extract_line, if malloc fails, the input buffer (current_processing_buffer)
        // is typically not modified. So, its full content (potential line + remainder) should be saved.
		ft_strlcpy(gnl_static_buffers[fd], current_processing_buffer, BUFFER_SIZE + 1);
		free(current_processing_buffer);
		return (NULL); // Signal error
	}

	// If extract_line succeeded, line_to_return has the line, 
	// and current_processing_buffer now holds ONLY the remainder.
	// Copy this remainder into the static buffer for the next call.
	ft_strlcpy(gnl_static_buffers[fd], current_processing_buffer, BUFFER_SIZE + 1);
	
	free(current_processing_buffer); // Free the buffer that held combined data/then remainder
	return (line_to_return); // Return the extracted line (allocated by extract_line)
}
