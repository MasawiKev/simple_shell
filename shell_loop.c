#include "shell.h"

/**
 * hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(info_t *info, char **av)
{
        ssize_t r = 0;
        int builtin_ret = 0;

        while (r != -1 && builtin_ret != -2)
        {
                remove_info(info);
                if (interact(info))
                        _put("$ ");
                _eput_charac(BUF_FLUSH);
                r = _get_input(info);
                if (r != -1)
                {
                        setup_info(info, av);
                        builtin_ret = find_built_in(info);
                        if (builtin_ret == -1)
                                find_cmnd(info);
                }
                else if (interact(info))
                        _putchar('\n');
                frees_info(info, 0);
        }
        wrte_history(info);
        frees_info(info, 1);
        if (!interact(info) && info->status)
                exit(info->status);
        if (builtin_ret == -2)
	        {
                if (info->err_num == -1)
                        exit(info->status);
                exit(info->err_num);
        }
        return (builtin_ret);
}

/**
 * find_built_in - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *                      0 if builtin executed successfully,
 *                      1 if builtin found but not successful,
 *                      -2 if builtin signals exit()
 */
int find_built_in(info_t *info)
{
        int i, built_in_ret = -1;
        builtin_table builtintbl[] = {
                {"exit", _exits},
                {"env", _environn},
                {"help", _cd_help},
                {"history", _show_history},
                {"setenv", _set_environn},
                {"unsetenv", _unset_envv},
                {"cd", _changecd},
                {"alias", _alias_string},
                {NULL, NULL}
        };

        for (i = 0; builtintbl[i].type; i++)
                if (str_comp(info->argv[0], builtintbl[i].type) == 0)
                {
                        info->line_count++;
                        built_in_ret = builtintbl[i].func(info);
                        break;
                }
        return (built_in_ret);
}

/**
 * find_cmnd - finds a command in PATH
 * @info: the parameter & return info struct
 * *
 * Return: void
 */
void find_cmnd(info_t *info)
{
        char *path = NULL;
        int i, k;

        info->path = info->argv[0];
        if (info->linecount_flag == 1)
        {
                info->line_count++;
                info->linecount_flag = 0;
        }
        for (i = 0, k = 0; info->arg[i]; i++)
                if (!is_delimeter(info->arg[i], " \t\n"))
                        k++;
        if (!k)
                return;

        path = finds_path(info, _get_env(info, "PATH="), info->argv[0]);
        if (path)
        {
                info->path = path;
                fork_cmnd(info);
        }
	        else
        {
                if ((interact(info) || _get_env(info, "PATH=")
                        || info->argv[0][0] == '/') && _cmnd(info, info->argv[0]))
                        fork_cmnd(info);
                else if (*(info->arg) != '\n')
                {
                        info->status = 127;
                        print_error(info, "not found\n");
                }
        }
}

/**
 * fork_cmnd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void fork_cmnd(info_t *info)
{
        pid_t child_pid;

        child_pid = fork();
        if (child_pid == -1)
        {
                /* TODO: PUT ERROR FUNCTION */
                perror("Error:");
                return;
        }
        if (child_pid == 0)
        {
                if (execve(info->path, info->argv, get_environ(info)) == -1)
                {
                        frees_info(info, 1);
                        if (errno == EACCES)
                                exit(126);
                        exit(1);
                }
                /* TODO: PUT ERROR FUNCTION */
        }
        else
        {
                wait(&(info->status));
                if (WIFEXITED(info->status))
                {
                        info->status = WEXITSTATUS(info->status);
                        if (info->status == 126)
                                print_error(info, "Permission denied\n");
                }
        }
}
