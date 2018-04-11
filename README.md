# Arithmetic mean problem of the Top N records in Database SQL

There are a list of athletes' records consisting of (id, ath_id, name,
record, r_date), a single athlete may have several different records.

Please find a solution to list the arithmetic means of the best
three records(so-called top n, here n is 3) of EACH athlete.

It's hard to write an exact sql query; A stored procedure is possible but
still not easy; A C++ program, wow, that's REALLY a CHALLENGE!

Let's get it done using the modern C++, C++14 precisely. The solution shows
you the charming & power of "move semantics", "smart pointers", "hash table", 
"template programming", database persistence (binary serialization and 
deserialization), "lambda expression", ... Of course, RAW pointers to the 
memory buffer are inevitable since you're using C++ instead of Java ^_^

![alt a screenshot of the output result]
(https://github.com/jerysun/test_mean/blob/master/mobr.jpg?raw=true)

Well, I know you're curious about what the MySQL sql query statement in this
case looks exactly like. The answer is:

```bash

SELECT ath_id, avg(record)

FROM 

(SELECT ath_id, record

 FROM athletes as t1

 WHERE 

 (SELECT COUNT(*) FROM athletes

  WHERE t1.ath_id=ath_id and record > t1.record) < 3) as d

GROUP BY ath_id;

```


Don't panic, dude :-)

Have fun!

Jerry Sun

https://sites.google.com/site/geekssmallworld/
