#!/usr/bin/perl
use utf8;
use LWP::Simple;
binmode(STDIN, ':encoding(utf8)');
binmode(STDOUT, ':encoding(utf8)');
binmode(STDERR, ':encoding(utf8)');

use HTML::Parse;
use HTML::Element;
use LWP::UserAgent;
use strict;
use warnings;
use Spreadsheet::WriteExcel; #寫入Excel資料
use Spreadsheet::ParseExcel; #讀取Excel資料

my ($sec, $min, $hour, $day, $mon, $year) = localtime(time);
$year+= 1900;
$mon += 1;

my @data = pick(34096);

for(my $i=0; $i<@data; $i++)
{
    my $id=$data[$i][0];
    my $name =$data[$i][1];
    my $workbook = Spreadsheet::WriteExcel->new($id."_".$name."_".$year.$mon.$day.".xls");#打開Excel檔
    
    my @stock = history($id,$workbook);
    my @eps = nlog($id,$workbook);
    #    if ((@stock) && $stock [0][13]>0 && $stock [1][13]<0) {print "Notice ".$id." for OSC.\n"}
    
    print $id.$name. " is done.\n";
    
}

system("mkdir ".$year.$mon.$day);
system("mv *".$year.$mon.$day.".xls ".$year.$mon.$day);
print "Job Finish!!!\n";

