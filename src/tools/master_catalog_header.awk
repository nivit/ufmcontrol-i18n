#
# $Id: master_catalog_header.awk,v 1.1 2003/10/31 16:18:00 nivit Exp $
#
# Print the header of the master catalog
#
BEGIN {

	print "$";
	print "$ THIS FILES IS GENERATED/UPDATED AUTOMATICALLY";
	print "$";
	print "$ See gencat(1) for more information about its format.";
	print "$";
	print "\$quote \"";
}
