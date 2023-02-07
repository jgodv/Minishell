/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgo <jgo@student.42seoul.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 17:38:28 by jgo               #+#    #+#             */
/*   Updated: 2023/02/07 19:17:50 by jgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

# define LOOP TRUE
# define ROOT 0
# define MY_PROMPT "> "
# define S_QUOTE '\''
# define D_QUOTE '\"'
/* enum typedef */

typedef enum e_token_type t_token_type;
typedef enum e_rdr_type t_rdr_type;
typedef enum e_tree_edge t_tree_edge;
typedef enum e_simple_cmd_type t_simple_cmd_type;

/* struct typedef */

typedef struct s_meta t_meta;
typedef struct s_tree t_tree;
typedef struct s_stack t_stack;
typedef struct s_tree_node t_tree_node;
typedef struct s_cmd t_cmd;
typedef struct s_rdr t_rdr;
typedef struct s_pipe t_pipe;
typedef struct s_token t_token;
typedef struct s_simple_cmd t_simple_cmd;

/* union typedef */

typedef union u_cmd			t_ucmd;

/* stack typedef */

typedef struct s_stack		t_stack;

enum e_token_type
{
	RDR = 0,
	PIPE = 1,
	CMD = 2, // 실제로 실행하지는 않는다.
	WORD = 3
};

enum e_rdr_type
{
	IN = 0,
	OUT = 1,
	APPEND = 2,
	HEREDOC = 3
};

enum	e_tree_edge
{
	LEFT = 0,
	RIGHT = 1
};

enum	e_simple_cmd_type	
{
	FT_ECHO = 0,
	FT_CD = 1,
	FT_PWD = 2,
	FT_EXPORT = 3,
	FT_UNSET = 4,
	FT_ENV = 5,
	FT_EXIT = 6,
	FT_EXTERNAL = 7
};

struct s_meta  // 모든 구조체를 담을 부모구조체
{
	int		err;
	t_list	*envp;
	t_list	*unlink_lst;	// here_doc의 예외처리를 위한 list
	// t_history;
};

struct s_tree_node {
	void	*value;
// int		size; // 본인을 제외한 트리의 크기.
	t_tree_node	*left;
	t_tree_node	*right;
};

struct s_tree {
	t_tree_node	*root;
	void		(*insert)(t_tree_node*, t_tree_edge, t_tree_node*);
	void		(*pre_order_traversal)(t_tree_node *, void(*f)(t_tree_node*));
	void		(*delete_node)(t_tree_node*);
	void		(*destroy)(t_tree *);
};

union u_cmd
{
	t_simple_cmd	*simple_cmd;
	t_rdr			*rdr;
	t_pipe			*pipe;
};

struct s_token
{
	t_token_type	type;
	t_ucmd cmd_val;
};

struct s_simple_cmd
{
	t_simple_cmd_type	type;
	char	*cmd;
	char	**args;
};

struct s_rdr
{
	t_rdr_type rdr_type;
	char	*file;
	t_bool	quote; // heredoc 때문에 적어놓음. quote가 있었는지 없었는지 확인.
};

struct s_pipe
{
	int	fd[2];
};

struct s_stack
{
	t_list	*arr; // malloc arr.content int
	int		size;
	void	(*push)(t_stack *, void *);
	void	*(*pop)(t_stack *);
	void	*(*peek)(t_stack *);
	void	(*destory)(t_stack *);
	t_bool	(*is_empty)(t_stack *stack);
};

#endif