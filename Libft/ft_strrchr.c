/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 11:29:18 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/22 11:34:15 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(const char *s, int c)
{
	const unsigned char	*str;
	unsigned char		b;
	const unsigned char	*lastoccur;

	str = (const unsigned char *)s;
	b = (unsigned char)c;
	lastoccur = 0;
	while (*str != '\0')
	{
		if (*str == b)
		{
			lastoccur = str;
		}
		str++;
	}
	if (b == '\0')
	{
		return ((char *)str);
	}
	return ((char *)lastoccur);
}
/*
int main()
{
    char str1[] = "My name is Dilara";
    printf("String 1: %s\n", ft_strrchr(str1, 'a'));
    
    char str2[] = "My name is Dilara";
    printf("String 2: %s",  strrchr(str2, 'a'));
    return 0;
}*/
