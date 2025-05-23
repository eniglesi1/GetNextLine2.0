#include "get_next_line.h"
#include <stdlib.h> // For malloc, free
#include <stddef.h> // For size_t

// Version from get_next_line.c (with added NULL check for safety)
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] != '\0')
		i++;
	return (i);
}

// Version from get_next_line.c
size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	a;
    size_t src_len;

	if (!dst || !src)
		return (0);
    
    src_len = 0;
    while(src[src_len] != '\0') // Calculate src_len manually to avoid recursive ft_strlen if it's the one being defined/tested
        src_len++;

	if (size == 0)
		return (src_len); // Return length of src as per spec
	a = 0;
	while (a < (size - 1) && src[a] != '\0')
	{
		dst[a] = src[a];
		a++;
	}
	dst[a] = '\0';
	return (src_len); // Return length of src as per spec
}

// From get_next_line.c (modified for clarity and standard behavior)
size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;

	if (!dst && size == 0 && !src) // Edge case from some libcs
		return (0);
	if (!dst && !src) // if both are null
	    return 0;
	    
	src_len = ft_strlen(src); // Length of the source string

    dst_len = 0;
    // Find the current length of dst, but don't read past `size`
    while (dst_len < size && dst[dst_len] != '\0')
        dst_len++;

	if (dst_len >= size) // If dst is already full or `size` is too small to append anything
		return (size + src_len); // As per man page: size + strlen(src)

	// Available space for src content (dst_len already includes space for current content)
    // size - dst_len is remaining space INCLUSIVE of null terminator for dst
	size_t remaining_buffer = size - dst_len; 

	i = 0;
	// Copy src to dst, ensuring there's space for null terminator (hence i < remaining_buffer - 1)
	while (src[i] != '\0' && i < (remaining_buffer - 1) )
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0'; // Null-terminate

	return (dst_len + src_len); // Total length it tried to create
}

// From get_next_line.c
void	ft_bzero(void *s, size_t n)
{
    unsigned char *p = s; // Use unsigned char pointer for byte manipulation
	while (n > 0)
	{
		n--;
		p[n] = '\0'; // Set byte to 0
	}
}

// From get_next_line.c
void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*a;

    if (nmemb == 0 || size == 0) // Standard calloc: if nmemb or size is 0, then calloc() returns NULL or a unique pointer.
    {                           // Let's allocate a zero-sized item for the unique pointer behavior.
        nmemb = 1; size = 1; // To handle the case where it should return a valid pointer to 0 bytes. Or return NULL.
                                // For simplicity and to avoid UB with malloc(0), make it malloc(1) and bzero.
                                // However, the original check was for overflow, then malloc.
                                // If nmemb or size is 0, (nmemb * size) will be 0. malloc(0) is implementation-defined.
                                // Let's stick to original structure.
    }


	if (nmemb != 0 && (nmemb * size) / nmemb != size) // Check for overflow before multiplication
		return (NULL);
	
	a = malloc(nmemb * size);
	if (!a)
		return (NULL);
	ft_bzero(a, nmemb * size); // Zero out the allocated memory
	return (a);
}

// From get_next_line.c (modified for clarity and safety)
void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;
    size_t  old_len;

	if (ptr == NULL)
		return (ft_calloc(size, 1)); // Use 1 as element size for ft_calloc

	if (size == 0)
    {
        free(ptr);
        // Standard realloc(ptr, 0) is equivalent to free(ptr) and may return NULL or a unique pointer.
        // Returning NULL is a common and safe approach.
        return (NULL); 
    }

	// Allocate new memory block. Using ft_calloc ensures it's zeroed if new_size > old_size.
    // This is not strictly what realloc does (it doesn't guarantee zeroing new part).
    // However, original ft_realloc used ft_calloc.
	new_ptr = ft_calloc(size, 1); 
	if (new_ptr == NULL)
    {
        // If allocation fails, original block is untouched.
        // The prompt's original ft_realloc free(ptr) here, which is non-standard.
        // Standard realloc leaves ptr as is if new allocation fails.
        // Let's follow the original ft_realloc from the problem.
        free(ptr); 
        return (NULL);
    }

    // Copy data from old block to new block.
    // Original ft_realloc used ft_strlcpy, implying string data.
    // A true realloc would use memcpy and need the old size.
    // We use ft_strlcpy as per original, assuming null-terminated string at ptr.
    old_len = ft_strlen((char*)ptr); // Length of string in old block
    
    // Copy up to size - 1 characters, ensuring null termination.
    ft_strlcpy((char*)new_ptr, (const char*)ptr, size);
    
	free(ptr); // Free the old memory block
	return (new_ptr);
}

// From get_next_line.c (logic corrected for clarity)
int	isline(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (0);
	while (str[i] && str[i] != '\n')
		i++;
	return (str[i] == '\n'); // Returns 1 if '\n' is found, 0 otherwise.
}

// From get_next_line.c
char	*ft_strchr(const char *s, int c)
{
    if (!s) return NULL; // Guard for NULL string
	while (*s)
	{
		if (*s == (unsigned char)c)
			return ((char *)s);
		s++;
	}
	if (c == 0 || *s == (unsigned char)c) // If c is '\0', return pointer to the null terminator
		return ((char *)s);
	return (NULL);
}

// Existing function from previous step
char *extract_line(char *buffer)
{
    char *line;
    size_t i; 
    size_t j; 

    if (!buffer || buffer[0] == '\0')
        return (NULL);

    i = 0;
    while (buffer[i] != '\0' && buffer[i] != '\n')
        i++;

    size_t line_len = i + (buffer[i] == '\n');
    line = (char *)malloc(sizeof(char) * (line_len + 1));
    if (!line)
        return (NULL); 

    ft_strlcpy(line, buffer, line_len + 1); // Uses the ft_strlcpy defined in this file
    line[line_len] = '\0'; // Ensure null termination

    if (buffer[i] == '\n')
    {
        j = 0;
        // i + 1 is the character after '\n'
        while (buffer[i + 1 + j] != '\0')
        {
            buffer[j] = buffer[i + 1 + j];
            j++;
        }
        buffer[j] = '\0'; 
    }
    else 
    {
        buffer[0] = '\0'; 
    }

    return (line);
}
