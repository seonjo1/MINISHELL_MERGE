/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_process_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:31:16 by seonjo            #+#    #+#             */
/*   Updated: 2024/01/06 21:12:09 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

char	*ex_merge_key_and_value(char const *s1, char const *s2)
{
	char	*str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	str = ft_calloc_s(sizeof(char), ft_strlen(s1) + ft_strlen(s2) + 2);
	while (s1[j])
		str[i++] = s1[j++];
	str[i++] = '=';
	j = 0;
	while (s2[j])
		str[i++] = s2[j++];
	str[i] = 0;
	return (str);
}

int	ex_is_builtin(t_cmds *cmds, t_envs *envsp, int fork_flag)
{
	if (ft_strncmp(cmds->argv[0], "cd", 3) == 0)
		btin_cd(cmds, envsp, fork_flag);
	else if (ft_strncmp(cmds->argv[0], "pwd", 4) == 0)
		btin_pwd(fork_flag);
	else if (ft_strncmp(cmds->argv[0], "export", 7) == 0)
		btin_export(cmds, envsp, 0, fork_flag);
	else if (ft_strncmp(cmds->argv[0], "unset", 6) == 0)
		btin_unset(cmds, envsp, fork_flag);
	else if (ft_strncmp(cmds->argv[0], "echo", 5) == 0)
		btin_echo(cmds, fork_flag);
	else if (ft_strncmp(cmds->argv[0], "env", 4) == 0)
		btin_env(envsp, fork_flag);
	else if (ft_strncmp(cmds->argv[0], "exit", 5) == 0)
		btin_exit(cmds, fork_flag);
	else
		return (0);
	return (1);
}

char	**ex_change_to_envp(t_envs *envsp)
{
	t_envs	*node;
	char	**envp;
	int		size;
	int		i;

	size = 0;
	node = envsp->next;
	while (node->next != NULL)
	{
		node = node->next;
		size++;
	}
	envp = malloc(sizeof(char *) * (size + 1));
	node = envsp->next;
	i = 0;
	while (i < size)
		envp[i++] = ex_merge_key_and_value(node->key, node->value);
	envp[i] = NULL;
	return (envp);
}

pid_t	ex_fork(t_cmds *cmdsp, char **envp, int pipe_fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		btin_out(1, errno, strerror(errno));
	else if (pid == 0)
	{
		if (pipe_fd[0] != -1)
			close(pipe_fd[0]);
		if (cmdsp->in_file != -1)
			if (dup2(cmdsp->in_file, 0) == -1)
				btin_out(1, errno, strerror(errno));
		else if (dup2(cmdsp->pipe_read, 0) == -1)
			btin_out(1, errno, strerror(errno));
		if (cmdsp->out_file != -1)
			if (dup2(cmdsp->out_file, 1) == -1)
				btin_out(1, errno, strerror(errno));
		else if (pipe_fd[1] != -1 && dup2(pipe_fd[1], 1) == -1)
			btin_out(1, errno, strerror(errno));
		ex_execute(cmdsp, envp);
	}
}

pid_t	ex_do_pipe(t_cmds *cmdsp, t_envs *envsp, char **envp)
{
	pid_t	pid;
	int		pipe_fd[2];

	if (ex_is_builtin(cmdsp, envsp, 1) != 0)
	{
		if (cmdsp->next == NULL)
		{
			pipe_fd[0] = -1;
			pipe_fd[1] = -1;
			pid = ex_fork(cmdsp, envp, pipe_fd);
			close(cmdsp->pipe_read);
			return (pid);
		}
		else
		{
			pipe(pipe_fd);
			pid = ex_fork(cmdsp, envp, pipe_fd);
			close(pipe_fd[1]);
			if (cmdsp->pipe_read != -1)
				close(cmdsp->pipe_read);
			cmdsp->pipe_read = pipe_fd[0];
			return (pid);
		}
	}
}

void	ex_process_command(t_cmds *cmdsp_head, t_envs *envsp)
{
	t_cmds	*cmdsp;
	char	**envp;
	int		status;
	pid_t	pid;

	cmdsp = cmdsp_head->next;
	cmdsp->pipe_read = -1;
	//단일 builtin 명령어 처리
	if (cmdsp->next == NULL && ex_is_builtin(cmdsp, envsp, 0) == 1)
		;
	else
	{
		envp = ex_change_to_envp(envsp);
		while (cmdsp != NULL)
		{
			pid = ex_do_pipe(cmdsp, envsp, envp);
			cmdsp = cmdsp->next;
		}
		waitpid(pid, &status, NULL);
		while (waitpid(-1, NULL, NULL) != -1)
			;
		if (WIFEXITED(status) != 0)
			g_errno = WEXITSTATUS(status);
		else
			g_errno = WTERMSIG(status) + 128;
		// all_close();
	}
}
