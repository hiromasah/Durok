#!/bin/sh

# base-files version 3.9-3

#--
# Create symbolic links from some /etc files to the Windows equivalents
#--

FILES="hosts protocols services networks"

OSNAME="`/bin/uname -s`"
WINHOME="`/bin/cygpath -w -W`"
WINETC="$WINHOME\\system32\\drivers\\etc"

if [ ! -d "$WINETC" ]
then
  echo "Directory $WINETC does not exist; exiting" >&2
  echo "If directory name is garbage you need to update your cygwin package" >&2
  exit 0
fi

for mketc in $FILES
do
  if [ ! -e "/etc/$mketc" -a ! -L "/etc/$mketc" ]
  then
    # Windows only uses the first 8 characters
    WFILE="$WINETC\\`expr substr "$mketc" 1 8`"
    /bin/ln -s -v "$WFILE" "/etc/$mketc"
  fi
done

