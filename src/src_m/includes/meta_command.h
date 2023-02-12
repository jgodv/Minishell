/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta_command.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgo <jgo@student.42seoul.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 11:20:50 by jgo               #+#    #+#             */
/*   Updated: 2023/02/12 12:33:57 by jgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef META_MVP_H
# define META_MVP_H

/* meta_receiver/c */
void    *meta_receiver(t_meta_flags flag);

/* meta_invoker.c */
t_meta *get_meta(void);
int     get_err_num(void);
t_hash_table *get_envp(void);
t_list  *get_unlink_lst(void);

#endif