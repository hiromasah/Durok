#!/bin/sh
# base-files version 3.9-3

manifest=/etc/preremove/base-files-manifest.lst

[ -f ${manifest} ] || (echo "Unable to find manifest file" && exit 0)

while read f; do
    fSrc=/etc/defaults/${f}
    fDest=/${f}
    if [ ! -e ${fDest} -a ! -L ${fDest} ]; then
        /bin/echo "Using the default version of ${fDest} (${fSrc})"
	/bin/mkdir -p `dirname ${fDest}`
        /bin/touch ${fDest}
        /bin/cp ${fSrc} ${fDest}
    else
        echo ${fDest} is already in existance, not overwriting.
    fi
done < ${manifest}

