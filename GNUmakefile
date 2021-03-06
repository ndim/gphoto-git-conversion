GIT = git
GCL = $(GIT) config --local
export GIT
export GCL

RSYNC = rsync

SVN2GIT = svn2git
export SVN2GIT

GITHUB_USER = ndim
export GITHUB_USER

LOG_WINDOW_SIZE = 20000
export LOG_WINDOW_SIZE

# As libgphoto is the conversion taking the longest time, we put it
# second, with the shortest time repo on top.
#
# In case of CONCURRENT_TASKS = 1:
#    We get a quick exercise of the whole conversion code for the
#    short repo, and the total conversion time is not affected.
#
# In case of CONCURRENT_TASKS > 1:
#    We get a quick run through all conversion steps for 'm4',
#    and the longest-running libgphoto2 conversion is started
#    concurrently at the beginning of the conversion job.
#    Total conversion time is not affected by the sequence of
#    repos.
REPO =
# ~10min
REPO += gphoto2-manual
# ~330min
REPO += libgphoto2
# ~130min
REPO += gphoto2
# ~45min
REPO += gtkam
# ~25min
REPO += m4
# ~25min
REPO += gphotofs
# ~15min
REPO += website
# ~10min
REPO += bindings/libgphoto2-java
# ~10min
REPO += bindings/libgphoto2-python
# ~10min
REPO += bindings/libgphoto2-sharp

TOP_DIR = $(PWD)
export TOP_DIR

AUTHORS = $(PWD)/authors.txt
export AUTHORS

PUBLIC_SVN = https://svn.code.sf.net/p/gphoto/code
export PUBLIC_SVN

PUBLIC_RSYNC = svn.code.sf.net::p/gphoto/code/*

SVNROOTDIR = $(TOP_DIR)/svn-backup
SVNROOT = file://$(SVNROOTDIR)
export SVNROOT
S2GROOT = $(TOP_DIR)/git-repos
export S2GROOT

NON_ASCII_LOG = $(S2GROOT)/non-ascii.log
export NON_ASCII_LOG

# CONCURRENT_TASKS = 1
CONCURRENT_TASKS = 3

# DONE: svn2git does not handle --rewrite-root. Do we need that? Yes. Fixed.
# TODO: Change ISO-8859-1 characters in commit messages to UTF-8.

.PHONY: backup-and-convert
backup-and-convert: backup convert

.PHONY: backup
backup:
	$(RSYNC) -av "$(PUBLIC_RSYNC)" "$(SVNROOTDIR)"

.PHONY: convert
convert: grep-non-ascii log-non-ascii commit-msg-del.sed
	rm -f "$(NON_ASCII_LOG)"
	mkdir -p "$(S2GROOT)"
	set -ex; \
	for svnrepo in $(REPO); do echo "$$svnrepo"; done | \
	  xargs -r -n1 -P$(CONCURRENT_TASKS) /usr/bin/time -v "$(PWD)/convert-module.sh"
	set -ex; \
	for svnrepo in $(REPO); do \
	  cd "$(S2GROOT)"; \
	  repo="$$(basename "$$svnrepo")"; \
	  cd "$${repo}"; \
	  $(GIT) push -v; \
	done
	$(MAKE) "$(S2GROOT)/unconverted.log"

CFLAGS += -std=c99
CFLAGS += -Wextra
CFLAGS += -Werror
CFLAGS += -pedantic

grep-non-ascii: grep-non-ascii.c

log-non-ascii: log-non-ascii.c

commit-msg-del.sed: commit-msg.sed
	sed -E 's,[^/]+/g$$,/g,' < commit-msg.sed > commit-msg-del.sed

$(S2GROOT)/unconverted.log: $(S2GROOT)/non-ascii.log commit-msg-del.sed grep-non-ascii
	sed -f commit-msg-del.sed < "$<" | ./grep-non-ascii > "$@"
