//odds_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/math/odds.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

int main(int argc, char* argv[])
{
	std::cout << yggr::math::odds_to_winning(0.0f) << std::endl;
	std::cout << yggr::math::odds_to_winning(1, 100) << std::endl;
	std::cout << yggr::math::odds_to_winning(1, 9) << std::endl;
	std::cout << yggr::math::odds_to_winning(-300) << std::endl;
	std::cout << yggr::math::odds_to_winning(-100) << std::endl;
	std::cout << yggr::math::odds_to_winning(100) << std::endl;
	std::cout << yggr::math::odds_to_winning(900) << std::endl;

	std::cout << yggr::math::odds_to_failure_rate(0.0f) << std::endl;
	std::cout << yggr::math::odds_to_failure_rate(1, 100) << std::endl;
	std::cout << yggr::math::odds_to_failure_rate(1, 9) << std::endl;

	std::cout << yggr::math::winning_to_floating_odds(yggr::math::odds_to_winning(0.0f)) << std::endl;
	std::cout << yggr::math::winning_to_floating_odds(yggr::math::odds_to_winning(1, 9)) << std::endl;

	float odds = 1.0f;

	std::cout << yggr::math::limit_continuous_lose_count(124, odds) << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
