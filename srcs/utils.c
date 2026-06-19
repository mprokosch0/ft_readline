# include "ft_readline.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	unsigned int	i;
	unsigned int	j;
	char			*dest;

	if (!s1 && !s2)
		return (NULL);
	dest = malloc(sizeof(char) * (strlen(s1) + strlen(s2) + 1));
	if (!dest)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
	{
		dest[j] = s1[i];
		i++;
		j++;
	}
	i = 0;
	while (s2[i])
	{
		dest[j] = s2[i];
		i++;
		j++;
	}
	dest[j] = '\0';
	return (dest);
}

void	skip_lines(int fd, size_t nb_lines)
{
	for (size_t i = 0; i < nb_lines; i++)
	{
		char c = 1;
		while (c != '\n')
			if (read(fd, &c, 1) <= 0)
				return ;
	}
}