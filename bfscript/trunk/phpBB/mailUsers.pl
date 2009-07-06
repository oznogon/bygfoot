#! /usr/bin/perl
open(FILE, "phpbb_users.csv");
print "#! /bin/sh\n";
while (<FILE>) {
	@mailFields = split(/;/, $_);
        if ($mailFields[0]!=-1){
		print "echo -e 'Hello,\\n\\nYour account has been accepted by the administrator, you can activate it by going to the link http://bygfoot.sourceforge.net/phpBB2/profile.php\?mode=activate&u=".$mailFields[0]."&act_key=".$mailFields[1]."\\n\\nYou need to do this within 30 days or your account will be removed.\\n\\nThe Bygfoot team' | mailx -s '[bygfoot] Your account has been accepted by the administrator' ".$mailFields[2]."\n";
	}
}
close FILE;
