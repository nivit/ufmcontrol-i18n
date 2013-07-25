#
# $Id: make_master_catalog.awk,v 1.1 2003/10/31 16:18:00 nivit Exp $
#
# Make a message "master" catalog file
#
BEGIN {
	old_catid="";
	id = 0;
	mn = 0;
	FS = ":";
}

{
	mn++;
	cur_catid = $1;

	if (cur_catid != old_catid) {
		old_catid = cur_catid; ++id;
		str_catid = sprintf("%s", toupper(cur_catid));
		sub(/\./, "_", str_catid);
		print "\$set", id, str_catid "_SET";
	};

	left = match($0, /NLS\(\".*\"\)/) + 4;
	mesg_right = substr($0, left);
	right = match(mesg_right, /[^\\]\"\)/) + 1;
	mesg = substr(mesg_right, 2, right - 2);

	print "$", $1 ": line", $2;

	tmp_file = sprintf("mesg%s.tmp", mn);
	tmp_file_md5 = sprintf("mesg%s_md5.tmp", mn);
	print mesg > tmp_file;
	md5_cmd = sprintf("md5 < %s > %s", tmp_file, tmp_file_md5);
	system(md5_cmd);
	getline md5_checksum < tmp_file_md5;

	close(tmp_file);
	close(tmp_file_md5);

	printf "$ MD5 (\"%s\") = %s\n", mesg, md5_checksum;
	printf "%s \"%s\"\n", $2, mesg;
}

END {
	system("rm mesg*.tmp");
}
