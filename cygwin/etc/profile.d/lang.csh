# if no locale variable is set, indicate terminal charset via LANG
if ( $?LC_ALL == 0 && $?LC_CTYPE == 0 && $?LANG == 0 ) setenv LANG C.UTF-8

