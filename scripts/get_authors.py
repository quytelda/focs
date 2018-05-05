#!/usr/bin/python

import operator
import os
import os.path
import subprocess
import sys

USAGE_DESC = "USAGE: " + sys.argv[0] + " <path>\n"

def get_commit_authors(path):
    list_authors_cmd = ["git", "log", "--pretty=format:\"%an <%ae>\"", path]
    output = subprocess.check_output(list_authors_cmd);

    return output.decode('utf-8').splitlines()

def sort_authors_by_frequency(authors_by_commit):
    unique_authors = set(authors_by_commit)

    authors_by_freq = {}
    for author in unique_authors:
        authors_by_freq[author] = authors_by_commit.count(author)

    # Report authors in order of commit frequency
    return sorted(authors_by_freq.items(),
                  key=operator.itemgetter(1),
                  reverse=True)

if __name__ == "__main__":

    # Check parameters
    if (len(sys.argv) <= 1) or (len(sys.argv) > 2):
        sys.stderr.write(USAGE_DESC)
        exit(1)

    if (sys.argv[1] == "help") or \
       (sys.argv[1] == "-h")   or \
       (sys.argv[1] == "--help"):
       print(USAGE_DESC)
       exit(0)


    path = sys.argv[1]
    if not os.path.exists(path):
        sys.stderr.write("Path '%s' does not exist!\n" % (path,))
        exit(1);
    
    authors_by_commit = get_commit_authors(path)    
    for (author, ncommits) in sort_authors_by_frequency(authors_by_commit):
        print("Author %s has %d commit(s)." % (author, ncommits));
