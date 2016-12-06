#!/usr/bin/awk -f
# AWK script to filter out the superset functions as well as differences 
# between DELPHI library and C implementation

BEGIN	{
	  while (getline < fsuperset) {
	    if ((NF==0) || ($1=="#")) continue; # skip this record
	    superset[$1]=0;
	    }
	  while (getline < libdef)
            if ($1=="EXPORTS") break;
	  while (getline < libdef) {
	    if ((NF==0) || ($1=="#")) continue; # skip this record
	    def[$1]=0;
	    }
	}

# Filter out only exports
$2=="T"	{ 
	   if ($3 in def) def[$3]++;
	   else 
             if ($3 in superset) superset[$3]++;
             else
	       print $3;
	}


END {
	  # Find differences
	  total=0;
	  for (i in def)
	    if (def[i]==0) {
              print i > diffs;
	      total++;
	      }
	  if (total!=0)
	    print "Total Diffs: ", total > diffs;
	  total=0;
	  for (i in superset)
	    if (superset[i]==0)
	      if ( (!(i in def)) || (def[i]==0) ) {
	        print i > diffs;
	        total++;
		}
	  if (total!=0)
	    print "SuperDiffs: ", total > diffs;
	  close(diffs);
	}
