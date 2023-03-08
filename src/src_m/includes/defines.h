/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgo <jgo@student.42seoul.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 17:38:28 by jgo               #+#    #+#             */
/*   Updated: 2023/03/08 17:07:00 by jgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

# define LOOP TRUE
# define ROOT 0
# define MY_PROMPT " ❯ "
# define S_QUOTE '\''
# define D_QUOTE '\"'
# define DOLLAR	 '$'
# define BUILT_IN_EXEC_OK 1
# define BUILT_IN_EXEC_FAIL 0
# define READ 0
# define WRITE 1

/* color */
# define BOLD "\e[1m"
# define MAGENTA "\e[35m"
# define CYAN "\e[36m"
# define GREEN "\e[92m"
# define GRAY "\e[90m"
# define RED "\e[91m"
# define BLUE "\e[94m"
# define RESET "\e[0m"

#define H_INITIAL_SIZE 100
#define H_GROWTH_FACTOR 2
/* enum typedef */

typedef enum e_token_type t_token_type;
typedef enum e_rdr_type t_rdr_type;
typedef enum e_tree_edge t_tree_edge;
typedef enum e_simple_cmd_type t_simple_cmd_type;
typedef enum e_envp_flags t_envp_flags;
typedef enum e_meta_flags t_meta_flags;
typedef enum e_err_type t_err_type;
typedef enum e_signal_flags t_signal_flags;
typedef enum e_sequence t_sequence;

/* struct typedef */

typedef struct s_meta		t_meta;
typedef struct s_tree		t_tree;
typedef struct s_stack		t_stack;
typedef struct s_deque		t_deque;
typedef struct s_tree_node	t_tree_node;
typedef struct s_cmd		t_cmd;
typedef struct s_pipe		t_pipe;
typedef struct s_rdr		t_rdr;
typedef struct s_token		t_token;
typedef struct s_simple_cmd	t_simple_cmd;
typedef struct s_tokenize	t_tokenize;
typedef struct s_here_doc	t_here_doc;
typedef	struct s_executor	t_executor;
typedef struct s_child_proc t_child_proc;

/* union typedef */

typedef union u_cmd			t_ucmd;

/* data_structure typedef */

typedef struct s_deque	t_deque;
typedef struct s_hash_table t_hash_table;
typedef struct s_stack t_stack;
typedef struct s_tree t_tree;

/* hash_table typedef */
typedef struct s_hash_table t_hash_table;
typedef struct s_hash_asset t_hash_asset;
typedef struct s_hash_elem  t_hash_elem;

/* func_ptr typedef */
typedef int		(*t_built_in_func)(t_simple_cmd *);
typedef void	(*t_rdr_func)(t_rdr *);

enum e_token_type
{
	NONE = -1,
	RDR = 0,
	PIPE = 1,
	CMD = 2, // 실제로 실행하지는 않는다.
	WORD = 3,
	S_CMD = 4
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

enum	e_envp_flags
{
	SET = 0,
	GET = 1,
	DEL = 2,
	PRINT = 3,
	CHAR = 4
};

enum	e_meta_flags
{
	META = 0,
	ENVP = 1,
	UNLINK = 2,
	ERR_NUM = 3,
	PID = 4,
	EXIT_STATUS = 5,
	SET_ERR_NUM = 6,
	SET_EXIT_STATUS = 7,
	GET_CUR_DIR = 8,
	SET_CUR_DIR = 9
};

enum	e_err_type
{
	ERR_NOTHING = 0,
	// 100 번대 parsing
	ERR_PARSE_NUM = 100,
	// 200 번대 heredoc
	ERR_QUOTE = 100,
	ERR_MULTI = 101,
	ERR_PIPE = 102,
	ERR_MULTI_PIPE = 103,
	ERR_FIRST_PIPE = 104,
	
	ERR_RDR_IN = 110,
	ERR_RDR_OUT = 111,
	ERR_RDR_HERE = 112,
	ERR_RDR_APPEND = 113,
	ERR_RDR_MULTI = 114,
	
	ERR_NL = 120,
	// 200 번대 heredoc
	ERR_HEREDOC = 200,
	ERR_HD_WRITE = 201,
	ERR_HD_OPEN = 203,
	// 300 번대 executor
	ERR_ARGS_NUM = 300
};

enum	e_signal_flags
{
	SIG_INIT = 0,
	SIG_CHILD = 1
};

enum	e_sequence
{
	FIRST = 0,
	MIDDLE = 1,
	LAST = 2
};

struct s_meta
{
	t_err_type		err;
	t_hash_table	*envp;
	t_list			*unlink_lst;	// here_doc의 예외처리를 위한 list
	int				exit_status;
	char			*cur_dir;
	char			*prompt;
	// t_history;
};

struct s_envp
{
	char *key;
	char *val;
	int	val_len;
};

struct s_here_doc
{
	char	*file;
	int		fd;
};

struct s_executor
{
	int	out_fd;
	int	in_fd;
	int	cur_fd[2];
	int	prev_fd[2];
	t_token_type child[2];
	t_list *child_lst;
	t_bool	built_in;
	t_bool	single;
};

struct s_tree_node {
	void		*value;
	// int		size; // 본인을 제외한 트리의 크기.
	t_tree_node	*left;
	t_tree_node	*right;
};

struct s_tree {
	t_tree_node	*root;
	void		(*insert)(t_tree_node*, t_tree_edge, t_tree_node*);
	void		(*pre_order_traversal)(t_tree_node *, void (*f)(t_tree_node*));
	void		(*delete_node)(t_tree_node*);
	void		(*destroy)(t_tree *);
};

struct s_deque
{
	size_t	capacity;
	size_t	front;
	size_t	rear;
	size_t	use_size;
	void	**nodes;
	void	(*push_front)(t_deque *, void *);
	void	(*push_rear)(t_deque *, void *);
	void	*(*pop_front)(t_deque *);
	void	*(*pop_rear)(t_deque *);
	void	*(*peek_front)(const t_deque *);
	void	*(*peek_rear)(const t_deque *);
};

struct s_child_proc
{
	pid_t	pid;
	char	*name;
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
	t_ucmd			cmd_val;
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

struct	s_hash_table
{
    t_hash_elem *bucket;
    int cnt;
    int size;
    int prime;
};

struct s_hash_asset
{
    int key;
    int hash;
    int d_hash;
};

struct s_hash_elem
{
    char *key;
    char *val;
    int   val_len;
};

struct s_tokenize
{
	t_token_type	type;
	char			*str;
	int				size;
};

struct s_pipe
{
	int	fd[2];
};


#endif