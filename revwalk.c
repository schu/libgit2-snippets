
#include <stdio.h>

#include "common.h"

int cmd_revwalk(git_repository *repo, int argc, const char **argv)
{
	git_revwalk *walker;
	git_oid oid;
	int error;

	if (git_revwalk_new(&walker, repo) < 0)
		return -1;

	git_revwalk_sorting(walker, GIT_SORT_TIME);

	if (git_revwalk_push_head(walker) < 0)
		return -1;

	while ((error = git_revwalk_next(&oid, walker)) == 0) {
		char hex[41]; hex[40] = '\0';
		git_oid_fmt(hex, &oid);
		printf("%s\n", hex);
	}

	return error == GIT_ITEROVER ? 0 : error;
}
