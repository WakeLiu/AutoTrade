#!/usr/bin/perl

use utf8;
use Encode;
use strict;
use warnings;
use LWP::UserAgent;
use MIME::Lite;
use open qw(:std :utf8);
my $response_err=0;
my $temp;
my $to_all=1;
while(1)
{
    sleep(5);
    my $datestring = localtime();
    my $ua = LWP::UserAgent->new( ssl_opts => { verify_hostname => 0 } );
    my $response = $ua->get('https://www.ptt.cc/bbs/Stock/index.html');
    if ( $response->is_success ) {
        $response_err=0;
        my $web = $response->decoded_content;
        my $author="chengwaye";
        if($web =~/$author/)
        {
            my $to = 'wakeliu@gmail.com';
            my $cc;
            if ($to_all==1){$cc= 'lai_162@hotmail.com, duke_il@hotmail.com, dbfddark@yahoo.com.tw, jimmy02140@gmail.com';}
            my $from = 'StockGod@wake.liu.com';
            my $subject = '[PTT] '.$author.' has posted an article!!!';
            my $str =substr ($web,index($web,"$author")-200);
            $str = substr($str,index($str,"["),index($str,"</a>")-index($str,"["));
            my $message = $str;
            if ($str ne $temp) 
            {
                print $author." has posted an article.\n";
                $temp = $str;
                print $str;
            }
            else 
            {
                print $datestring.": Nobody appears, please keep waiting...\n";
                next;
            }
            my $msg = MIME::Lite->new(
            From     => $from,
            To       => $to,
            Cc       => $cc,
            Subject  => $subject,
            Data     => $message
            );
            
            $msg->send;
            print "Email Sent Successfully\n";
        }
        else
        {
            print $datestring.": Nobody appears, please keep waiting...\n";
            #print $response->decoded_content;
        }
    }
    else {
        $response_err++;
        if($response_err==10)
        {
            die $datestring.": Dead link.\n"
            #die $response->status_line;
        }
    }
    
}
