// test_mean.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

/*
* filename: test_mean.cpp
*/

#include <iostream>
#include "mean_of_best_records.h"

int main() {
	std::unique_ptr<mean_of_best_records> mobr = std::make_unique<mean_of_best_records>(3);

	/*
	* Simulate DB (e.g., MySQL) operations:
	* INSERT INTO (ath_id, athletes(record, name, r_date))
	* VALUES(2, 79, "John Wayne", "2010-07-08");
	*/
	std::unique_ptr<std::unordered_multimap<unsigned int, athlete>>
		athletes{ std::make_unique<std::unordered_multimap<unsigned int, athlete>>() };

	athletes->insert(std::make_pair<unsigned int, athlete>(2, { "John Wayne", 79, "2010-07-08" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(7, { "Ronald Regan", 51, "2000-03-22" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(1, { "Ford Harrison", 85, "2009-11-13" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(2, { "John Wayne", 69, "2017-01-01" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(7, { "Ronald Regan", 97, "2005-05-20" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(3, { "Demi Lovato", 33, "2002-04-28" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(1, { "Ford Harrison", 89, "2001-09-03" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(5, { "Anna Paquin", 79, "2010-07-07" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(7, { "Ronald Regan", 39, "2010-07-08" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(4, { "Kate Winslet", 97, "2010-07-08" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(3, { "Demi Lovato", 49, "2010-07-08" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(6, { "Monica Bellucci", 59, "2010-07-08" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(5, { "Anna Paquin", 95, "2010-07-08" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(5, { "Anna Paquin", 37, "2010-07-08" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(5, { "Anna Paquin", 89, "2010-05-11" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(1, { "Ford Harrison", 59, "2010-07-08" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(4, { "Kate Winslet", 56, "2010-07-08" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(5, { "Anna Paquin", 89, "2010-04-17" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(7, { "Ronald Regan", 74, "2010-07-08" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(4, { "Kate Winslet", 51, "2010-07-08" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(7, { "Ronald Regan", 91, "2010-07-08" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(6, { "Monica Bellucci", 92, "2006-02-08" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(2, { "John Wayne", 73, "2003-05-03" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(3, { "Demi Lovato", 59, "2008-08-18" }));
	athletes->insert(std::make_pair<unsigned int, athlete>(6, { "Monica Bellucci", 84, "2001-03-12" }));

	mobr->input_athletes(std::move(athletes));
	mobr->output_athletes();
	std::cout << "\nOutput athletes ordered by ID:\n\n";
	mobr->output_athletes_ordered();
	std::cout << "\nOutput the arithmetic mean of the top " << mobr->getTopn()
		<< " records of each athlete:\n\n";
	mobr->output_mean_of_best_records();
	mobr->serialize_table("arithmetic_mean_of_top3.dst");
	mobr->deserialize_table("arithmetic_mean_of_top3.dst");

	std::cout << "\ntime_t of '2010-07-08': " << string_to_time_t("2010-07-08") << std::endl;

	std::time_t tmt = 1278540000;
	std::cout << "time_t = 1278540000, date = " << time_t_to_string(tmt) << std::endl;

	std::cout << "sizeof std::time_t = " << sizeof(std::time_t) << std::endl;
}
