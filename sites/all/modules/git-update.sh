#!/bin/sh

# Run this in the modules dir to do a git pull on everything that has a .git directory in it
##  Use with care  ##

if [ "$(id -u)" = "0" ]
then
  echo Getting module updates ...;
  for file in * ; do
    if [ -d $file ] ; then
      cd $file
      if [ -d .git ]
        then
        echo Checking for update in $file ...;
        git pull origin;
      fi
    cd ..
    fi
  done
  cd ../libraries
  echo Getting library updates ...;
  for file in * ; do
    if [ -d $file ] ; then
      cd $file
      if [ -d .git ]
        then
        echo Checking for update in $file ...;
        git pull origin;
      fi
    cd ..
    fi
  done
  exit 0
else
  echo Must be root user to run updates. Try sudo $0;
  exit 1;
fi

