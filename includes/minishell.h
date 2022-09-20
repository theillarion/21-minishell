#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "../lib/libft/libft.h"
# include "../lib/libvector/includes/vector.h"

# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <errno.h>
//# include <linux/limits.h>
# include <fcntl.h>
# include "get_next_line.h"

# ifndef PATH_MAX
#  define PATH_MAX 1024
# endif 

# define COUNT_FUNCTIONS 7

typedef struct sigaction	t_sigaction;

typedef struct s_prompt
{
	char	*current_prompt;
	bool	is_need_change;
}	t_prompt;

typedef struct s_info
{
	char	*name_shell;
	char	*pwd;
}	t_info;

typedef struct s_variable_env
{
	char	*name;
	char	**values;
}			t_variable_env;

enum e_status
{
	SUCCESS = 0,
	COMMON_ERROR
};

enum e_Token
{
	t_word,
	t_pipe,
	t_r_in,
	t_hd,
	t_r_out,
	t_r_outa,
	t_sep
};

typedef struct s_token
{
	char	*start;
	int		size;
	int		type;
}	t_token;

typedef struct s_redir
{
	int		r_type;
	t_token	*arg;
}	t_redir;

typedef struct s_environment
{
	t_vector	variables_env;
	t_vector	functions;
	t_sigaction	action;
	t_info		info;
	t_prompt	prompt;
	char		*input_line;
	char		**envp;
	bool		is_need_update_envp;
	int			last_code;
	t_vector	tokens;
	t_vector	groups;
}				t_environment;

typedef struct s_function
{
	char	*name;
	int		(*func)(t_environment *, const char *const*);
}			t_function;

typedef struct s_command
{
	t_token		*command;
	t_vector	args;
	t_vector	redirs;
	t_function	*builtin;
}				t_command;

void			ft_push(t_vector	*vector, const char	*string_var);
size_t			ft_find_by_name(const t_vector	*vector, const char	*name);
t_variable_env	*ft_get_by_name(const t_vector	*vector, const char *name);

//		prompt.c
void			ft_set_new_prompt(const t_vector *variable_env,
					t_prompt *prompt, t_info info);

//		fill.c
bool			ft_fill(t_environment	*env, char	**envp,
					const char	*name_shell);

//		init.c
void			ft_init(t_environment	*env);

//		commands_utilities.c
char			*ft_get_pwd(void);

//		commands.c
int				ft_command_cd(t_environment *env, const char *const *args);
int				ft_command_pwd(t_environment	*env, const char *const *args);
int				ft_command_env(t_environment	*env, const char *const *args);
int				ft_command_unset(t_environment	*env, const char *const *args);

//		commands_2.c
int				ft_command_exit(t_environment *env, const char *const *args);
int				ft_command_export(t_environment	*env, const char *const *args);
int				ft_command_echo(t_environment	*env, const char *const *args);

//		utilities_readline.c
void			ft_readline_insert(const char	*str);

//		utilities.c
void			ft_foreach(void **array, void (*func)(void *));
void			ft_smart_free(void	**address);
void			ft_smart_double_free(void	***address);
int				ft_smart_atoi(const char	*src, bool	*is_error);
size_t			ft_size_array(void	**address);

//		signal.c
void			ft_handle_signal(int signal);
void			ft_handle_signal_child(int signal);

//		destroy.c
void			ft_destroy(t_environment	*env);

//		exit.c
int				ft_exit(t_environment	*env, int status);
void			ft_exit_with_message(t_environment	*env, int status,
					const char	*command, const char	*msg);

//		variable_env.c
bool			ft_convert_str_to_struct(t_variable_env	*dst, const char *src);
bool			ft_convert_vector_to_array(char	***dst,	const t_vector	*src);
bool			ft_convert_token_vector_to_str_array(char ***dst,
					const t_vector *src);

//		in_out_files.c
void			input_file_fd(t_redir *token, int pipe_fd[2]);
void			output_file_fd(t_redir *token, int pipe_fd[2]);

//		ft_isspace.c
int				ft_isspace(int c);

//		ft_errors_managment.c
void			cmd_not_found(char *const *in_argv);
int				ft_raise_error(char *strarg);
int				ft_raise_perror(char *strarg, int free_arg);
void			exit_find_failure(char **in_argv, char *access_denied_path);

//		get_next_line.c
char			*get_next_line(int fd);

//		here_doc.c
void			here_doc(t_redir *token, int pipe_fd[2]);

//		lexer.c
void			lexer(t_environment *env);

//		parser_utilities_vars.c
char			*ft_strjoin_with_free(char *str1, char *str2, int free_1,
					int free_2);
char			*get_v(const char *string, int *i, const t_environment *env,
					int beg);
//		parser_utilities_syntax.c
int				ft_syntax_error(t_environment *env);
int				check_syntax_token(t_environment *env, size_t i);

//		parser_utilities.c
int				expand_word(t_environment *env, char **start, int *size);

//		parser.c
int				parser(t_environment *env);

//		paths_utilities.c
void			free_command_args(char **args);

//		executor_utilities.c
void			postactions(t_environment *env);
void			find_cmd_in_path(char **args, char **envp);

//		executor.c
int				executor(t_environment *env);

//		file_utilities.c
bool			ft_is_regular_file(char const *path);
bool			ft_is_exist(char const *path);
bool			ft_which(const char *const *paths, const char *name,
					char **dst);

//		print
void			ft_print_error(const t_environment	*env, const char *command,
					const char *msg);
void			ft_print_errno(t_environment	*env, const char *command);

//		main.c
int				main(int argc, char **argv, char **envp);

#endif
