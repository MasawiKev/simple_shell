#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM        0
#define CMD_OR          1
#define CMD_AND         2
#define CMD_CHAIN       3

/* covert_number () */
#define CONVERT_LOWERCASE       1
#define CONVERT_UNSIGNED                2

/*if using system getline () 1 */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE       ".simple_shell_history"
#define HIST_MAX        4096


extern char **environ;


/**
 * struct liststr - a singly linked list
 * @num: number field
 * @str: string
 * @next: it points to the next node
 */
typedef struct liststr
{
        int num;
        char *str;
        struct liststr *next;
} list_t;

/**
 * struct passinfo - this contains psuedo-arrangements to pass into
 * funtion, allowing uniform prototype for funtion pointer struct
 * @arg: string generated from getline containing arguments.
 * @argv: an array of strings generated form arg
 * @path: string path for the current command
 * @argc: argument count
 * @line_count: error count.
 * @err_num: error code for exit ()
 * @linecount_flag: on count this line input
 * @fname: the program filename.
 * @env: a linked list local copy of environ.
 * @environ: a custom modified copy of environ form env
 * @history: history node.
 * @alias: alias node.
 * @env_changed: on if environ wa changed
 * @status: returns status of the last exec'd command
 *  * @cmd_buf: an address of the pointer to cmd_buf, on, if chaining
 * @cmd_buf_type: CMD_type ||, &&, ;
 * @readfd: fd from which to read line input
 * @histcount: history line number count
 */
typedef struct passinfo
{
        char *arg;
        char **argv;
        char *path;
        int argc;
        unsigned int line_count;
        int err_num;
        int linecount_flag;
        char *fname;
        list_t *env;
        list_t *history;
        list_t *alias;
        char **environ;
        int env_changed;
        int status;
        char **cmd_buf;
        int cmd_buf_type;
        int readfd;
        int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
        0, 0, 0}

/**
 * struct builtin - this contains a builtin string and related function
 * @type: builtin command flag
 * @func: function
 *
 */
typedef struct builtin
{
        char *type;
        int (*func)(info_t *);
} builtin_table;

/* shloop.c*/
int hsh(info_t *, char **);
int find_built_in(info_t *);
void find_cmnd(info_t *);
void fork_cmnd(info_t *);

/* presser.c */
int _cmnd(info_t *, char *);
char *dupl_char(char *, int, int);
char *finds_path(info_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* errors.c */
void _eput(char *);
int _eput_charac(char);
int _put_fd(char c, int fd);
int _puts_fd(char *str, int fd);

/* first_string.c */
int str_length(char *);
int str_comp(char *, char *);
char *start_with(const char *, const char *);
char *str_cat(char *, char *);

/* new_string.c */
char *string_copy(char *, char *);
char *string_dup(const char *);
void _put(char *);
int _putchar(char);

/* _the_exit.c */
char *_strn_copy(char *, char *, int);
char *_string_cat(char *, char *, int);
char *_str_character(char *, char);

/* tokenization.c */
char **str_tor(char *, char *);
char **strt_ower(char *, char);

/* reallocate.c */
char *_memset(char *, char, unsigned int);
void freee(char **);
void *_reallocat(void *, unsigned int, unsigned int);

/* the_memory.c */
int frees(void **);

/* atoi.c */
int interact(info_t *);
int is_delimeter(char, char *);
int _alpha(int);
int _atoi(char *);

/* new_errors.c */
int _error_atoi(char *);
void print_error(info_t *, char *);
int print_delimeter(int, int);
char *convert_no(long int, int, int);
void delete_comment(char *);

/* first_builtin.c */
int _exits(info_t *);
int _changecd(info_t *);
int _cd_help(info_t *);

/* new_builtin.c */
int _show_history(info_t *);
int _alias_string(info_t *);
int prints_newname(list_t *);
int unsets_newname(info_t *, char *);
int sets_newname(info_t *info, char *);

/* get_the_line.c */
ssize_t _get_input(info_t *);
int _get_the_line(info_t *, char **, size_t *);
void signal_intHandler(int);
ssize_t input_buffer(info_t *, char **, size_t *);
ssize_t reads_buffer(info_t *, char *, size_t *);

/* get_information.c */
void remove_info(info_t *);
void setup_info(info_t *, char **);
void frees_info(info_t *, int);

/* the_environn.c */
char *_get_env(info_t *, const char *);
int _environn(info_t *);
int _set_environn(info_t *);
int _unset_envv(info_t *);
int populate_list(info_t *);

/* get_env.c */
char **get_environ(info_t *);
int _the_unsetenv(info_t *, char *);
int _set_env(info_t *, char *, char *);

/* savedhistory.c */
char *get_history(info_t *info);
int wrte_history(info_t *info);
int reads_history(info_t *info);
int build_historylist(info_t *info, char *buf, int linecount);
int renumber_history(info_t *info);

/*formerlists.c */
list_t *adds_node(list_t **, const char *, int);
list_t *adds_nodeend(list_t **, const char *, int);
size_t print_list_string(const list_t *);
int delete_indexnode(list_t **, unsigned int);
void frees_list(list_t **);

/* newlists1.c */
size_t list_length(const list_t *);
char **list_strings(list_t *);
size_t prints_list(const list_t *);
list_t *node_startswith(list_t *, char *, char);
ssize_t get_indexnode(list_t *, list_t *);

/* varations.c */
int iss_chain(info_t *, char *, size_t *);
void check_chain(info_t *, char *, size_t *, size_t, size_t);
int change_alias(info_t *);
int change_vars(info_t *);
int change_string(char **, char *);

#endif
