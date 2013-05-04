
#include <string.h>
#include <stdlib.h>

#include "common.h"

static char *repo_path = NULL;

static struct {
	const char *name;
	cmd cb;
} cmds[] = {
	{"clone", cmd_clone},
	{"fetch", cmd_fetch},
	{"pack-objects", cmd_pack_objects},
	{"push", cmd_push},
	{"revwalk", cmd_revwalk},
	{NULL, NULL}
};

static const char * usage()
{
	return "usage: ./git [--repo <repo>] <cmd> [<args>]";
}

/* taken from git.git */
static int handle_options(const char ***argv, int *argc)
{
	const char **orig_argv = *argv;

	while (*argc > 0) {
		const char *cmd = (*argv)[0];
		if (cmd[0] != '-')
			break;

		if (!strcmp(cmd, "--repo")) {
			if (*argc < 2)
				die("no path given for --repo %d", 123);

			repo_path = malloc(sizeof((*argv)[1]) + 1);
			if (!repo_path)
				die("out of memory");
			strcpy(repo_path, (*argv)[1]);

			(*argv)++;
			(*argc)--;
		} else
			die("unknown option %s", cmd);

		(*argv)++;
		(*argc)--;
	}
	return (*argv) - orig_argv;
}

int run_cmd(cmd cb, int argc, const char **argv)
{
	int error;
	git_repository *repo = NULL;

	git_threads_init();

	error = git_repository_open(&repo, repo_path ? repo_path : ".git");
	if (error < 0)
		die_giterror();

	error = cb(repo, argc, argv);
	if (error < 0)
		die_giterror();

	if (repo_path)
		free(repo_path);
	if (repo)
		git_repository_free(repo);

	git_threads_shutdown();

	return error;
}

int main(int argc, const char **argv)
{
	int i;

	if (argc < 2)
		die(usage());

	argv++;
	argc--;
	handle_options(&argv, &argc);

	if (argc == 0)
		die(usage());

	for (i=0; cmds[i].name; i++) {
		if (!strcmp(argv[0], cmds[i].name))
			return run_cmd(cmds[i].cb, --argc, ++argv);
	}

	die("unknown command %s", argv[0]);
}
