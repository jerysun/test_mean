/*
 * filename: mean_of_best_records.h
 * describe: Arithmetic mean problem of the Top N records in Database SQL.
 *           There are a list of athletes' records consisting of ath_id,
 *           name, record, r_date. Please find a solution to list the
 *           arithmetic means of the best three records(so-called top n,
 *           here n is 3) of EACH athlete.
 * author:   Jerry Sun <jerysun0818@gmail.com>
 * date:     Octobler 1, 2017
 * remark:   It's hard to write an exact sql query; A stored procedure is
 *           possible but still not easy; A C++ program, wow, that's REALLY
 *           a CHALLENGE!
 *           
 *           Let's get it done using the modern C++, C++14 precisely. The 
 *           shows you the charming & power of "move semantics", "smart
 *           pointers", "hash table", "template programming", database
 *           persistence (binary serialization and deserialization),
 *           "lambda expression", ... Of course, RAW pointers to the memory
 *           buffer are inevitable since you're using C++ instead of Java ^_^
 *
 *           The complete project that can be built in my github:
 *           https://github.com/jerysun
 *
 * web:      http://nl.linkedin.com/in/jerysun
 * blog:     https://sites.google.com/site/geekssmallworld
 */

#ifndef MEAN_OF_BEST_RECORDS_H
#define MEAN_OF_BEST_RECORDS_H

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

#include "set_map_routines.h"
#include "dst_util.h"

struct athlete {
  athlete() {}
  athlete(std::string name, unsigned record, std::string r_date);
  
  char name[24] = {0};
  unsigned int record; // 0 ~ 100 
  char r_date[16] {0}; // year-months-date: 2000-02-30
};
  

class mean_of_best_records {
  public:
    mean_of_best_records(unsigned int topn = 2) : m_topn(topn) {}
  
    void input_athletes(std::unique_ptr<std::unordered_multimap<unsigned int, athlete>> as);
    void output_athletes();
    void output_athletes_ordered();
    void output_mean_of_best_records();

    //serialization and deserialization
    //ath_id: not the sequential unique id automatically generated for a table
    std::ifstream& read_athlete_bin(std::ifstream &ifs, athlete &ath, unsigned int &ath_id);
    bool serialize_table(const char* filepath);
    bool deserialize_table(const char* filepath);

    unsigned int getTopn();
    void setTopn(unsigned int tn);

  private:
    unsigned int m_topn;

    //unsigned int is for ath_id: not the sequential unique id automatically generated for a table
    std::shared_ptr<std::unordered_multimap<unsigned int, athlete>> 
      map_athletes{std::make_shared<std::unordered_multimap<unsigned int, athlete>>()};

    std::shared_ptr<std::set<unsigned int>> m_keys{std::make_shared<std::set<unsigned int>>()};
};

#endif //MEAN_OF_BEST_RECORDS_H
