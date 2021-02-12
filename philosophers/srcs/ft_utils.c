#include "../includes/philosophers.h"

int		ft_isdigit(int c)
{
	if (c < 48 || c > 57)
		return (0);
	return (1);
}

int		ft_atoi(const char *str)
{
	int i;
	int nb;
	int sign;

	i = 0;
	nb = 0;
	sign = 1;
	while (str[i] == ' ' || str[i] == '\f' || str[i] == '\n' ||
		str[i] == '\r' || str[i] == '\v' || str[i] == '\t')
		i++;
	if (str[i] == '-')
	{
		sign *= -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (ft_isdigit(str[i]))
	{
		nb = nb * 10 + str[i] - '0';
		i++;
	}
	return (nb * sign);
}

void	*ft_memset(void *b, int c, size_t len)
{
	char		*p;
	size_t		i;

	p = (char*)b;
	i = 0;
	while (i < len)
	{
		p[i] = (unsigned char)c;
		i++;
	}
	return (b);
}

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, '\0', n);
}
