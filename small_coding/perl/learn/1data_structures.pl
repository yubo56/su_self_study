use feature ':5.10';
use strict;
use warnings;

my $len = 10; # scalar
my @arr = []; # array
my %hash = ();# hash

my $nested_refs = [ # reference
    [1],
    [1, 2],
    [1, 2, 3],
    [1, 2, 3, 4]
];
my %nested_hash = (
    "Holiday Name" => "Bisu Day",
    "Date" => { # reference
        "Day" => 3,
        "Month" => 3,
        "Year" => 2007
    },
    "Source" => "google.com"
);

my @sorted;

for (my $i = 0; $i < $len; $i++) {
    $arr[$i] = $i;
    $hash{$i} = $len - $i;
}

print "arr coercion: " . @arr . "\n";
print @arr, "\n\n";

print "hash coercion: " . %hash . "\n";
print %hash, "\n";
print "hash length: ", scalar keys %hash, "\n";
for (my $i = 0; $i < keys %hash; $i++) {
    print $i, ", ", $hash{$i}, "\n";
}
print "\n";

@sorted = sort %hash;
print "sorted coercion: " . @sorted . "\n";
print @sorted, "\n\n";

print $nested_refs, "\n"; # prints pointer
for (my $i = 0; $i < scalar @{ $nested_refs }; $i++) {
    my $subarr = @{ $nested_refs }[$i];
    for (my $j = 0; $j < scalar @{ $subarr }; $j++) {
        print @{ $subarr }[$j];
    }
    print "\n";
}
print "foreach mode\n";
foreach my $subarr ( @{ $nested_refs } ) {
    foreach my $el ( @{ $subarr } ) {
        print $el;
    }
    print "\n";
}
print "\n";

print "Holiday Name: ",  $nested_hash{"Holiday Name"}, "\n";
print "Date: ",
    ${ $nested_hash{"Date"} }{"Month"}, "/",
    ${ $nested_hash{"Date"} }{"Day"}, "/",
    ${ $nested_hash{"Date"} }{"Year"},
    "\n";
print "URL: ", $nested_hash{"Source"}, "\n\n";

say reverse "hello world";
say reverse scalar "hello world"; # scalar changes the context of its right
    # hello world
say scalar reverse "hello world";
    # dlrow olleh, reverse operates in scalar context
