GIT = git
GCL = $(GIT) config --local

RSYNC = rsync

SVN2GIT = svn2git

GITHUB_USER = ndim

REPO =
REPO += m4
REPO += gphoto2
REPO += libgphoto2
REPO += gphotofs
REPO += gtkam
REPO += gphoto2-manual
REPO += website
REPO += bindings/libgphoto2-java
REPO += bindings/libgphoto2-python
REPO += bindings/libgphoto2-sharp

AUTHORS = $(PWD)/authors.txt
PUBLIC_SVN = https://svn.code.sf.net/p/gphoto/code
PUBLIC_RSYNC = svn.code.sf.net::p/gphoto/code/*

SVNROOTDIR = $(PWD)/svn-backup
SVNROOT = file://$(SVNROOTDIR)
S2GROOT = $(PWD)/git-repos

# DONE: svn2git does not handle --rewrite-root. Do we need that? Yes. Fixed.
# TODO: Change ISO-8859-1 characters in commit messages to UTF-8.

.PHONY: backup-and-convert
backup-and-convert: backup convert

.PHONY: backup
backup:
	$(RSYNC) -av "$(PUBLIC_RSYNC)" "$(SVNROOTDIR)"

.PHONY: convert
convert:
	mkdir -p "$(S2GROOT)"
	set -ex; \
	for svnrepo in $(REPO); do \
	  cd "$(S2GROOT)"; \
	  repo=$$(basename "$$svnrepo"); \
	  if test -f "$${repo}/.git/info/exclude"; then \
	    cd "$${repo}"; \
	    $(SVN2GIT) \
	      --rebase \
	      --trunk "trunk/$$svnrepo" \
	      --branches "branches/*/$$svnrepo" \
	      --tags "tags/*/$$svnrepo" \
	      --metadata \
	      --rewrite-root "$(PUBLIC_SVN)" \
	      --verbose \
	      --authors "$(AUTHORS)"; \
	  else \
	    mkdir "$${repo}"; \
	    cd "$${repo}"; \
	    $(SVN2GIT) \
	      $(SVNROOT) \
	      --trunk "trunk/$$svnrepo" \
	      --branches "branches/*/$$svnrepo" \
	      --tags "tags/*/$$svnrepo" \
	      --metadata \
	      --rewrite-root "$(PUBLIC_SVN)" \
	      --verbose \
	      --authors "$(AUTHORS)"; \
	    if   test "m4"      = "$$repo"; then ghrepo="gphoto-m4"; \
	    elif test "website" = "$$repo"; then ghrepo="gphoto-website"; \
	    else                                 ghrepo="$$repo"; fi; \
	    $(GCL) remote.origin.url "git@github.com:$(GITHUB_USER)/$${ghrepo}.git"; \
	    $(GCL) remote.origin.fetch "+refs/heads/*:refs/remotes/origin/*"; \
	    $(GCL) --add remote.origin.push "+refs/heads/*"; \
	    $(GCL) --add remote.origin.push "+refs/tags/*"; \
	    $(GIT) filter-branch \
	      --tag-name-filter cat \
	      --msg-filter 'sed -f$(PWD)/commit-msg.sed' \
	      -- --all; \
	    $(GIT) for-each-ref --format="%(refname)" refs/original/ \
	      | xargs -n 1 git update-ref -d; \
	    $(GIT) reflog expire --expire=now --all; \
	    $(GIT) gc --prune=now; \
	  fi; \
	  $(GIT) push -v; \
	done
