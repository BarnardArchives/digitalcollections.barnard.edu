#!/bin/sh

# Run this in the modules dir to do a git fetch ; git status on everything that has a .git directory in it
##  This gives you a basic idea if things have changed... read the output carefully  ##

if [ "$(id -u)" = "0" ]
then
  echo Getting modules status ...;
  for file in * ; do
    if [ -d $file ] ; then
      cd $file
      if [ -d .git ]
        then
        echo Checking for status in $file ...;
        git fetch ; git status;
      fi
    cd ..
    fi
  done
  cd ../libraries
  echo Getting libraries status ...;
  for file in * ; do
    if [ -d $file ] ; then
      cd $file
      if [ -d .git ]
        then
        echo Checking for status in $file ...;
        git fetch ; git status;
      fi
    cd ..
    fi
  done
  exit 0
else
  echo Must be root user to run status. Try sudo $0;
  exit 1;
fi

