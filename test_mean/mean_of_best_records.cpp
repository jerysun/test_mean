#include "stdafx.h"

/*
 * filename: mean_of_best_records.cpp
 * describe: Arithmetic mean problem of the Top N records in Database SQL.
 *           There are a list of athletes' records consisting of (id, ath_id,
 *           name, record, r_date), a single athlete may have several
 *           different records. Please find a solution to list the
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

#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <string.h>
#include "mean_of_best_records.h"

athlete::athlete(std::string name, unsigned record, std::string r_date) {
  name = trim(name);
  size_t maxlen = sizeof(this->name);
  size_t arglen = strlen(name.c_str());
  size_t len = arglen < maxlen ? arglen : maxlen - 1;
  strncpy(this->name, name.c_str(), len);
  this->name[len] = '\0';

  this->record = record;

  r_date = trim(r_date);
  maxlen = sizeof(this->r_date);
  arglen = strlen(r_date.c_str());
  len = arglen < maxlen ? arglen : maxlen - 1;
  strncpy(this->r_date, r_date.c_str(), len);
  this->r_date[len] = '\0';
}

void mean_of_best_records::input_athletes (
    std::unique_ptr<std::unordered_multimap<unsigned int, athlete>> as) {
  *map_athletes = std::move(*as);
}

void mean_of_best_records::output_athletes() {
  if (!map_athletes->empty()) {
    for (const auto &a : *map_athletes) {
      std::cout << "ath_id: " << a.first << ", record: " << a.second.record << std::endl;
    }
  }
}

void mean_of_best_records::output_athletes_ordered() {
  if (!map_athletes->empty()) {
    getMapKeys(map_athletes, m_keys);

    for (const auto &k : *m_keys) {
      auto ret = map_athletes->equal_range(k);

      for (std::unordered_multimap<unsigned int, athlete>::const_iterator it = ret.first;
           it != ret.second; ++it) {
        std::cout << "ath_id: " << it->first << ", record: " 
                  << it->second.record << std::endl;
      }
    }
  }
}

void mean_of_best_records::output_mean_of_best_records() {
  if (!map_athletes->empty()) {
    unsigned int cnt = 0;
    unsigned int sum = 0;
    unsigned int i = 0;

    //The athlete might achieve the same best record several times
    std::unique_ptr<std::multiset<unsigned int>> records;
    getMapKeys(map_athletes, m_keys);

    for (const auto &k : *m_keys) {
      cnt = map_athletes->count(k);
      if (cnt == 0) continue; //can not be diveded by 0, just in case
      if (cnt > m_topn) cnt = m_topn;

      auto ret = map_athletes->equal_range(k);

      records.reset(new std::multiset<unsigned int>());
      for (std::unordered_multimap<unsigned int, athlete>::const_iterator it = ret.first;
           it != ret.second; ++it) {
        records->insert(it->second.record);
      }
      
      auto it = records->crbegin();
      for (i = 0, sum = 0; i < cnt && it != records->crend(); ++it, ++i) {
        sum += *it;
      }
      
      std::cout << "ath_id: " << k << ", the arithmetic mean of his/her top " 
                << cnt << " records: " << sum / cnt << std::endl;
    }
  }
}

//serialization
bool mean_of_best_records::serialize_table(const char* filepath) {
  std::ofstream ofs;

  if (!map_athletes->empty()) {
    try {
      ofs.open(filepath, std::ios::out | std::ios::binary);

      for (const auto &a : *map_athletes) {
        ofs.write((char*)&a.first, sizeof(a.first));
        ofs.write(a.second.name, sizeof(a.second.name));
        ofs.write((char*)&a.second.record, sizeof(a.second.record));
        ofs.write(a.second.r_date, sizeof(a.second.r_date));
      }

      ofs.close();
      return true;
    } catch (std::ofstream::failure& e) {
      std::cerr << "\nIO Exception during the writing of file:\n"
                << e.what() << std::endl;

      if (ofs.is_open()) ofs.close();
      return false;
    }
  } else {
    return false;
  }
}

std::ifstream& mean_of_best_records::read_athlete_bin (
    std::ifstream &ifs, athlete &ath, unsigned int &ath_id) {
  ifs.read((char*)&ath_id, sizeof(ath_id));
  ifs.read(ath.name, sizeof(ath.name));
  ifs.read((char*)&ath.record, sizeof(ath.record));
  ifs.read(ath.r_date, sizeof(ath.r_date));
/*
  std::cout.width(9);  std::cout << std::left  << ath.ath_id;
  std::cout.width(23); std::cout << std::left  << ath.name;
  std::cout.width(7);  std::cout << std::left  << ath.record;
  std::cout << std::left  << ath.r_date << std::endl;
 */
  return ifs;
}

// deserialization
bool mean_of_best_records::deserialize_table(const char* filepath) {
  std::ifstream ifs;
  athlete ath;
  unsigned int ath_id;
  std::unique_ptr<std::unordered_multimap<unsigned int, athlete>> 
    tbl_athlete{std::make_unique<std::unordered_multimap<unsigned int, athlete>>()};

  /*
  std::cout << "Deserializing table:\n\n";
  std::cout << std::endl
            << "ath_id   name                   record r_date         " << std::endl
            << "------------------------------------------------------" << std::endl;
   */
  try {
    ifs.open(filepath, std::ios::in | std::ios::binary);

    while (read_athlete_bin(ifs, ath, ath_id)) {
      tbl_athlete->insert(std::pair<unsigned int, athlete>(ath_id, ath));
    }

    ifs.close();
     
  } catch (std::ifstream::failure& e) {
    std::cerr << "\nIO Exception during the reading of file:\n"
              << e.what() << std::endl;

    if (ifs.is_open()) ifs.close();
    return false;
  }

  std::cout << "\nDeserializing table:\n\n";
  std::cout << "ath_id   name                   record r_date         " << std::endl
            << "------------------------------------------------------" << std::endl;
  if (!tbl_athlete->empty()) {
    for (const auto &a : *tbl_athlete) {
      std::cout.width(9);  std::cout << std::left  << a.first;
      std::cout.width(23); std::cout << std::left  << a.second.name;
      std::cout.width(7);  std::cout << std::left  << a.second.record;
      std::cout << std::left  << a.second.r_date << std::endl;
    }
  }

  return true;
}

unsigned int mean_of_best_records::getTopn() {
  return m_topn;
}

void mean_of_best_records::setTopn(unsigned int tn) {
  m_topn = tn;
}
