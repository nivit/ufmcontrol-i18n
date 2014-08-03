#
# $Id: update_slave_catalog.awk,v 1.1 2003/10/31 16:18:00 nivit Exp $
#
# Update slave nls catalog file
#
# Add new lines, delete missing lines in master file,
# update message number line in slave file.
#

BEGIN {

	if (master == "" || slave == "") usage();

	if (master == slave) usage();

	ret = make_md5_file();

	if (ret != 0) {
		print "\"update_slave_catalogs.awk\"", \
			"can not make MD5 files";
		exit 1;
	}

	# required by find_new_messages()
	new_message_file = "new_messages.txt"

	ret = find_new_messages(new_message_file);
	if (ret != 0) {
		printf "\"update_slave_catalogs.awk\" " \
			"can not compare " \
			"%s.md5 and %s.md5 files\n", \
			master, slave;
		exit 1;
	}

	close(master);
	close(slave);

}

# print any comment, statement...
/^\$.*/ {
	print $0;
}

#/^\$ MD5\(\".*\"\) = [[:alnum:]]*/ {
/^\$ MD5.*/ {

	mesg = is_new($0);

	checksum = sprintf("%s", $0);

	close(new_message_file);

	getline;

	if (mesg == 0) {
		print_old_message($0, checksum);
	} else 
	print;
}

#        prints usage information and exit
function usage() {

	print "usage: awk -f update_slave_catalogs.awk", \
		"-v master=MASTER_FILE", \
		"-v slave=SLAVE_FILE",
		"MASTER_FILE";
		exit 1;
}


#        extract all lines starting with $ MD5 from "master" and "slave"
#        files; sort them, and save the lines in separated files.
function make_md5_file () {

	master_md5_cmd = sprintf("grep -e '^$ MD5 ' %s > %s.md5",
	    master, master);
	error = system(master_md5_cmd);

	if (error > 0) {
		print master_md5_cmd;
		return 1;
	}

	master_sort_cmd = sprintf("sort %s.md5 > %s.sort",
	    master, master);
	error = system(master_sort_cmd);

	if (error > 1) return error;

	slave_md5_cmd = sprintf("grep -e '^$ MD5 ' %s > %s.md5",
	    slave, slave);
	error = system(slave_md5_cmd);

	if (error > 0) return error;

	slave_sort_cmd = sprintf("sort %s.md5 > %s.sort",
	    slave, slave);
	error = system(slave_sort_cmd);

	if (error > 1) return error;

	return 0;
}


#        This function calls simply the comm(1) utility
#        with `master`.md5 and `slave`.md5 files as
#        arguments.
function find_new_messages(new_messages_file) {

	find_cmd = sprintf("comm -23 %s.sort %s.sort > %s",
	    master, slave, new_messages_file);

	find_ret = system(find_cmd);

	return find_ret;
}

#        check if the message is been already translated
function is_new(line_checksum,    new_message, checksum) {

	# new_message = 0;

	while ((getline checksum < new_message_file) > 0) {
		if (checksum == line_checksum) {
			# is a new message
			new_message = 1;
			break;
		}
	}

	return new_message;
}

function print_old_message(new_mesg, old_checksum,    found, old_message, number_line) {
	number_line = $1;

	while ((getline old_message < slave) > 0) {

		if (old_message == old_checksum) {
			# the next line is the message
			getline  < slave; # errors?
			$1 = ""
			print number_line $0;
			break;
		}
	}

	close(slave);
}

END {

	# remove temporary files
	rm_cmd = sprintf("rm -f *.sort *.md5 new_messages.txt");
	system(rm_cmd);
}
