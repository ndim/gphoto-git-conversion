gphoto SVN to git conversion
============================

This is a collection of scripts and data to conver the
http://gphoto.org/ project's SVN repos at SF.net to git repositories.


Install svn2git
---------------

We require _svn2git_ for doing the actual conversion.

However, upstream _svn2git_ at git://github.com/nirvdrum/svn2git.git
lacks support for --rewrite-root, so you must get _svn2git_ here:

    git clone git://github.com/ndim/svn2git.git
    cd svn2git
    git checkout feature/rewrite-root

Then you can locally build and install it with

    gem build svn2git.gemspec
    gem install ./svn2git-2.2.2.gem

To uninstall it later, run

    gem uninstall -x svn2git


Usage
-----

Run

    make

This will create a backup of the SF.net SVN repo at

    svn-backup/

and then convert the code to git repos stored at

    git-repos/
