��    �      �  �         �  ~   �  p  0  �   �  F   P  I   �     �  0        2  L   D  J   �  -   �  5   
  (   @  L   i     �  D   �  ?     B   U  D   �     �  I   �  =   G  A   �  J   �  =     8   P  9   �  C   �  F     (   N  @   w  B   �  M   �  K   I  8   �  ,   �  J   �  9   F  /   �  0   �  K   �  $   -  G   R  )   �  V  �  9        U  G   t  A   �  <   �  .   ;  C   j  ,   �  ?   �  <     E   X  B   �  %   �  5     F   =  .   �  >   �  >   �  A   1   8   s   3   �   #   �   /   !  D   4!  /   y!  4   �!  �   �!  !   �"     �"  !   #     %#  F   E#  =   �#  I   �#  &   $     ;$     Z$  I   r$  1   �$  &   �$     %     ,%     I%     e%  $   }%     �%     �%     �%     �%     �%     &     "&  #   2&     V&  �   q&  =   #'  '   a'     �'     �'     �'  %   �'     �'     �'     (     (     '(  "   4(  4   W(     �(  .   �(     �(  ;   �(  3   +)  /   _)  +   �)  '   �)  #   �)     *     '*     C*  *   S*     ~*  )   �*  !   �*  '   �*  )   +     /+     F+      V+      w+     �+     �+  	   �+     �+     �+     �+     ,  1   ,  2   I,  0   |,     �,  #   �,     �,  )   -  1   2-  -   d-     �-     �-     �-     �-     �-     �-  "   .  %   3.     Y.     g.     s.     �.  	   �.     �.     �.     �.     �.  ,   �.     /     /     //  
   C/     N/  |  [/  t   �0  �  M1  �   �2  I   z3  O   �3  !   4  2   64     i4  n   {4  P   �4  0   ;5  ;   l5  0   �5  f   �5     @6  G   ^6  D   �6  L   �6  K   87  /   �7  g   �7  ;   8  J   X8  d   �8  �   9  ;   �9  >   �9  C   :  I   Q:  (   �:  B   �:  C   ;  o   K;  x   �;  ;   4<  9   p<  u   �<  7    =  0   X=  7   �=  �   �=  ,   F>  8   s>  )   �>  �  �>  3   �@      �@  I   A  4   WA  =   �A  .   �A  =   �A      7B  >   XB  ;   �B  >   �B  D   C  +   WC  7   �C  H   �C  3   D  C   8D  8   |D  9   �D  5   �D  ;   %E  !   aE  0   �E  J   �E  1   �E  <   1F  �   nF     ZG  !   yG  %   �G  $   �G  P   �G  B   7H  j   zH  &   �H     I     ,I  O   HI  H   �I  2   �I     J     *J     EJ     ]J  "   qJ     �J     �J     �J     �J  '   �J  $   K     -K  "   =K      `K  �   �K  E   +L  ,   qL     �L     �L     �L  ,   �L     �L     M     $M     7M     JM  *   VM  <   �M  "   �M  :   �M     N  4   .N  -   cN  )   �N  %   �N  !   �N     O     !O     ;O     QO  ,   ]O     �O  %   �O     �O  )   �O  +   P     >P     WP  '   fP  %   �P     �P     �P     �P     �P     Q     Q     2Q  1   LQ  -   ~Q  *   �Q  '   �Q  %   �Q     %R  8   BR  =   {R  6   �R     �R     S     *S     ?S     OS     \S  %   yS  1   �S     �S     �S     �S     T     T     T     4T     :T     NT  "   `T     �T  %   �T     �T     �T     �T        +   �               9   X          o   �   �       u   ~   �       q   $      �   �       a   �      d   r   0      |   j   W      e          �              F   .      �       �       
   c      H   R         Z           �   �   w   4   @         _   z   '   �   g       �      5   �   l          �   ^   �   �   �       �   �   �   G   !   ]                  m           y       �   J   *   �       "   �   I      �   8          (              Y   =          :   �   f   ?      [   �   2   �       k   7   �   �   A       {   Q   �   t   -   V               }   �       �      h       C      K   T      M   �       3   6   /   P                     `   B   O   b   >   �   ;           S   D   i   #   �   �   �   ,   \               U   s   )   <   �       1       E       N       �   �              �           %   x   &   �   L          �   �           	   �           v       p   �       n      Either GFMT or LFMT may contain:
    %%  %
    %c'C'  the single character C
    %c'\OOO'  the character with octal code OOO   GFMT may contain:
    %<  lines from FILE1
    %>  lines from FILE2
    %=  lines common to FILE1 and FILE2
    %[-][WIDTH][.[PREC]]{doxX}LETTER  printf-style spec for LETTER
      LETTERs are as follows for new group, lower case for old group:
        F  first line number
        L  last line number
        N  number of lines = L-F+1
        E  F-1
        M  L+1   LFMT may contain:
    %L  contents of line
    %l  contents of line, excluding any trailing newline
    %[-][WIDTH][.[PREC]]{doxX}n  printf-style spec for input line number   LTYPE is `old', `new', or `unchanged'.  GTYPE is LTYPE or `changed'.   Skip the first SKIP1 bytes of FILE1 and the first SKIP2 bytes of FILE2. %s %s differ: byte %s, line %s
 %s %s differ: byte %s, line %s is %3o %s %3o %s
 %s: diff failed:  --GTYPE-group-format=GFMT  Similar, but format GTYPE input groups with GFMT. --LTYPE-line-format=LFMT  Similar, but format LTYPE input lines with LFMT. --binary  Read and write data in binary mode. --diff-program=PROGRAM  Use PROGRAM to compare files. --from-file and --to-file both specified --from-file=FILE1  Compare FILE1 to all operands.  FILE1 can be a directory. --help  Output this help. --horizon-lines=NUM  Keep NUM lines of the common prefix and suffix. --ignore-file-name-case  Ignore case when comparing file names. --line-format=LFMT  Similar, but format all input lines with LFMT. --no-ignore-file-name-case  Consider case when comparing file names. --normal  Output a normal diff. --speed-large-files  Assume large files and many scattered small changes. --strip-trailing-cr  Strip trailing carriage return on input. --tabsize=NUM  Tab stops are every NUM (default 8) print columns. --to-file=FILE2  Compare all operands to FILE2.  FILE2 can be a directory. --unidirectional-new-file  Treat absent first files as empty. -3  --easy-only  Output unmerged nonoverlapping changes. -A  --show-all  Output all changes, bracketing conflicts. -B  --ignore-blank-lines  Ignore changes whose lines are all blank. -D NAME  --ifdef=NAME  Output merged file to show `#ifdef NAME' diffs. -D option not supported with directories -E  --ignore-tab-expansion  Ignore changes due to tab expansion. -E  --show-overlap  Output unmerged changes, bracketing conflicts. -H  --speed-large-files  Assume large files and many scattered small changes. -I RE  --ignore-matching-lines=RE  Ignore changes whose lines all match RE. -L LABEL  --label=LABEL  Use LABEL instead of file name. -N  --new-file  Treat absent files as empty. -S FILE  --starting-file=FILE  Start with FILE when comparing directories. -T  --initial-tab  Make tabs line up by prepending a tab. -W  --ignore-all-space  Ignore all white space. -X  Output overlapping changes, bracketing them. -X FILE  --exclude-from=FILE  Exclude files that match any pattern in FILE. -a  --text  Treat all files as text. -b  --ignore-space-change  Ignore changes in the amount of white space. -b  --print-bytes  Print differing bytes. -c  -C NUM  --context[=NUM]  Output NUM (default 3) lines of copied context.
-u  -U NUM  --unified[=NUM]  Output NUM (default 3) lines of unified context.
  --label LABEL  Use LABEL instead of file name.
  -p  --show-c-function  Show which C function each change is in.
  -F RE  --show-function-line=RE  Show the most recent line matching RE. -d  --minimal  Try hard to find a smaller set of changes. -e  --ed  Output an ed script. -e  --ed  Output unmerged changes from OLDFILE to YOURFILE into MYFILE. -i  --ignore-case  Consider upper- and lower-case to be the same. -i  --ignore-case  Ignore case differences in file contents. -i  Append `w' and `q' commands to ed scripts. -i SKIP  --ignore-initial=SKIP  Skip the first SKIP bytes of input. -i SKIP1:SKIP2  --ignore-initial=SKIP1:SKIP2 -l  --left-column  Output only the left column of common lines. -l  --paginate  Pass the output through `pr' to paginate it. -l  --verbose  Output byte numbers and values of all differing bytes. -m  --merge  Output merged file instead of ed script (default -A). -n  --rcs  Output an RCS format diff. -n LIMIT  --bytes=LIMIT  Compare at most LIMIT bytes. -o FILE  --output=FILE  Operate interactively, sending output to FILE. -q  --brief  Output only whether files differ. -r  --recursive  Recursively compare any subdirectories found. -s  --quiet  --silent  Output nothing; yield exit status only. -s  --report-identical-files  Report when two files are the same. -s  --suppress-common-lines  Do not output common lines. -t  --expand-tabs  Expand tabs to spaces in output. -v  --version  Output version info. -w  --ignore-all-space  Ignore all white space. -w NUM  --width=NUM  Output at most NUM (default 130) print columns. -x  --overlap-only  Output overlapping changes. -x PAT  --exclude=PAT  Exclude files that match PAT. -y  --side-by-side  Output in two columns.
  -W NUM  --width=NUM  Output at most NUM (default 130) print columns.
  --left-column  Output only the left column of common lines.
  --suppress-common-lines  Do not output common lines. Common subdirectories: %s and %s
 Compare files line by line. Compare three files line by line. Compare two files byte by byte. Exit status is 0 if inputs are the same, 1 if different, 2 if trouble. Exit status is 0 if successful, 1 if conflicts, 2 if trouble. FILES are `FILE1 FILE2' or `DIR1 DIR2' or `DIR FILE...' or `FILE... DIR'. File %s is a %s while file %s is a %s
 Files %s and %s are identical
 Files %s and %s differ
 If --from-file or --to-file is given, there are no restrictions on FILES. If a FILE is `-' or missing, read standard input. If a FILE is `-', read standard input. Invalid back reference Invalid character class name Invalid collation character Invalid content of \{\} Invalid preceding regular expression Invalid range end Invalid regular expression Memory exhausted No match No newline at end of file No previous regular expression Only in %s: %s
 Premature end of regular expression Regular expression too big SKIP values may be followed by the following multiplicative suffixes:
kB 1000, K 1024, MB 1,000,000, M 1,048,576,
GB 1,000,000,000, G 1,073,741,824, and so on for T, P, E, Z, Y. SKIP1 and SKIP2 are the number of bytes to skip in each file. Side-by-side merge of file differences. Success Torbjorn Granlund Trailing backslash Try `%s --help' for more information. Unknown system error Unmatched ( or \( Unmatched ) or \) Unmatched [ or [^ Unmatched \{ Usage: %s [OPTION]... FILE1 FILE2
 Usage: %s [OPTION]... FILE1 [FILE2 [SKIP1 [SKIP2]]]
 Usage: %s [OPTION]... FILES
 Usage: %s [OPTION]... MYFILE OLDFILE YOURFILE
 Written by %s and %s.
 Written by %s, %s, %s,
%s, %s, %s, %s,
%s, %s, and others.
 Written by %s, %s, %s,
%s, %s, %s, %s,
%s, and %s.
 Written by %s, %s, %s,
%s, %s, %s, %s,
and %s.
 Written by %s, %s, %s,
%s, %s, %s, and %s.
 Written by %s, %s, %s,
%s, %s, and %s.
 Written by %s, %s, %s,
%s, and %s.
 Written by %s, %s, %s,
and %s.
 Written by %s, %s, and %s.
 Written by %s.
 `-' specified for more than one input file block special file both files to be compared are directories cannot compare `-' to a directory cannot compare file names `%s' and `%s' cannot interactively merge standard input character special file cmp: EOF on %s
 conflicting %s option value `%s' conflicting output style options conflicting tabsize options conflicting width options directory extra operand `%s' fifo incompatible options input file shrank internal error: invalid diff type in process_diff internal error: invalid diff type passed to output internal error: screwup in format of diff blocks invalid --bytes value `%s' invalid --ignore-initial value `%s' invalid context length `%s' invalid diff format; incomplete last line invalid diff format; incorrect leading line chars invalid diff format; invalid change separator invalid horizon length `%s' invalid tabsize `%s' invalid width `%s' memory exhausted message queue missing operand after `%s' options -l and -s are incompatible pagination not supported on this host program error read failed regular empty file regular file semaphore shared memory object socket stack overflow standard output subsidiary program `%s' could not be invoked symbolic link too many file label options typed memory object weird file write failed Project-Id-Version: GNU diffutils 2.8.7
Report-Msgid-Bugs-To: bug-diffutils@gnu.org
POT-Creation-Date: 2010-02-11 10:26+0100
PO-Revision-Date: 2004-04-14 23:26+0100
Last-Translator: Petr Ko�vara <petr.kocvara@nemfm.cz>
Language-Team: Czech <translation-team-cs@lists.sourceforge.net>
MIME-Version: 1.0
Content-Type: text/plain; charset=ISO-8859-2
Content-Transfer-Encoding: 8-bit
   Jeden z GFMT nebo LFMT m��e obsahovat:
    %%  %
    %c'C'  jeden znak C
    %c'\000'  znak s osmi�kov�m k�dem 000   GFMT m��e obsahovat:
    %<  ��dky ze SOUBOR1
    %>  ��dky ze SOUBOR2
    %=  ��dky spole�n� pro SOUBOR1 i SOUBOR2
    %[-][DELKA][.[PRES]]{doxX}ZNAK  form�t stylu printf pro ZNAK
      ZNAKy d�le jsou pro novou skupinu, mal�mi p�smeny pro starou skupinu:
        F  ��slo prvn�ho ��dku
        L  ��slo posledn�ho ��dku
        N  po�et ��dk� = L-F+1
        E  F-1
        M  L+1   LFMT m��e obsahovat:
    %L  obsah ��dku
    %l  obsah ��dku, s vyj�mkou znaku konce ��dku
    %[-][���KA][.[P�ES]]{doxX}c  form�t stylu printf pro ��slo vstupn�ho ��dku   LTYPE je `star�', `nov�' nebo `nezm�n�n'. GTYPE je LTYPE nebo `zm�n�n'.   P�esko�� prvn�ch N1 bajt� souboru SOUBOR1 a prvn�ch N2 bajt� souboru SOUBOR2. %s %s se li��: bajt %s, ��dek %s
 %s %s se li��: bajt %s, ��dek %s je %3o %s %3o %s
 %s: diff selhal:  --GTYPE-group-format=FMTS  Podobn�, ale form�tuje vstupn� skupiny GTYPE
                           podle FMTS. --LTYPE-line-format=FMTR  Podobn�, ale form�tuje vstupn� ��dky LTYPE podle FMTR. --binary  �te a zapisuje data v bin�rn�m re�imu. --diff-program=PROGRAM  Pou�ij PROGRAM k porovn�n� soubor�. parametry --from-file i to-file pou�ity najednou --from-file=SOUBOR1  Porovn� SOUBOR1 se v�emi operandy. SOUBOR1 m��e b�t
                     adres��. --help  Vyp��e tuto n�pov�du. --horizon-lines=PO�ET  Ponech� PO�ET shodn�ch ��dk� p�edpony a p��pony. --ignore-file-name-case  Ignoruje velikost p�smen v n�zvech soubor�. --line-format=FMTR  Podobn�, ale form�tuje v�echny vstupn� ��dky podle FMTR. --no-ignore-file-name-case  Bere v potaz velikost p�smen v n�zvech soubor�. --normal  V�stup bude v norm�ln�m diff form�tu. --speed-large-files  P�edpokl�d� velk� soubory a mnoho rozpt�len�ch
                     drobn�ch zm�n. --strip-trailing-cr  Odstran� ukon�ovac� znak CR na vstupu. --tabsize=PO�  Tab zastavuje ka�d�ch PO� (implicitn� 8) tiskov�ch sloupc�. --to-file=SOUBOR2  Porovn� v�echny operandy se SOUBOR2. SOUBOR2 m��e b�t
                   adres��. --unidirectional-new-file  P�i porovn�v�n� adres��� pova�uje neexistuj�c�
                           soubory v prv�m adres��i za pr�zdn�. -3  --easy-only  Vyp��e neslou�en� nep�ekr�vaj�c� se zm�ny. -A  --show-all  Vyp��e v�echny rozd�ly, konflikty v z�vork�ch. -B  --ignore-blank-lines  Ignoruje zm�ny v p��pad� pr�zdn�ch ��dk�. -D JMENO  --ifdef=JMENO  Vyp��e slou�en� soubor s rozd�ly `#ifdef JMENO'. -D p�ep�na� nepodporuje pr�ci s adres��i -E  --ignore-tab-expansion  Ignoruje zm�ny v odsazen� tabel�torem. -E  --show-overlap  Vyp��e neslou�en� zm�ny, konflikty v z�vork�ch. -H  --speed-large-files  P�edpokl�d� velk� soubory a mnoho rozpt�len�ch
                         drobn�ch zm�n. -I RV  --ignore-matching-lines=RV  Ignoruje zm�ny na v�ech ��dc�ch
                                   odpov�daj�c�ch RV. -L POPIS  --label=POPIS  Pou�ije POPIS m�sto jm�na souboru. -N  --new-file  Neexistuj�c� soubory pova�uje za pr�zdn�. -S SOUBOR  --starting-file=SOUBOR  P�i porovn�v�n� adres��� za�ne souborem
                                   SOUBOR. -T  --initial-tab  Na za��tek ��dk� se vlo�� tabul�tor. -W  --ignore-all-space  Ignoruje v�echny mezery. -X  Vyp��e p�ekr�vaj�c� se zm�ny, uzav�en� v z�vork�ch. -X SOUBOR  --exclude-from=SOUBOR  Vynech� soubory, kter� odpov�daj�
                                  libovoln�mu vzorku ze SOUBORu. -a  --text  Pokl�d� v�echny soubory za text. -b  --ignore-space-change  Ignoruje zm�ny v po�tu mezer. -b  --print-bytes  Vyp��e rozd�ln� bajty. -c  -C PO�  --context[=PO�]  Vyp��e PO� (implicitn� 3) ��dky kop�rovan�ho
                             kontextu.
-u  -U PO�  --unified[=PO�]  Vyp��e PO� (implicitn� 3) ��dky sjednocen�ho
                             kontextu.
  -L POPIS  --label POPIS  Pou�ije POPIS m�sto jm�na souboru.
  -p  --show-c-function  U ka�d� zm�ny vyp��e jm�no C funkce, ve kter� se
                         tato zm�na nach�z�.
  -F RV  --show-function-line=RV  Vyp��e nejnov�j�� ��dky odpov�daj�c� RV. -d  --minimal  Pokus� se nal�zt nejmen�� sadu zm�n. -e  --ed  Vytvo�� skript pro ed. -e  --ed  Vyp��e nespojen� zm�ny ze STARYSOUBOR k VASSOUBOR do MUJSOUBOR. -i  --ignore-case  Nerozli�uje velk� a mal� p�smena. -i  --ignore-case  Ignoruje velikost p�smen v obsahu souboru. -i  P�id� p��kazy `w' a `q' do skript� pro ed. -i N  --ignore-initial=N  Ignoruje prvn�ch N bajt� na vstupu. -i N1:N2  --ignore-initial=N1:N2 -l  --left-column  Vyp��e pouze lev� sloupec spole�n�ch ��dk�. -l  --paginate  V�stup projde p�es `pr' pro p�estr�nkov�n�. -l  --verbose  Vyp��e pozice a hodnoty v�ech rozd�ln�ch bajt�. -m  --merge  Vyp��e spojen� soubor m�sto ed skriptu (implicitn� -A). -n  --rcs  V�stup bude ve form�tu RCS diff. -n LIMIT  --bytes=LIMIT  Porovn� maxim�ln� LIMIT bajt�. -o SOUBOR  --output=SOUBOR  Interaktivn� pr�ce, v�stup p�jde do SOUBORu. -q  --brief  V�stup pouze p�i rozd�ln�ch souborech. -r  --recursive  Rekurz�vn� porovn�n� v�ech nalezen�ch podadres���. -s  --quiet  --silent  ��dn� v�stup; vr�t� pouze status. -s  --report-identical-files  Uvede pouze shodn� soubory. -s  --suppress-common-lines  Nevypisuje shodn� ��dky. -t  --expand-tabs  Ve v�stupu p�evede tabul�tory na mezery. -v  --version  Informace o verzi. -w  --ignore-all-space  Ignoruje v�echny mezery. -w PO�  --width=PO�  Vyp��e maxim�ln� PO� (implicitn� 130) znak� na ��dek. -x  --overlap-only  Vyp��e p�ekr�vaj�c� se zm�ny. -x VZOR  --exclude=VZOR  Vynech� soubory odpov�daj�c� VZORu. -y  --side-by-side  V�stup ve dvou sloupc�ch.
  -W PO�  --width=PO�  Vyp��e maxim�ln� PO� (implicitn� 130) znak� na ��dek.
  --left-column  Vyp��e pouze lev� sloupec spole�n�ch ��dk�.
  --suppress-common-lines  Nevyp��e spole�n� ��dky. Spole�n� podadres��e: %s a %s
 Porovn�n� soubor� ��dek po ��dku. Porovn�n� t�� soubor� ��dek po ��dku. Porovn�n� dvou sobor� bajt po bajtu. N�vratov� k�d je roven 0 pokud jsou vstupy shodn�, 1 pokud se li��a 2 p�i chyb�. N�vratov� hodnota je 0 p�i �sp�chu, 1 p�i konfliktu a 2 p�i chyb�. SOUBORY jsou `SOUBOR1 SOUBOR2' nebo `ADRESAR1 ADRESAR2' nebo
`ADRESAR SOUBOR...' nebo `SOUBOR... ADRESAR'. Soubor %s je %s pokud soubor %s je %s
 Soubory %s a %s jsou identick�
 Soubory %s a %s jsou r�zn�
 Pokud je uveden --from-file nebo --to-file, pak nejsou u SOUBOR� ��dn� omezen�. Pokud SOUBOR bude `-' nebo nebude existovat, bude �ten standardn� vstup. Pokud SOUBOR bude `-', bude �ten standardn� vstup. Neplatn� zp�tn� odkaz Neplatn� jm�no t��dy znak� Neplatn� znak porovn�n� Neplatn� obsah \{\} Neplatn� p�edchoz� regul�rn� v�raz Neplatn� konec rozsahu Neplatn� regul�rn� v�raz Pam� vy�erp�na ��dn� shoda Chyb� znak konce ��dku na konci souboru P�edchoz� regul�rn� v�raz neexistuje Pouze v %s: %s
 P�ed�asn� konec regul�rn�ho v�razu Regul�rn� v�raz je p��li� dlouh� Hodnoty SKIP mohou b�t dopln�ny n�sleduj�c�mi p��ponami:
kB 1000, K 1024, MB 1,000,000, M 1,048,576,
GB 1,000,000,000, G 1,073,741,824, a stejn� tak i pro T, P, E, Z, Y. N1 a N2 ud�vaj� po�et bajt�, kter� budou ignorov�ny v ka�d�m souboru. Aplikace zm�n v souboru v m�du `vedle sebe`. Hotovo Torbjorn Granlund Koncov� zp�tn� lom�tko V�ce informac� z�sk�te p��kazem `%s --help'. Nezn�m� chyba syst�mu Nep�rov� ( nebo \( Nep�rov� ) nebo \) Nep�rov� [ nebo ]^ Nep�rov� \{ Pou�it�: %s [P�EP�NA�]... SOUBOR1 SOUBOR2
 Pou�it�: %s [P�EP�NA�]... SOUBOR1 [SOUBOR2 [SKIP1 [SKIP2]]]
 Pou�it�: %s [P�EP�NA�]... SOUBORY
 Pou�it�: %s [P�EP�NA�]... MUJSOUBOR STARYSOUBOR VASSOUBOR
 Auto�i: %s a %s.
 Auto�i: %s, %s, %s,
%s, %s, %s, %s,
%s, %s a dal��.
 Auto�i: %s, %s, %s,
%s, %s, %s, %s,
%s a %s.
 Auto�i: %s, %s, %s,
%s, %s, %s, %s
a %s.
 Auto�i: %s, %s, %s,
%s, %s, %s a %s.
 Auto�i: %s, %s, %s,
%s, %s a %s.
 Auto�i: %s, %s, %s,
%s a %s.
 Auto�i: %s, %s, %s
a %s.
 Auto�i: %s, %s a %s.
 Autor: %s.
 `-' zad�no pro v�ce ne� jeden vstupn� soubor speci�ln� blokov� soubor oba soubory k porovn�n� jsou adres��i `-' s adres��em nelze porovnat nemohu porovnat jm�na soubor� `%s' a `%s' interaktivn� nelze standardn� vstup slou�it speci�ln� znakov� soubor cmp: EOF v %s
 pro p�ep�na� %s konfliktn� hodnota `%s' konfliktn� p�ep�na�e pro styl v�stupu konfliktn� p�ep�na�e tabsize konfliktn� volby ���ky v�stupu adres�� operand `%s' je nadbyte�n� roura nekompatibiln� p�ep�na�e vstupn� soubor se zmen�il vnit�n� chyba: nespr�vn� typ diffu v process_diff vnit�n� chyba: nespr�vn� typ diffu pro v�stup vnit�n� chyba: chyba ve form�tu diff blok� neplatn� hodnota p�ep�na�e --bytes `%s' neplatn� hodnota --ignore-inital `%s' neplatn� d�lka kontextu `%s' neplatn� form�t diff souboru; nekompletn� posledn� ��dek neplatn� form�t diff souboru; nespr�vn� �vodn� znaky na ��dku neplatn� form�t diff souboru; neplatn� odd�lova� zm�ny neplatn� d�lka obzoru `%s' nespr�vn� hodnota tabsize `%s' nespr�vn� d�lka `%s' pam� vy�erp�na fronta zpr�v po `%s' je nespr�vn� operand p�ep�na�e -l a -s nejsou kompatibiln� p�estr�nkov�n� nen� na tomto po��ta�i podporov�no chyba programu �ten� selhalo oby�ejn� pr�zdn� soubor oby�ejn� soubor semafor objekt sd�len� pam�ti soket p�ete�en� z�sobn�ku standardn� v�stup pomocn� program `%s' nelze spustit symbolick� odkaz p��li� mnoho p�ep�na�� popisu souboru objekt typov� pam�ti soubor nezn�m�ho typu z�pis selhal 