.. highlight :: sh

==================
Submitting Patches
==================

You can submit patches to the project generated using `git format-patch`, `git diff`, or `diff -u` via email.

How to Generate a Patch
-----------------------
If you do your development under Git, creating a patch is very easy.  The ``git format-patch`` command will automatically convert your commits to patches that can be applied to the upstream repository.  If there is more than one commit, then a patch is generated for each one and the result is referred to as a "patch series".  For example::

  $ git checkout mybranch
  $ ... # make changes
  $ git commit -m "My git commit message."
  $ git format-patch master..mybranch

You should see a new file in the current directory called ``0001-My-git-commit-message.patch``.  This is the patch file that describes your changes.  Unless you know what you are doing, it is best to submit the patch as an email attachment, since most email clients/servers do a really good job of mangling patches and rendering them unusable.  Please do not send links to patch file downloads.  If you want, Git itself can be configured to send patches via email in a compatible format.  See https://git-scm.com/docs/git-send-email.
  
Where to Send the Patch
-----------------------
Once you have generated your patch(es), you may wonder where to send them.  The best way to find out is to use the ``scripts/get_authors.py`` script.  It will tell you, in sorted order, who has commited the most to a certain file or directory.  Run ``scripts/get_authors.py`` and pass it the file or directory you modified in your changes::

  $ git checkout master # make sure to check commiters on the master branch
  $ ./scripts/get_authors.py src/list/double_list.c 
  Author "Quytelda Kahja <quytelda@tamalin.org>" has 8 commit(s).
  Author "Erumpet <nhakmiller@gmail.com>" has 1 commit(s).

Send the patch to the most frequent commiter on the list, and perhaps ``CC`` the second or third most commiters for good measure.

Additionally, you can always send patches to the primary project maintainer: Quytelda Kahja <quytelda@tamalin.org>.
