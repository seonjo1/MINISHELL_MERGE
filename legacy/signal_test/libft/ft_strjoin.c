/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 22:25:02 by michang           #+#    #+#             */
/*   Updated: 2024/01/05 20:51:43 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	size_t	i;
	char	*new;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	new = ft_calloc(s1_len + s2_len + 1, sizeof(char));
	if (!new)
		return (0);
	i = 0;
	ft_strlcpy(new, s1, s1_len + 1);
	ft_strlcat(new + s1_len, s2, s1_len + s2_len + 1);
	return (new);
}

char	*ft_strjoin_s(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	size_t	i;
	char	*new;

	if (!s1)
		s1 = ft_strdup(" ");
	if (!s2)
		s2 = ft_strdup(" ");
	if (!s1 || !s2)
		return (0);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	new = ft_calloc(s1_len + s2_len + 1, sizeof(char));
	if (!new)
		exit(1);
	i = 0;
	ft_strlcpy(new, s1, s1_len + 1);
	ft_strlcat(new + s1_len, s2, s1_len + s2_len + 1);
	return (new);
}
