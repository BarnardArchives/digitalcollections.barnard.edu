#!/bin/sh

if [ "$(id -u)" = "0" ]
then
  echo "## MODULES"
  for file in * ; do
    if [ -d $file ] ; then
      cd $file
      if [ -d .git ]
        then
        #echo Checking for status in $file ...;
        COMMIT=$(git rev-parse HEAD)
        REPO_ORG=$(git remote -v| grep fetch|awk '{print $2}')
        BRANCH=$(git branch --contains $COMMIT| sed 's/* //g')
        echo "$file $COMMIT $REPO_ORG $BRANCH"
        #git fetch ; git status;
      else
        echo "$file NOT A GIT REPO"
      fi
    cd ..
    fi
  done
  cd ../libraries
  echo "## LIBRARIES"
  for file in * ; do
    if [ -d $file ] ; then
      cd $file
      if [ -d .git ]
        then
        #echo Checking for status in $file ...;
        COMMIT=$(git rev-parse HEAD)
        REPO_ORG=$(git remote -v| grep fetch|awk '{print $2}')
        BRANCH=$(git branch --contains $COMMIT| sed 's/* //g')
        echo "$file $COMMIT $REPO_ORG $BRANCH"
        #git fetch ; git status;
      else
        echo "$file NOT A GIT REPO"
      fi
    cd ..
    fi
  done
  cd ../themes
  echo "## THEMES"
  for file in * ; do
    if [ -d $file ] ; then
      cd $file
      if [ -d .git ]
        then
        #echo Checking for status in $file ...;
        COMMIT=$(git rev-parse HEAD)
        REPO_ORG=$(git remote -v| grep fetch|awk '{print $2}')
        BRANCH=$(git branch --contains $COMMIT| sed 's/* //g')
        echo "$file $COMMIT $REPO_ORG $BRANCH"
        #git fetch ; git status;
      else
        echo "$file NOT A GIT REPO"
      fi
    cd ..
    fi
  done
  exit 0
else
  echo Must be root user to run status. Try sudo $0;
  exit 1;
fi
