/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_match.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	str_match(const char *str1, const char *str2)
{
	int	i;

	if (str1 == NULL || str2 == NULL)
		return (0);
	i = 0;
	while (str1[i] != '\0' && str2[i] != '\0' && str1[i] == str2[i])
		++i;
	return (str1[i] == str2[i]);
}
