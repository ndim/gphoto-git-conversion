TODO items for migrating gphoto.org code from SF.net/SVN to github/git
======================================================================

Pull requests welcome.


Before final migration to git repos
-----------------------------------

  * Verify all commit messages in the converted repos are valid utf-8
    and contain the proper characters.

  * Find and list existing documentation for how to get the sourcecode
    and how to build it.


Before or after final migration to git repos
--------------------------------------------

Not having these done at the time of the "switchover" incurs a day or
two of something approaching a downtime.

  * Make `git clone` and building the code work.

  * Document how to get the source code and how to build it.


After final migration to git repos
----------------------------------

  * Verify `git clone` and builds works.

  * Update existing documentation (gphoto2-manual, website, etc.)

  * Find and document a way to compose and publish code releases.


Some time, done easiest with git instead of svn
-----------------------------------------------

  * Update Lutz'' mail addr in AUTHORS and MAINTAINERS files

  * Move all non-po files to a single charset: utf-8.
