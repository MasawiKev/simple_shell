#include "shell.h"

/**
 * _show_history - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _show_history(info_t *info)
{
        prints_list(info->history);
        return (0);
}

/**
 * unsets_newname - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unsets_newname(info_t *info, char *str)
{
        char *p, c;
        int ret;

        p = _str_character(str, '=');
        if (!p)
                return (1);
        c = *p;
        *p = 0;
        ret = delete_indexnode(&(info->alias),
                get_indexnode(info->alias, node_startswith(info->alias, str, -1)));
        *p = c;
        return (ret);
}

/**
 * sets_newname - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int sets_newname(info_t *info, char *str)
{
        char *p;

        p = _str_character(str, '=');
        if (!p)
                return (1);
        if (!*++p)
                return (unsets_newname(info, str));

        unsets_newname(info, str);
        return (adds_nodeend(&(info->alias), str, 0) == NULL);
}

/**
 * prints_newname - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int prints_newname(list_t *node)
{
        char *p = NULL, *a = NULL;

        if (node)
        {
                p = _str_character(node->str, '=');
                for (a = node->str; a <= p; a++)
                        _putchar(*a);
                _putchar('\'');
                _put(p + 1);
                _put("'\n");
                return (0);
        }
        return (1);
}

/**
 * _alias_string - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _alias_string(info_t *info)
{
        int i = 0;
        char *p = NULL;
        list_t *node = NULL;

        if (info->argc == 1)
        {
                node = info->alias;
                while (node)
                {
                        prints_newname(node);
                        node = node->next;
                }
                return (0);
        }
        for (i = 1; info->argv[i]; i++)
        {
                p = _str_character(info->argv[i], '=');
                if (p)
                        sets_newname(info, info->argv[i]);
                else
                        prints_newname(node_startswith(info->alias, info->argv[i], '='));
        }

        return (0);
}