sub pick{
    my ($id)=@_;
    my $web = get 'http://stock.wespai.com/p/'.$id;

    my $str =substr ($web,index($web,"<th>代號<\/th>"));
    my $title_str = substr($str,0,index($str,"</tr>"));
    my @title = $title_str =~ /<th>(.+)<\/th>/g;
    my @stock;
    foreach my $i( @title)
    {
        if ($i=~ /<a href/)
        {
            $i=~ />(.+)<\/a>/;
            $i = $1;
        }
        print $i."\t"
    }
    print "\n";

    for (my $i=0;$str =~/<tr>/ ;$i++)
    {
        $str =substr ($str,index($str,"<tr>")+5);
        my $stock_temp = substr($str,0, index($str,"</tr>"));
        my @num = $stock_temp =~ /<td>(.*)<\/td>/g;
        for (my $j=0;$j<@num;$j++)
        {
            if ($num[$j]=~ /<a href/)
            {
                $num[$j]=~ />(.+)<\/a>/;
                $num[$j] = $1;
            }
            print $num[$j]."\t";
            $stock[$i][$j]=$num[$j];
        }
        print "\n";
    }
    my $workbook = Spreadsheet::WriteExcel->new("股市_".$year.$mon.$day.".xls");#打開Excel檔
    my $worksheet = $workbook->add_worksheet("今日分析");
    for (my $i =0; $i<@title; $i++)
    {
        $worksheet->write(0,$i,$title[$i]);
    }
    for (my $i=0;$i<@stock;$i++)
    {
        for(my $j=0; $j<=45; $j++)
        {
            $worksheet->write($i+1,$j,$stock[$i][$j]);
        }
    }
    return @stock;
}
#print $title;
#print $stock;
sub history {
    my ($id,$workbook)=@_;
    
    my $doc = get 'http://www.google.com/finance/getprices?q='.$id.'&x=TPE&i=86400&p=5Y&f=d,c,h,l,o,v';
    if (!$doc) {return;}
    
    my $worksheet = $workbook->add_worksheet("歷史股價"); #新建一個Sheet
    
    my @title=("日期","收盤價","最高","最低","開盤","成交量","漲跌","漲跌比率","DI","EMA12","EMA26","DIF","MACD","OSC","5MA","10MA","20MA","60MA","100MA","200MA","","RSV","日K","日D","日K-日D","日KDA","週RSV","週K","週D","週K-週D","週KDA","RSI6","RSI12","RSI24","RSI50","RSI100","5日漲跌","10日漲跌","20日漲跌","60日漲跌","100日漲跌","200日漲跌","WKS20");
    my @line = split('\n', $doc);
    my @stock;
    #my $row =1;
    my $timestamp;
    my $countstamp=0;
    for (my $i =0; $i<@title; $i++)
    {
        $worksheet->write(0,$i,$title[$i]);
    }
    for (my $i =7; $i<@line;$i++)
    {
        my $r = $i-7;
        if(1)
        {
            #print "$line[$i]\n";
            my @temp=split(',', $line[$i]);
            foreach my $j (0 .. @temp-1)
            {
                
                if($j==0)#timestamp;
                {
                    if ($temp[$j] =~ /a(\d+)/ )
                    {
                        $temp[$j]= $1;
                        $timestamp = $1;
                        $countstamp=0;
                    }
                    else
                    {
                        $countstamp++;
                        $temp[$j]= $timestamp + 86400 * $temp[$j];
                    }
                    #$worksheet->write($row,$j,scalar localtime $temp[$j]);
                }
                $stock[$r][$j]=$temp[$j];
                
                
            }
            
            #@stock 0 日期 1 收盤 2 最高 3 最低 4 開盤 5 成交量 6 漲跌 7漲跌比率
            #       8 每日需求指數(Demand Index，DI)，DI = (最高價 + 最低價 + 2 × 收盤價) ÷ 4。
            #       9 EMA12 10 EMA26 11 DIV 12 MACD 13 OSC
            #       14 5MA 15 10MA 16 20MA 17 60MA 18 100MA 19 200 MA
            #       21 RSV 22 K 23 D
            #       31 5日漲跌 32 10日漲跌 33 20日漲跌 34 60日漲跌 35 100日漲跌 36 200日漲跌
            #       100 歷史股價和(用於計算平均)
            
            #漲跌
            if($i!=7)
            {
                $stock[$r][6]=$stock[$r][1]-$stock[$r-1][1];
            }
            else
            {
                $stock[$r][6]=0;
            }
            
            $stock[$r][7]=$stock[$r][6]/$stock[$r][1]*100;
            
            
            #歷史股價和
            if($i!=7)
            {
                $stock[$i-7][100]=$stock[$i-8][100]+$stock[$i-7][1];
            }
            else
            {
                $stock[$i-7][100]=$stock[$i-7][1];
            }
            
            #Demand Index
            if ($stock[$i-7][2]==0&&$stock[$i-7][3]==0)
            {
                $stock[$i-7][8]=$stock[$i-7][1];
            }
            else
            {
                $stock[$i-7][8] = $stock[$i-7][2]*0.25+$stock[$i-7][3]*0.25+$stock[$i-7][1]*0.5;
            }
            #歷史Demand Index和 8 歷史DI和 9 EMA12 10 EMA26 11 DIV 12 MACD 13 OSC
            if($i!=7)
            {
                #$stock[$i-7][8]=$stock[$i-8][8]+$stock[$i-7][7];
                $stock[$i-7][9]=$stock[$i-8][9]*(11/13)+$stock[$i-7][8]*(2/13);
                $stock[$i-7][10]=$stock[$i-8][10]*(25/27)+$stock[$i-7][8]*(2/27);
                $stock[$i-7][11]=$stock[$i-7][9]-$stock[$i-7][10];
                $stock[$i-7][12]=$stock[$i-8][12]*0.8+$stock[$i-7][11]*0.2;
                $stock[$i-7][13]=$stock[$i-7][11]-$stock[$i-7][12];
            }
            else
            {
                $stock[$i-7][10]=$stock[$i-7][9]=$stock[$i-7][8];
                $stock[$i-7][13]=$stock[$i-7][12]=$stock[$i-7][11]=0;
            }
            
            
            #       14 5MA 15 10MA 16 20MA 17 60MA 18 100MA 19 200 MA
            if(1)
            {
                $stock[$i-7][14]= $i-7 >= 5 ? ($stock[$i-7][100]-$stock[$i-7-5][100])/5:$stock[$i-7][100]/($i-6);
                $stock[$i-7][15]= $i-7 >= 10 ? ($stock[$i-7][100]-$stock[$i-7-10][100])/10:$stock[$i-7][100]/($i-6);
                $stock[$i-7][16]= $i-7 >= 20 ? ($stock[$i-7][100]-$stock[$i-7-20][100])/20:$stock[$i-7][100]/($i-6);
                $stock[$i-7][17]= $i-7 >= 60 ? ($stock[$i-7][100]-$stock[$i-7-60][100])/60:$stock[$i-7][100]/($i-6);
                $stock[$i-7][18]= $i-7 >= 100 ? ($stock[$i-7][100]-$stock[$i-7-100][100])/100:$stock[$i-7][100]/($i-6);
                $stock[$i-7][19]= $i-7 >= 200 ? ($stock[$i-7][100]-$stock[$i-7-200][100])/200:$stock[$i-7][100]/($i-6);
            }
            
            #21 日RSV 22 日K 23 日D 24 日K-日D
            my $min = $stock[$r][3];
            my $max = $stock[$r][2];
            for (my $j= $r-1; $j>$r-9 && $j >= 0; $j--)
            {
                $min = $min < $stock[$j][3] ? $min : $stock[$j][3];
                $max = $max > $stock[$j][2] ? $max : $stock[$j][2];
            }
            $stock[$r][21]= ($max-$min)?100*($stock[$r][1]-$min)/($max-$min):50;
            $stock[$r][22]= $r?(1/3)*$stock[$r][21]+(2/3)*$stock[$r-1][22]:50;
            $stock[$r][23]= $r?(1/3)*$stock[$r][22]+(2/3)*$stock[$r-1][23]:50;
            $stock[$r][24]= $stock[$r][22]-$stock[$r][23];
            $stock[$r][25]= $r?$stock[$r][24]-$stock[$r-1][24]:0;
            
            #26 週RSV 27 週K 28 週D 29 週K-週D
            $min = $stock[$r][3];
            $max = $stock[$r][2];
            for (my $j= $r-1; $j>$r-45 && $j >= 0; $j--)
            {
                $min = $min < $stock[$j][3] ? $min : $stock[$j][3];
                $max = $max > $stock[$j][2] ? $max : $stock[$j][2];
            }
            $stock[$r][26]= ($max-$min)?100*($stock[$r][1]-$min)/($max-$min):50;
            $stock[$r][27]= $r?(1/3)*$stock[$r][26]+(2/3)*$stock[$r-1][27]:50;
            $stock[$r][28]= $r?(1/3)*$stock[$r][27]+(2/3)*$stock[$r-1][28]:50;
            $stock[$r][29]=$stock[$r][27]-$stock[$r][28];
            $stock[$r][30]= $r?$stock[$r][29]-$stock[$r-1][29]:0;
            
            
            #31 6RSI 32 12RSI 32 24RSI
            my $rsi=30;
            my @rsi_d= (6,12,24,50,100);
            for(my $j = 0; $j<@rsi_d ; $j++)
            {
                if($r>=$rsi_d[$j])
                {
                    my $up=0;
                    my $dn=0;
                    for(my $k =$r;$k>$r-$rsi_d[$j];$k--)
                    {
                        if($stock[$k][6]>0){$up+=$stock[$k][6]};
                        if($stock[$k][6]<0){$dn-=$stock[$k][6]};
                    }
                    $stock[$r][$rsi+1+$j]=($dn+$up)? 100 * $up/($dn+$up):50;
                }
                else
                {$stock[$r][$rsi+1+$j]="N/A";}
            }
            
        }
    }
    
    for (my $r=0;$r<@stock;$r++)
    {
        #31 5日漲跌 32 10日漲跌 33 20日漲跌 34 60日漲跌 35 100日漲跌 36 200日漲跌
        my $fu=35;
        $stock[$r][$fu+1]= $r+5  <@stock ?$stock[$r+5][1]/$stock[$r][1]-1: "N/A";
        $stock[$r][$fu+2]= $r+10 <@stock ?$stock[$r+10][1]/$stock[$r][1]-1: "N/A";
        $stock[$r][$fu+3]= $r+20 <@stock ?$stock[$r+20][1]/$stock[$r][1]-1: "N/A";
        $stock[$r][$fu+4]= $r+60 <@stock ?$stock[$r+60][1]/$stock[$r][1]-1: "N/A";
        $stock[$r][$fu+5]= $r+100<@stock ?$stock[$r+100][1]/$stock[$r][1]-1: "N/A";
        $stock[$r][$fu+6]= $r+200<@stock ?$stock[$r+200][1]/$stock[$r][1]-1: "N/A";
        
        my $sc=42;my $score=0;
        #MACD
        if($r&&$stock[$r][13]>0&&$stock[$r-1][13]<0){$score+=2.5};#OSC
        if(($r-1)&&(2*$stock[$r-1][13]-$stock[$r-2][13])>0){$score+=2.5};
        if($r-5){$score+=($stock[$r][12]-$stock[$r-5][12])};#MACD差
        #MA
        if($r&&$stock[$r][1]>$stock[$r][18]&&$stock[$r-1][1]<$stock[$r-1][18]){$score+=2};
        if($r&&$stock[$r][1]>(2*$stock[$r][18]-$stock[$r-1][18])&&$stock[$r-1][1]<(2*$stock[$r][18]-$stock[$r-1][18])){$score+=1};
        #if($r&&$stock[$r][1]>$stock[$r][16]&&$stock[$r-1][1]<$stock[$r-1][16]){$score+=1};
        #if($r&&$stock[$r][1]>$stock[$r][15]&&$stock[$r-1][1]<$stock[$r-1][15]){$score+=0.5};
        #if($r&&$stock[$r][1]>$stock[$r][14]&&$stock[$r-1][1]<$stock[$r-1][14]){$score+=0.5};
        
        #KD
        if($r&&$stock[$r][24]>0&&$stock[$r-1][24]<0){$score+=2.5};
        if(($r-1)&&(2*$stock[$r-1][24]-$stock[$r-2][24])>0){$score+=2.5};
        if($r&&$stock[$r][25]>0&&$stock[$r-1][25]<0){$score+=1};
        #if($r&&$stock[$r][29]>0&&$stock[$r-1][29]<0){$score+=1.5};
        #if($r&&$stock[$r][30]>0&&$stock[$r-1][30]<0){$score+=0.5};
        #RSI
        if($stock[$r][31] =~ qr/^-?\d+(\.\d+)?$/){$score+=(50-$stock[$r][31])/50;}
        #if($stock[$r][31] ne "N/A"){$score+=(100-$stock[$r][32])/50;}
        if(($r-1)&& $stock[$r-1][31]=~ qr/^-?\d+(\.\d+)?$/&&$stock[$r-1][32]=~ qr/^-?\d+(\.\d+)?$/&& $stock[$r][31]=~ qr/^-?\d+(\.\d+)?$/&&$stock[$r][32]=~ qr/^-?\d+(\.\d+)?$/&&$stock[$r][31]>$stock[$r][32]&&$stock[$r-1][31]<$stock[$r-1][32]){$score+=1};
        
        $stock[$r][$sc]=$score;
    }
    
    
    @stock= reverse @stock;
    for (my $i=0;$i<@stock;$i++)
    {
        $worksheet->write($i+1,0,scalar localtime $stock[$i][0]);
        for(my $j=1; $j<=45; $j++)
        {
            $worksheet->write($i+1,$j,$stock[$i][$j]);
        }
    }
    
    
    return @stock;
    
    #    @stock_c = reverse @stock_c;
    #print @stock_c;
    #print $id.": ".average(100,@stock_c)."\n";#20MA
    #$worksheet->write(++$row,$1,average(100,@stock_c));
    #print $stock[0][1];
    
    
}



