#include "shell.h"

/**
 * _exits - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if info.argv[0] != "exit"
 */
int _exits(info_t *info)
{
        int exitcheck;

        if (info->argv[1])  /* If there is an exit arguement */
        {
                exitcheck = _error_atoi(info->argv[1]);
                if (exitcheck == -1)
                {
                        info->status = 2;
                        print_error(info, "Illegal number: ");
                        _eput(info->argv[1]);
                        _eput_charac('\n');
                        return (1);
                }
                info->err_num = _error_atoi(info->argv[1]);
                return (-2);
        }
        info->err_num = -1;
        return (-2);
}

/**
 * _changecd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * *  Return: Always 0
 */
int _changecd(info_t *info)
{
        char *s, *dir, buffer[1024];
        int chdir_ret;

        s = getcwd(buffer, 1024);
        if (!s)
                _put("TODO: >>getcwd failure emsg here<<\n");
        if (!info->argv[1])
        {
                dir = _get_env(info, "HOME=");
                if (!dir)
                        chdir_ret = /* TODO: what should this be? */
                                chdir((dir = _get_env(info, "PWD=")) ? dir : "/");
                else
                        chdir_ret = chdir(dir);
        }
        else if (str_comp(info->argv[1], "-") == 0)
        {
                if (!_get_env(info, "OLDPWD="))
                {
                        _put(s);
                        _putchar('\n');
                        return (1);
                }
                _put(_get_env(info, "OLDPWD=")), _putchar('\n');
                chdir_ret = /* TODO: what should this be? */
                        chdir((dir = _get_env(info, "OLDPWD=")) ? dir : "/");
        }
        else
                chdir_ret = chdir(info->argv[1]);
        if (chdir_ret == -1)
        {
                print_error(info, "can't cd to ");
                _eput(info->argv[1]), _eput_charac('\n');
        }
        else
        {
                _set_env(info, "OLDPWD", _get_env(info, "PWD="));
                _set_env(info, "PWD", getcwd(buffer, 1024));
        }
        return (0);
}

/**
 * _cd_help - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _cd_help(info_t *info)
{
        char **arg_array;

        arg_array = info->argv;
        _put("help call works. Function not yet implemented \n");
        if (0)
                _put(*arg_array); /* temp att_unused workaround */
        return (0);
}
