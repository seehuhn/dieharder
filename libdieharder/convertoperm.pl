#!/usr/bin/perl
#

 use FileHandle;
 my $i=1;
 my $j=1;

 $r[$i][$j] = 1;
 

 $firstcall = 1;

 # Open op5dat.for
 open(IN,"op5dat.for") || die "Sorry, I couldn't open op5dat.for\n";
 open(OUT,">op5dat") || die "Sorry, I couldn't open op5dat.\n";
 autoflush OUT 1;
 # Write to the file...
 printf OUT "#========================================================================\n";
 printf OUT "# R and S data ported from diehard and put into C-style matrix form\n";
 printf OUT "#========================================================================\n";
 # ...and also output to console, just for fun.
 printf "#========================================================================\n";
 printf "# R and S data ported from diehard and put into C-style matrix form\n";
 printf "#========================================================================\n";

 # Initialize the table (read in the whole thing in order)
 nextrs();
 for($i=0;$i<60;$i++) {
   for($j=$i;$j<60;$j++){
     $r[$i][$j] = nextrs();
     $r[$j][$i] = $r[$i][$j];
   }
 }

 print OUT "static int r[60][60] = {\n";
 for($i=0;$i<60;$i++) {
   print OUT "  {";
   for($j=0;$j<59;$j++){
     printf OUT (" %10d,",$r[$i][$j]);
   }
   if($i<59){
     printf OUT (" %10d},\n",$r[$i][59]);
   } else {
     printf OUT (" %10d}\n",$r[$i][59]);
   }
 }
 print OUT "};\n";

 for($i=0;$i<60;$i++) {
   for($j=$i;$j<60;$j++){
     $s[$i][$j] = nextrs();
     $s[$j][$i] = $s[$i][$j];
   }
 }

 print OUT "static int s[60][60] = {\n";
 for($i=0;$i<60;$i++) {
   print OUT "  {";
   for($j=0;$j<59;$j++){
     printf OUT (" %10d,",$s[$i][$j]);
   }
   if($i<59){
     printf OUT (" %10d},\n",$r[$i][59]);
   } else {
     printf OUT (" %10d}\n",$r[$i][59]);
   }
 }
 print OUT "};\n";

 exit;


sub nextrs {
 if($firstcall == 1){
   my $irs = 0;
   my $jrs;
   while(<IN>){
     chop;
     @field = split(/\s+/,$_);
     $fields = @field;
     for($jrs=1;$jrs<$fields;$jrs++){
       $rs[$irs] = $field[$jrs];
       $irs++;
     }
   }
   # print "# Read in $irs values\n";
   $irs = 0;
   $firstcall = 0;
   return;
 }

 return $rs[$irs++];

}

