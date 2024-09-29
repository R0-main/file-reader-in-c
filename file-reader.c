#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

typedef struct file_line {
	char		*content;
	int		len;
	struct file_line	*next;
} file_line;

char	*ft_strdup(char *str)
{
	int	i;
	char	*result;
	int	str_len;

	i = 0;
	str_len = ft_strlen(str);
	result = (char *)malloc(sizeof(char) * str_len + 1);
	if (!result)
		return NULL;
	while (str[i])
	{
		result[i] = str[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

file_line *get_file_lines(char *path)
{
	int		i;
	int		file_definition;
	file_line	*first_line;
	file_line	*old_line;
	file_line	*current_line;
	char		*buffer;
	int		read_len;
	int		offset;
	char		*str;

	offset = 0;
	read_len = 1;
	old_line = NULL;
	buffer = NULL;
	first_line = NULL;
	while (buffer == NULL || buffer[read_len - 2] != '\0'){
		free(buffer);
		file_definition = open(path, O_RDONLY);
		if (file_definition == -1)
			break;
		buffer = malloc(sizeof(char) * read_len + 1);
		if (!buffer)
			break;
		read(file_definition, buffer, read_len);
		buffer[read_len] = '\0';
		if (buffer[read_len - 1] == '\n' || buffer[read_len - 1] == '\0')
		{
			current_line = malloc(sizeof(file_line));
			if (!current_line)
				break;
			current_line->content = ft_strdup(buffer + offset);
			current_line->len = ft_strlen(current_line->content);
			if (old_line)
				old_line->next = current_line;
			old_line = current_line;
			if (offset == 0)
				first_line = current_line;
			offset = read_len;
		}
		close(file_definition);
		read_len++;
	}
	current_line->next = NULL;
	free(buffer);
	return (first_line);
}

int	main(int argc, char **argv)
{
	if (argc < 1 || argc > 2)
		return (1);

	char *path = argv[1];

	file_line	*first_line;
	first_line = get_file_lines(path);
	file_line	*current_line;
	current_line = first_line;
	
	while (current_line != NULL)
	{
		printf("%s", current_line->content);
		current_line = current_line->next;
	}
	while (first_line != NULL)
	{
		current_line = first_line->next;
		free(first_line->content);
		free(first_line);
		first_line = current_line;
	}
}
