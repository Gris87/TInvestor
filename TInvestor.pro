TEMPLATE = subdirs

SUBDIRS = \
  libs \
  app \
  tests \
  tools

# where to find the sub projects - give the folders
libs.subdir  = libs
app.subdir   = app
tests.subdir = tests
tools.subdir = tools

# what subproject depends on others
app.depends = libs
tests.depends = libs
tools.depends = libs
