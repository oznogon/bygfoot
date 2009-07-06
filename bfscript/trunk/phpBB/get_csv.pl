#! /usr/bin/perl
my $url = 'http://bygfoot.sourceforge.net/getUserList.php';

use LWP 5.64;
  my $browser = LWP::UserAgent->new;
  
  my $response = $browser->post( $url,
    [ 'u_name' => '<fill_in>',
      'u_password' => '<fill_in>',
    ]
  );


  # Then go do things with $content, like this:
print $response->content;

