#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <stdbool.h>
# include <errno.h>
# include <sys/types.h>
# include <fcntl.h>

# include <readline/readline.h>
# include <readline/history.h>

# include "../lib/libft/libft.h"
# include "../lib/libvector/includes/vector.h"
# include "ft_token.h"
# include "get_next_line.h"

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
	char	*value;
}			t_variable_env;

enum e_status
{
	SUCCESS = 0,
	COMMON_ERROR
};

enum e_Token
{
	t_word,
	t_word_exp,
	t_pipe,
	t_r_out,
	t_r_in,
	t_hd,
	t_r_outa
};

typedef struct s_token
{
	char	*start;
	int		size;
	int		type;
}	t_token;

typedef struct s_redir
{
	int r_type;
	t_token *arg;
}	t_redir;

typedef struct s_command
{
	t_token *command;
	t_vector args;
	t_vector redirs;
}	t_command;

typedef	struct s_environment
{
	t_prompt	prompt;
	t_info		info;
	t_sigaction	action;
	t_vector	variables_env;
	char		*input_line;
	int			last_code;
	t_vector 	tokens;
	t_vector 	groups;
}				t_environment;

void	ft_push(t_vector	*vector, const char	*string_var);
size_t	ft_find_by_name(const t_vector	*vector, const char	*name);

//		prompt.c
void	ft_set_new_prompt(t_prompt	*prompt, t_info	info);

//		init.c
void	ft_init(t_environment	*env, const char	**envp,
				const char	*name_shell);
//		error.c
void	ft_error(const char	*name_shell, const char	*err_msg);

//		commands.c
char	*ft_get_pwd(void);
int		ft_command_cd(t_environment 	*env, const char	*arg);
int		ft_command_pwd(t_environment	*env);
int		ft_command_env(const t_environment	*env);
int		ft_command_unset(t_environment	*env, const char	*arg);

//		commands_2.c
int		ft_command_exit(t_environment 	*env, const char	*arg);
int		ft_command_export(t_environment	*env, const char	*arg);

//		utilities_readline.c
void	ft_readline_insert(const char	*str);

//		utilities.c
void	ft_smart_free(void	**address);
int		ft_smart_atoi(const char	*src, bool	*is_error);

//		signal.c
void	ft_initial_action(t_sigaction *action);

//		deinit.c
void	ft_deinit(t_environment	*env);

//		exit.c
int		ft_exit(t_environment	*env, int status, bool is_clean);

//		variable_env.c
bool	ft_convert_to_struct(t_variable_env	*dst, const char	*src);

//		in_out_files.c
void	input_file_fd(t_redir *token);
void	output_file_fd(t_redir *token);

//		ft_isspace.c
int	ft_isspace(int c);

//		ft_errors_managment.c
void	cmd_not_found(char *const *in_argv);
int	ft_raise_error(char *strarg);
int	ft_raise_perror(char *strarg, int free_arg);
void	exit_find_failure(char **in_argv, char *access_denied_path);

//		get_next_line.c
char	*get_next_line(int fd);

//		here_doc.c
void	here_doc(t_redir *token, int pipe_fd[2])

//		lexer.c
int preparse(t_environment *env);
void lexer(t_environment *env);
void parser(t_environment *env);

//		main.c
int		main(int argc, char **argv, char    **envp);

#endif
