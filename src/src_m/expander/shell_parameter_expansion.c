/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parameter_expansion.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgo <jgo@student.42seoul.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 16:13:40 by jgo               #+#    #+#             */
/*   Updated: 2023/03/10 17:48:38 by jgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "defines.h"
#include "utils.h"
#include "data_structure.h"
#include "envp_command.h"
#include "expander.h"
#include "meta_command.h"

t_bool	dollar_control(char c, char *rear, char next)
{
	if (c == DOLLAR)
	{
		if (rear != NULL && *rear != S_QUOTE)
			return (FT_TRUE);
		else if (next == ' ' || next == '\0')
			return (FT_FALSE);
		else if (rear == NULL)
			return (FT_TRUE);
	}
	return (FT_FALSE);
}

char *expand_variable(char *dst, char *str)
{
	const t_deque *deque = deque_init(ft_strlen(str));
	int	tmp;
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (str[i++])
	{
		quote_control(deque, str[i - 1]);
		if (str[i - 1] == DOLLAR && str[i] == DOLLAR)
			double_dollar(dst, str, &i, &j);
		else if  (dollar_control(str[i - 1], (char *)deque->peek_rear(deque), str[i]) && is_shell_var(str[i]))
		{
			tmp = i;
			if (str[i] == '?' || ft_isdigit(str[i]))
				i++;
			else
				while (is_shell_var(str[i]))
					i++;
			tmp = expand_and_dup(dst, ft_substr(str, tmp, i - tmp), j);
			if (tmp != 0)
				j = tmp;
		}
		else
			dst[j++] = str[i - 1];
	}
	dst[j] = '\0';
	dq_free(deque);
	return (dst);
}

int	cal_expand_len(char *str)
{
	const t_deque *deque = deque_init(ft_strlen(str));
	int	len;
	int	tmp;
	int	i;

	len = 0;
	i = 0;
	while (str[i++])
	{
		quote_control(deque, str[i - 1]);
		if (str[i - 1] == DOLLAR && str[i] == DOLLAR)
			double_dollar(NULL, NULL, &i, &len);
		else if (dollar_control(str[i - 1], (char *)deque->peek_rear(deque), str[i]) && is_shell_var(str[i]))
		{
			tmp = i;
			if (str[i] == '?' || ft_isdigit(str[i]))
				i++;
			else
				while (is_shell_var(str[i]))
					i++;
			tmp = try_expand_and_cal_len(str, i, tmp);
			if (tmp > 0)
				len += tmp;
		}
		else
			len++;
	}
	dq_free(deque);
	return (len);
}


// $$ 
// ''
//  echo "$$SHELL>>$USER" 얘는 우리 쉘에선 $$SHELL>>jgo 이렇게 되어야함.
// 	echo "$1??$USER" 얘도 우리 쉘에선 "??jgo" $1은 expanding했지만 없기 때문에 \0이 됨.
		// 0		8
// echo "$SHELL$USER" /bin/zshjgo
// echo $USER
// $"U""S""E""R"
// $? 는 meta->exit status를 확장한다. 
char *shell_param_expand(char *str)
{
	const int	expand_len = cal_expand_len(str);
	char *dst;

	dst = ft_malloc(sizeof(char) * (expand_len + 1));
	dst = expand_variable(dst, str);
	free(str);
	return (dst);
}

