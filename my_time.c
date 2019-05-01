 #include "my_time.h"

 int timeToSec( Time inTime )
 {
     int sec;
     sec = ( inTime.hour ) * 3600 + ( inTime.minute ) * 60 + inTime.second;
     return sec;
 }
