TAG
===

It's 2018, should be able to tag media files with colors and fonts,
right ?

Actually my claim is not HTML, but that ID3 sucks because ideas before
HTTP era were ideas before HTTP era.

However the attack method is the same :
 1. start with end of file tagging
 2. support anywhere tagging in players
 3. win at start of file tagging.
 4. ???
 5. profit

Tagging is inline or offline, uses XML tags for extensible purposes,
and HTML support for fonts is a side effect.


CLI
===

A basic command line interface is available. For now it just generates
embeddable tags based on command line arguments.

It should really allow to read and edit tags from media files. This is work
in progress.


DEMUX
=====

A UNIX demuxer is in progress too to filter between stream data and tagging
data which is sent to a different file descriptor.


META
====

This README is TXT, because fuck Markdown.
