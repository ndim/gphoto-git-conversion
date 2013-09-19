#!/bin/bash

set -e

svnrepo="$1"
shift
repo="$(basename "$svnrepo")"

GIT="${GIT:=git}"
GCL="${GCL:=${GIT} config --local}"

AUTHORS="${AUTHORS:?}"
GITHUB_USER="${GITHUB_USER:?}"
LOG_WINDOW_SIZE="${LOG_WINDOW_SIZE:?}"
NON_ASCII_LOG="${NON_ASCII_LOG:?}"
PUBLIC_SVN="${PUBLIC_SVN:?}"
S2GROOT="${S2GROOT:?}"
SVN2GIT="${SVN2GIT:?}"
SVNROOT="${SVNROOT:?}"
TOP_DIR="${TOP_DIR:?}"

cd "${S2GROOT}"

exec 2>&1
exec >"conversion.$repo.log"
set -x

if test -f "${repo}/.git/info/exclude"; then
    echo "Update git repo '$repo' from SVN '$svnrepo'"
    cd "${repo}"
    ${SVN2GIT} \
	--rebase \
	--log-window-size "${LOG_WINDOW_SIZE}" \
	--trunk "trunk/$svnrepo" \
	--branches "branches/*/$svnrepo" \
	--tags "tags/*/$svnrepo" \
	--metadata \
	--rewrite-root "${PUBLIC_SVN}" \
	--verbose \
	--authors "${AUTHORS}"
else
    echo "Create new git repo '$repo' from SVN '$svnrepo'"
    mkdir "${repo}"
    cd "${repo}"
    ${SVN2GIT} \
	"${SVNROOT}" \
	--log-window-size "${LOG_WINDOW_SIZE}" \
	--trunk "trunk/$svnrepo" \
	--branches "branches/*/$svnrepo" \
	--tags "tags/*/$svnrepo" \
	--metadata \
	--rewrite-root "${PUBLIC_SVN}" \
	--verbose \
	--authors "${AUTHORS}"

    if   test "m4"      = "$repo"; then ghrepo="gphoto-m4";
    elif test "website" = "$repo"; then ghrepo="gphoto-website";
    else                                ghrepo="$repo"; fi

    ${GCL} remote.origin.url "git@github.com:${GITHUB_USER}/${ghrepo}.git"
    ${GCL} remote.origin.fetch "+refs/heads/*:refs/remotes/origin/*"
    ${GCL} --add remote.origin.push "+refs/heads/*"
    ${GCL} --add remote.origin.push "+refs/tags/*"

    ${GIT} filter-branch \
	--tag-name-filter cat \
	--msg-filter '${TOP_DIR}/log-non-ascii ${NON_ASCII_LOG} | sed -f${TOP_DIR}/commit-msg.sed' \
	-- --all

    ${GIT} for-each-ref --format="%(refname)" refs/original/ \
	| xargs -r -n 1 ${GIT} update-ref -d

    ${GIT} reflog expire --expire=now --all
    ${GIT} gc --prune=now
fi