#function to return the average number of the array ;

sub average {
    #    my $worksheet = $workbook->add_worksheet("技術分析"); #新建一個Sheet
    
    my ($day,@num) = @_;#往前回溯多少天，陣列
    #print $day;
    my $num = @num > $day? $day: @num;
    my $total;
    for my $i(0..$num) {
        
        $total += $num[$i];
    }
    
    #    $worksheet->write($1,$1,$total/$num);
    return ($total/$num);
}

sub nlog{
    
    my ($id,$workbook)=@_;
    my $web = get 'http://stock.nlog.cc/e/'.$id;
    
    if (!$web) {return;}
    
    my $worksheet = $workbook->add_worksheet("營收"); #新建一個Sheet
    my @title=("月份","營收","累積營收","資本額倍數","累積淨利","累積淨利(稅)","月EPS","累積EPS","累積EPS(稅)","EPS(母)","年EPS","年EPS(母)","P/E","P/E(母)","年增率％");
    for (my $i =0; $i<@title; $i++)
    {
        $worksheet->write(0,$i,$title[$i]);
    }
    my $str = substr ($web,index($web,"stad('p1',true)"),index($web,"本盈餘")-index($web,"stad('p1',true)"));
    my $this_year = substr ($str,index($str,"class=lo><td>")+13,index($str,"</td></tr><tr><td colspan=15 class=o>財報")-index($str,"class=lo><td>")-13);
    $this_year =~ s/<\/td><\/tr><tr class=le><td>|<\/td><\/tr><tr class=lo><td>/<\/td><td>/;
    my @line = split('</td><td>', $this_year);
    my $row=0;
    my @eps;
    foreach my $i(0 .. @line-1){
        $eps[$row][$i%15]=$line[$i];
        if($i%15==14)
        {
            $row++;
        }
    }
    
    @eps= reverse @eps;
    $row+=11;
    $str = substr ($str,index($str,"去年"));
    my $last_year = substr ($str,index($str,"class=lo><td>")+13,index($str,"</td></tr><tr><td colspan=15 class")-index($str,"class=lo><td>")-13);
    $last_year =~ s/<\/td><\/tr><tr class=le><td>|<\/td><\/tr><tr class=lo><td>/<\/td><td>/g;
    @line = split('</td><td>', $last_year);
    
    foreach my $i(0 .. @line-1){
        $eps [$row][$i%15]=$line[$i];
        if($i%15==14)
        {
            $row--;
        }
    }
    for (my $i=0;$i<@eps;$i++)
    {
        for(my $j=0; $j<15;$j++)
        {
            $worksheet->write($i+1,$j,$eps[$i][$j]);
        }
    }
    return @eps;
}

