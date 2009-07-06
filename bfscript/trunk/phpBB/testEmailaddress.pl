#! /usr/bin/perl
use LWP::Simple;
use XML::Simple;

open(FILE, "phpbb_users.csv");
while (<FILE>) {
	@mailFields = split(/;/, $_);
        if ($mailFields[0]!=-1){
		chomp($mailFields[2]);
		$url='http://www.stopforumspam.com/api?email='.$mailFields[2];
		$text = get $url;
		my $xs1 = XML::Simple->new();
		my $doc = $xs1->XMLin($text);
		if ($doc->{appears} eq "no") {
			print "$mailFields[2] is OK\n";
		}
        }
}
close FILE;
