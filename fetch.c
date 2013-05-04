
#include <stdio.h>
#include <string.h>

#include "common.h"

static int cb_cred(
	git_cred **cred,
	const char *url,
	const char *username_from_url,
	unsigned int allowed_types,
	void *data)
{
	if (git_cred_ssh_password_new(cred, "vagrant", "vagrant") < 0)
		die_giterror();
	return 0;
}

static int cb_update_tips(const char *ref, const git_oid *old,
			  const git_oid *new, void *data)
{
	char *hex;

	fprintf(stderr, "%s:\t", ref);

	hex = git_oid_allocfmt(old);
	fprintf(stderr, "%s -> ", hex);
	free(hex);

	hex = git_oid_allocfmt(new);
	fprintf(stderr, "%s\n", hex);
	free(hex);

	return 0;
}

int cmd_fetch(git_repository *repo, int argc, const char **argv)
{
	int error;
	git_remote *r;
	struct git_remote_callbacks callbacks;

	memset(&callbacks, 0x0, sizeof(callbacks));
	callbacks.update_tips = cb_update_tips;

	if (argc < 1)
		die("usage: ./git fetch <remote>");

	error = git_remote_load(&r, repo, argv[0]);
	if (error < 0)
		return -1;

	git_remote_set_cred_acquire_cb(r, cb_cred, NULL);
	git_remote_set_callbacks(r, &callbacks);
	git_remote_check_cert(r, 0);

	error = git_remote_connect(r, GIT_DIRECTION_FETCH);
	if (error < 0)
		return -1;

	error = git_remote_download(r, NULL, NULL);
	if (error < 0)
		return -1;

	error = git_remote_update_tips(r);
	if (error < 0)
		return -1;

	git_remote_disconnect(r);
	git_remote_free(r);

	return error;
}
