TODOs
=====

The things we have to do for migrating the gphoto.org source code
from subversion repos on sourceforge.net to git repos at github.com.

Pull requests welcome.


Before final migration to git repos
-----------------------------------

  * Verify all commit messages in the converted repos are valid utf-8
    and contain the proper characters.

  * Find and list existing documentation for how to get the sourcecode
    and how to build it.

  * Add a git tag to all generated git repos at their important branch
    heads clearly marking this as the point where the automatic SVN
    conversion stopped.


Before or after final migration to git repos
--------------------------------------------

Not having these done at the time of the "switchover" incurs a day or
two of something approaching a downtime.

  * Make `git clone` and building the code work.

  * Document how to get the source code and how to build it.

  * Add a git tag to all generated git repos at their important branch
    heads clearly marking this at the point where the builds should
    work from git clones.


After final migration to git repos
----------------------------------

  * Verify `git clone` and builds works.

  * Update existing documentation (gphoto2-manual, website, etc.)

  * Find and document a way to compose and publish code releases.

  * Get hold of release tarballs and use `git bisect` to find the git
    commits they were created from. Then re-create the appropriate
    tags and github releases.


Some time, done easiest with git instead of svn
-----------------------------------------------

  * Update Lutz´ mail addr in AUTHORS and MAINTAINERS files

  * Move all non-po files to a single charset: utf-8.
