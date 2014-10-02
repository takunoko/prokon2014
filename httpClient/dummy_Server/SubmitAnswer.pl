#!/usr/local/bin/perl

use strict;
use warnings;

use CGI;

my $q=CGI->new();
my @param = $q->param();

print $q->header(-type => 'text/plain' , -charset=>'utf-8');

foreach my $key(@param){
 print "$key : " .$q->param($key) ."\n";
}
