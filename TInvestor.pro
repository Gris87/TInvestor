TEMPLATE = subdirs

SUBDIRS = \
  libs \
  app \
  tests

# where to find the sub projects - give the folders
libs.subdir  = libs
app.subdir   = app
tests.subdir = tests

# what subproject depends on others
app.depends = libs
tests.depends = libs
