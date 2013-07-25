#
# $Id: find_translator.awk,v 1.1 2003/10/31 16:18:00 nivit Exp $
#
# Add information to message catalog file about the translator
#
BEGIN	{FS="\t"; print "$"; found=0;}

	{if ($1 == translator) {print "$ Translation by", $NF; found=1}}

END	{if (found==0) print "$ Not yet translated"}
