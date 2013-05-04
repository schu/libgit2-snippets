
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

int cmd_pack_objects(git_repository *repo, int argc, const char **argv)
{
	char line[1024];
	git_oid oid;
	git_packbuilder *pb;

	if (argc < 1)
		die("usage: ./git pack-objects <dst> [<threads>]");

	if (git_packbuilder_new(&pb, repo) < 0)
		die_giterror();

	if (argc > 1)
		git_packbuilder_set_threads(pb, atoi(argv[1]));

	for (;;) {
		if (!fgets(line, sizeof(line), stdin)) {
			if (feof(stdin))
				break;
			die("fgets");
		}

		if (git_oid_fromstr(&oid, line) < 0)
			die_giterror();

		if (git_packbuilder_insert(pb, &oid, line+41) < 0)
			die_giterror();
	}

	if (git_packbuilder_write(pb, argv[0]) < 0)
		die_giterror();

	git_packbuilder_free(pb);

	return 0;
}
