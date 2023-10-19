#include "shell.h"

/**
 * remove_info - initializes info_t struct
 * @info: struct address
 */
void remove_info(info_t *info)
{
        info->arg = NULL;
        info->argv = NULL;
        info->path = NULL;
        info->argc = 0;
}

/**
 * setup_info - initializes info_t struct
 * @info: struct address
 * @av: argument vector
 */
void setup_info(info_t *info, char **av)
{
        int i = 0;

        info->fname = av[0];
        if (info->arg)
        {
                info->argv = str_tor(info->arg, " \t");
                if (!info->argv)
                {

                        info->argv = malloc(sizeof(char *) * 2);
                        if (info->argv)
                        {
                                info->argv[0] = string_dup(info->arg);
                                info->argv[1] = NULL;
                        }
                }
                for (i = 0; info->argv && info->argv[i]; i++)
                        ;
                info->argc = i;

                change_alias(info);
                change_vars(info);
        }
}

/**
 * frees_info - frees info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void frees_info(info_t *info, int all)
{
        freee(info->argv);
        info->argv = NULL;
        info->path = NULL;
        if (all)
        {
                if (!info->cmd_buf)
                        free(info->arg);
                if (info->env)
                        frees_list(&(info->env));
                if (info->history)
                        frees_list(&(info->history));
                if (info->alias)
                        frees_list(&(info->alias));
                freee(info->environ);
                        info->environ = NULL;
                frees((void **)info->cmd_buf);
                if (info->readfd > 2)
                        close(info->readfd);
                _putchar(BUF_FLUSH);
        }
}
