# Authors:

- Devashi Tandon
- Pratyush Parimal
- Lawrence Candes

# To be updated....

# Sample Test Runs:

root@flo:/data/misc # ./file_loc hmwk6                                         
reading location info for: hmwk6
Age: 0
Lat: 40.799449
Long: -73.962456
Accuracy: 61.223000
http://maps.googleapis.com/maps/api/staticmap?center=40.799449,-73.962456&zoom=14&size=600x600&markers=color:blue%7Clabel:S%7C40.799449,-73.962456
root@flo:/data/misc # ./file_loc hmwk6/dir1/                                   
reading location info for: hmwk6/dir1/
Age: 1
Lat: 40.799449
Long: -73.962456
Accuracy: 61.223000
http://maps.googleapis.com/maps/api/staticmap?center=40.799449,-73.962456&zoom=14&size=600x600&markers=color:blue%7Clabel:S%7C40.799449,-73.962456
root@flo:/data/misc # ./file_loc hmwk6/dir1/fi                                 
file1 file2 
root@flo:/data/misc # ./file_loc hmwk6/dir1/file1                              
reading location info for: hmwk6/dir1/file1
Age: 1
Lat: 40.885951
Long: -73.912265
Accuracy: 62.223000
http://maps.googleapis.com/maps/api/staticmap?center=40.885951,-73.912265&zoom=14&size=600x600&markers=color:blue%7Clabel:S%7C40.885951,-73.912265
root@flo:/data/misc # ./file_loc hmwk6/dir1/file2                              
reading location info for: hmwk6/dir1/file2
Age: 1
Lat: 40.885717
Long: -73.912505
Accuracy: 59.112000
http://maps.googleapis.com/maps/api/staticmap?center=40.885717,-73.912505&zoom=14&size=600x600&markers=color:blue%7Clabel:S%7C40.885717,-73.912505
root@flo:/data/misc #
