#ifndef SET_MAP_ROUTINES_H
#define SET_MAP_ROUTINES_H

#include <set>
#include <memory>

//They supports all types of map
template<class map_t>
void getMapKeys(const map_t& m, std::set<typename map_t::key_type>& k) {
  typename std::set<typename map_t::key_type>::iterator endkey = k.end();

  for (typename map_t::const_iterator b(m.begin()), e(m.end()); b != e; ++b) {
    k.insert(endkey, b->first);
  }
}

template<class map_t>
void getMapKeys(const map_t& m, std::shared_ptr<std::set<typename map_t::key_type>> k) {
  typename std::set<typename map_t::key_type>::iterator endkey = k->end();

  for (typename map_t::const_iterator b(m.begin()), e(m.end()); b != e; ++b) {
    k->insert(endkey, b->first);
  }
}

template<class map_t>
void getMapKeys(const std::shared_ptr<map_t> m, std::set<typename map_t::key_type>& k) {
  typename std::set<typename map_t::key_type>::iterator endkey = k.end();

  for (typename map_t::const_iterator b(m->begin()), e(m->end()); b != e; ++b) {
    k.insert(endkey, b->first);
  }
}

template<class map_t>
void getMapKeys(const std::shared_ptr<map_t> m, std::shared_ptr<std::set<typename map_t::key_type>> k) {
  typename std::set<typename map_t::key_type>::iterator endkey = k->end();

  for (typename map_t::const_iterator b(m->begin()), e(m->end()); b != e; ++b) {
    k->insert(endkey, b->first);
  }
}

#endif //SET_MAP_ROUTINES_H
