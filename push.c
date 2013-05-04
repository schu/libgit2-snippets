
#include <stdio.h>

#include "common.h"

static int cred_cb(
	git_cred **cred,
	const char *url,
	const char *username_from_url,
	unsigned int atypes,
	void *payload)
{
	/* if (git_cred_userpass_plaintext_new(cred, "vagrant", "vagrant") < 0) */
	/* 	die_giterror(); */
	if (git_cred_ssh_password_new(cred, "vagrant", "vagrant") < 0)
		return -1;
	return 0;
}

static int status_cb(
	const char *ref,
	const char *msg,
	void *data)
{
	fprintf(stderr, "status ref: %s\t\tmsg: %s\n", ref, msg);
	return 0;
}

int cmd_push(git_repository *repo, int argc, const char **argv)
{
	git_remote *remote;
	git_push *push;

	if (argc-- < 2)
		die("usage: ./git push <remote> <refspec>...");

	if (git_remote_load(&remote, repo, argv[0]) < 0)
		return -1;

	git_remote_set_cred_acquire_cb(remote, &cred_cb, NULL);
	git_remote_check_cert(remote, 0);

	if (git_push_new(&push, remote) < 0)
		return -1;

	while (argc) {
		if (git_push_add_refspec(push, argv[argc--]) < 0)
			return -1;
	}

	if (git_push_finish(push) < 0)
		return -1;

	if (git_push_status_foreach(push, &status_cb, NULL) < 0)
		return -1;

	git_push_free(push);
	git_remote_free(remote);

	return 0;

}
