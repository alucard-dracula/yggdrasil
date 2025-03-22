//moving_average_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <yggr/math/math.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test_sma(void)
{
	typedef 
		yggr::math::moving_avg::moving_avg_accumulator
		<
			yggr::math::moving_avg::make_accumlator_group
			<
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					yggr::math::moving_avg::moving_collector<3, yggr::s32>,
					yggr::math::moving_avg::identical_genner<yggr::s32>,
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::sma_accumulator
					<
						yggr::math::moving_avg::moving_collector<3, yggr::s32>
					>
				>::type,
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					// (3 - 1)  Three samples generate two absolute mean differences
					yggr::math::moving_avg::moving_collector<3 - 1, yggr::s32>, 
					yggr::math::moving_avg::abs_distance<yggr::s32>, // |xi - x_avg|
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::sma_accumulator
					<
						yggr::math::moving_avg::moving_collector<3 - 1, yggr::s32>
					>
				>::type
			>::type
		> acc_type;

	int samples[] = { 1, 10, 100, 1000, 10000, 5000, 500 };

	int mma_means[] = { 1, 6, 37, 370, 3700, 5333, 5167 };
	bool mma_saturated[] = { false, false, true, true, true, true, true };

	int smad_means[] = { 0, 9, 52, 529, 5297, 5465, 3067 };
	bool smad_saturated[] = { false, false, true, true, true, true, true };
	
	acc_type acc;

	for(int i = 0, isize = sizeof(samples) / sizeof(int); i != isize; ++i)
	{
		acc << samples[i];
		//std::cout  << "[ "
		//			<< acc.get_result<0>() << ", " << mma_means[i] << ", "
		//			<< acc.is_saturated<0>() << ", " << mma_saturated[i]
		//			<< " ]" << std::endl;

		//std::cout  << "[ "
		//			<< acc.get_result<1>() << ", " << smad_means[i] << ", "
		//			<< acc.is_saturated<1>() << ", " << smad_saturated[i]
		//			<< " ]"<< std::endl;

		yggr_test_assert(acc.get_result<0>() == mma_means[i]);
		yggr_test_assert(acc.is_saturated<0>() == mma_saturated[i]);
		yggr_test_assert(acc.get_result<1>() == smad_means[i]);
		yggr_test_assert(acc.is_saturated<1>() == smad_saturated[i]);
	}

	std::cout << "!!!test_sma success!!!" << std::endl;
}

void test_mma(void)
{
	typedef 
		yggr::math::moving_avg::moving_avg_accumulator
		<
			yggr::math::moving_avg::make_accumlator_group
			<
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					yggr::math::moving_avg::non_moving_collector<3, yggr::s32>,
					yggr::math::moving_avg::identical_genner<yggr::s32>,
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::mma_accumulator
					<
						yggr::math::moving_avg::non_moving_collector<3, yggr::s32>
					>
				>::type,
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					// (3 - 1)  Three samples generate two absolute mean differences
					yggr::math::moving_avg::non_moving_collector<3 - 1, yggr::s32>, 
					yggr::math::moving_avg::abs_distance<yggr::s32>, // |xi - x_avg|
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::mma_accumulator
					<
						yggr::math::moving_avg::non_moving_collector<3 - 1, yggr::s32>
					>
				>::type
			>::type
		> acc_type;

	int samples[] = { 1, 10, 100, 1000, 10000, 5000, 500 };

	int mma_means[] = { 1, 6, 37, 358, 3572, 4048, 2865 };
	bool mma_saturated[] = { false, false, true, true, true, true, true };

	int mmad_means[] = { 0, 9, 52, 507, 5075, 3251, 3400 };
	bool mmad_saturated[] = { false, false, true, true, true, true, true };
	
	acc_type acc;

	for(int i = 0, isize = sizeof(samples) / sizeof(int); i != isize; ++i)
	{
		acc << samples[i];
		//std::cout  << "[ "
		//			<< acc.get_result<0>() << ", " << mma_means[i] << ", "
		//			<< acc.is_saturated<0>() << ", " << mma_saturated[i]
		//			<< " ]" << std::endl;

		//std::cout  << "[ "
		//			<< acc.get_result<1>() << ", " << mmad_means[i] << ", "
		//			<< acc.is_saturated<1>() << ", " << mmad_saturated[i]
		//			<< " ]"<< std::endl;

		yggr_test_assert(acc.get_result<0>() == mma_means[i]);
		yggr_test_assert(acc.is_saturated<0>() == mma_saturated[i]);
		yggr_test_assert(acc.get_result<1>() == mmad_means[i]);
		yggr_test_assert(acc.is_saturated<1>() == mmad_saturated[i]);
	}

	std::cout << "!!!test_mma success!!!" << std::endl;
}

void test_ema_1_n(void)
{
	typedef 
		yggr::math::moving_avg::moving_avg_accumulator
		<
			yggr::math::moving_avg::make_accumlator_group
			<
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					yggr::math::moving_avg::non_moving_collector<3, yggr::s32>,
					yggr::math::moving_avg::identical_genner<yggr::s32>,
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::mma_accumulator
					<
						yggr::math::moving_avg::non_moving_collector<3, yggr::s32>
					>
				>::type,
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					// (3 - 1)  Three samples generate two absolute mean differences
					yggr::math::moving_avg::non_moving_collector<3 - 1, yggr::s32>, 
					yggr::math::moving_avg::abs_distance<yggr::s32>, // |xi - x_avg|
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::ema_accumulator_N
					<
						yggr::math::moving_avg::non_moving_collector<3 - 1, yggr::s32>,
						0, 0
					>
				>::type
			>::type
		> acc_type;

	int samples[] = { 1, 10, 100, 1000, 10000, 5000, 500 };

	int mma_means[] = { 1, 6, 37, 358, 3572, 4048, 2865 };
	bool mma_saturated[] = { false, false, true, true, true, true, true };

	int emad_means[] = { 0, 9, 52, 507, 5075, 3251, 3400 };
	bool emad_saturated[] = { false, false, true, true, true, true, true };
	
	acc_type acc;

	for(int i = 0, isize = sizeof(samples) / sizeof(int); i != isize; ++i)
	{
		acc << samples[i];
		//std::cout  << "[ "
		//			<< acc.get_result<0>() << ", " << mma_means[i] << ", "
		//			<< acc.is_saturated<0>() << ", " << mma_saturated[i]
		//			<< " ]" << std::endl;

		//std::cout  << "[ "
		//			<< acc.get_result<1>() << ", " << emad_means[i] << ", "
		//			<< acc.is_saturated<1>() << ", " << emad_saturated[i]
		//			<< " ]"<< std::endl;

		yggr_test_assert(acc.get_result<0>() == mma_means[i]);
		yggr_test_assert(acc.is_saturated<0>() == mma_saturated[i]);
		yggr_test_assert(acc.get_result<1>() == emad_means[i]);
		yggr_test_assert(acc.is_saturated<1>() == emad_saturated[i]);
	}

	std::cout << "!!!test_ema_1_n success!!!" << std::endl;
}

void test_ema_2_np1(void)
{
	typedef 
		yggr::math::moving_avg::moving_avg_accumulator
		<
			yggr::math::moving_avg::make_accumlator_group
			<
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					yggr::math::moving_avg::non_moving_collector<3, yggr::s32>,
					yggr::math::moving_avg::identical_genner<yggr::s32>,
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::mma_accumulator
					<
						yggr::math::moving_avg::non_moving_collector<3, yggr::s32>
					>
				>::type,
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					// (3 - 1)  Three samples generate two absolute mean differences
					yggr::math::moving_avg::non_moving_collector<3 - 1, yggr::s32>, 
					yggr::math::moving_avg::abs_distance<yggr::s32>, // |xi - x_avg|
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::ema_accumulator_N
					<
						yggr::math::moving_avg::non_moving_collector<3 - 1, yggr::s32>,
						1, 1
					>
				>::type
			>::type
		> acc_type;

	int samples[] = { 1, 10, 100, 1000, 10000, 5000, 500 };

	int mma_means[] = { 1, 6, 37, 358, 3572, 4048, 2865 };
	bool mma_saturated[] = { false, false, true, true, true, true, true };

	int emad_means[] = { 0, 9, 66, 664, 6649, 3168, 3421 };
	bool emad_saturated[] = { false, false, true, true, true, true, true };
	
	acc_type acc;

	for(int i = 0, isize = sizeof(samples) / sizeof(int); i != isize; ++i)
	{
		acc << samples[i];
		//std::cout  << "[ "
		//			<< acc.get_result<0>() << ", " << mma_means[i] << ", "
		//			<< acc.is_saturated<0>() << ", " << mma_saturated[i]
		//			<< " ]" << std::endl;

		//std::cout  << "[ "
		//			<< acc.get_result<1>() << ", " << emad_means[i] << ", "
		//			<< acc.is_saturated<1>() << ", " << emad_saturated[i]
		//			<< " ]"<< std::endl;

		yggr_test_assert(acc.get_result<0>() == mma_means[i]);
		yggr_test_assert(acc.is_saturated<0>() == mma_saturated[i]);
		yggr_test_assert(acc.get_result<1>() == emad_means[i]);
		yggr_test_assert(acc.is_saturated<1>() == emad_saturated[i]);
	}

	std::cout << "!!!test_ema_2_np1 success!!!" << std::endl;
}

void test_ema_alpha(void)
{
	typedef 
		yggr::math::moving_avg::moving_avg_accumulator
		<
			yggr::math::moving_avg::make_accumlator_group
			<
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					yggr::math::moving_avg::non_moving_collector<3, yggr::s32>,
					yggr::math::moving_avg::identical_genner<yggr::s32>,
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::mma_accumulator
					<
						yggr::math::moving_avg::non_moving_collector<3, yggr::s32>
					>
				>::type,
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					// (3 - 1)  Three samples generate two absolute mean differences
					yggr::math::moving_avg::non_moving_collector<3 - 1, yggr::s32>, 
					yggr::math::moving_avg::abs_distance<yggr::s32>, // |xi - x_avg|
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::ema_accumulator_alpha
					<
						yggr::math::moving_avg::non_moving_collector<3 - 1, yggr::s32>,
						1, 2
					>
				>::type
			>::type
		> acc_type;

	int samples[] = { 1, 10, 100, 1000, 10000, 5000, 500 };

	int mma_means[] = { 1, 6, 37, 358, 3572, 4048, 2865 };
	bool mma_saturated[] = { false, false, true, true, true, true, true };

	int emad_means[] = { 0, 9, 52, 507, 5075, 3251, 3400 };
	bool emad_saturated[] = { false, false, true, true, true, true, true };
	
	acc_type acc;

	for(int i = 0, isize = sizeof(samples) / sizeof(int); i != isize; ++i)
	{
		acc << samples[i];
		//std::cout  << "[ "
		//			<< acc.get_result<0>() << ", " << mma_means[i] << ", "
		//			<< acc.is_saturated<0>() << ", " << mma_saturated[i]
		//			<< " ]" << std::endl;

		//std::cout  << "[ "
		//			<< acc.get_result<1>() << ", " << emad_means[i] << ", "
		//			<< acc.is_saturated<1>() << ", " << emad_saturated[i]
		//			<< " ]"<< std::endl;

		yggr_test_assert(acc.get_result<0>() == mma_means[i]);
		yggr_test_assert(acc.is_saturated<0>() == mma_saturated[i]);
		yggr_test_assert(acc.get_result<1>() == emad_means[i]);
		yggr_test_assert(acc.is_saturated<1>() == emad_saturated[i]);
	}

	std::cout << "!!!test_ema_alpha success!!!" << std::endl;
}

void test_cma(void)
{
	typedef 
		yggr::math::moving_avg::moving_avg_accumulator
		<
			yggr::math::moving_avg::make_accumlator_group
			<
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					yggr::math::moving_avg::non_moving_collector<3, yggr::s32>,
					yggr::math::moving_avg::identical_genner<yggr::s32>,
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::mma_accumulator
					<
						yggr::math::moving_avg::non_moving_collector<3, yggr::s32>
					>
				>::type,
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					// (3 - 1)  Three samples generate two absolute mean differences
					yggr::math::moving_avg::non_moving_collector<3 - 1, yggr::s32>, 
					yggr::math::moving_avg::abs_distance<yggr::s32>, // |xi - x_avg|
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::cma_accumulator
					<
						yggr::math::moving_avg::non_moving_collector<3 - 1, yggr::s32>
					>
				>::type
			>::type
		> acc_type;

	int samples[] = { 1, 10, 100, 1000, 10000, 5000, 500 };

	int mma_means[] = { 1, 6, 37, 358, 3572, 4048, 2865 };
	bool mma_saturated[] = { false, false, true, true, true, true, true };

	int cmad_means[] = { 0, 9, 52, 507, 5075, 3251, 3400 };
	bool cmad_saturated[] = { false, false, true, true, true, true, true };
	
	acc_type acc;

	for(int i = 0, isize = sizeof(samples) / sizeof(int); i != isize; ++i)
	{
		acc << samples[i];
		//std::cout  << "[ "
		//			<< acc.get_result<0>() << ", " << mma_means[i] << ", "
		//			<< acc.is_saturated<0>() << ", " << mma_saturated[i]
		//			<< " ]" << std::endl;

		//std::cout  << "[ "
		//			<< acc.get_result<1>() << ", " << emad_means[i] << ", "
		//			<< acc.is_saturated<1>() << ", " << emad_saturated[i]
		//			<< " ]"<< std::endl;

		yggr_test_assert(acc.get_result<0>() == mma_means[i]);
		yggr_test_assert(acc.is_saturated<0>() == mma_saturated[i]);
		yggr_test_assert(acc.get_result<1>() == cmad_means[i]);
		yggr_test_assert(acc.is_saturated<1>() == cmad_saturated[i]);
	}

	std::cout << "!!!test_cma success!!!" << std::endl;
}

void test_wma(void)
{
	typedef 
		yggr::math::moving_avg::moving_avg_accumulator
		<
			yggr::math::moving_avg::make_accumlator_group
			<
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					yggr::math::moving_avg::non_moving_collector<3, yggr::s32>,
					yggr::math::moving_avg::identical_genner<yggr::s32>,
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::mma_accumulator
					<
						yggr::math::moving_avg::non_moving_collector<3, yggr::s32>
					>
				>::type,
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					// (3 - 1)  Three samples generate two absolute mean differences
					yggr::math::moving_avg::moving_collector<3 - 1, yggr::s32>, 
					yggr::math::moving_avg::abs_distance<yggr::s32>, // |xi - x_avg|
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::wma_accumulator
					<
						yggr::math::moving_avg::moving_collector<3 - 1, yggr::s32>
					>
				>::type
			>::type
		> acc_type;

	int samples[] = { 1, 10, 100, 1000, 10000, 5000, 500 };

	int mma_means[] = { 1, 6, 37, 358, 3572, 4048, 2865 };
	bool mma_saturated[] = { false, false, true, true, true, true, true };

	int wmad_means[] = { 0, 9, 66, 674, 6749, 4166, 2841 };
	bool wmad_saturated[] = { false, false, true, true, true, true, true };
	
	acc_type acc;

	for(int i = 0, isize = sizeof(samples) / sizeof(int); i != isize; ++i)
	{
		acc << samples[i];
		//std::cout  << "[ "
		//			<< acc.get_result<0>() << ", " << mma_means[i] << ", "
		//			<< acc.is_saturated<0>() << ", " << mma_saturated[i]
		//			<< " ]" << std::endl;

		//std::cout  << "[ "
		//			<< acc.get_result<1>() << ", " << wmad_means[i] << ", "
		//			<< acc.is_saturated<1>() << ", " << wmad_saturated[i]
		//			<< " ]"<< std::endl;

		yggr_test_assert(acc.get_result<0>() == mma_means[i]);
		yggr_test_assert(acc.is_saturated<0>() == mma_saturated[i]);
		yggr_test_assert(acc.get_result<1>() == wmad_means[i]);
		yggr_test_assert(acc.is_saturated<1>() == wmad_saturated[i]);
	}

	std::cout << "!!!test_wma success!!!" << std::endl;
}

void test_standard_deviation_of_mma(void)
{
	typedef 
		yggr::math::moving_avg::moving_avg_accumulator
		<
			yggr::math::moving_avg::make_accumlator_group
			<
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					yggr::math::moving_avg::non_moving_collector<3, yggr::s32>,
					yggr::math::moving_avg::identical_genner<yggr::s32>,
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::mma_accumulator
					<
						yggr::math::moving_avg::non_moving_collector<3, yggr::s32>
					>
				>::type,
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s64,
					// (3 - 1)  Three samples generate two absolute mean differences
					yggr::math::moving_avg::non_moving_collector<3 - 1, yggr::s64>, 
					yggr::math::moving_avg::pow2_distance<yggr::s64>, // |xi - x_avg|
					yggr::math::moving_avg::sqrt_getter<yggr::s64>,
					yggr::math::moving_avg::mma_accumulator
					<
						yggr::math::moving_avg::non_moving_collector<3 - 1, yggr::s64>
					>
				>::type
			>::type
		> acc_type;

	int samples[] = { 1, 10, 100, 1000, 10000, 5000, 500 };

	int mma_means[] = { 1, 6, 37, 358, 3572, 4048, 2865 };
	bool mma_saturated[] = { false, false, true, true, true, true, true };

	int sd_emad_means[] = { 0, 9, 67, 683, 6835, 4937, 4299 };
	bool sd_emad_saturated[] = { false, false, true, true, true, true, true };
	
	acc_type acc;

	for(int i = 0, isize = sizeof(samples) / sizeof(int); i != isize; ++i)
	{
		acc << samples[i];
		//std::cout  << "[ "
		//			<< acc.get_result<0>() << ", " << mma_means[i] << ", "
		//			<< acc.is_saturated<0>() << ", " << mma_saturated[i]
		//			<< " ]" << std::endl;

		//std::cout  << "[ "
		//			<< acc.get_result<1>() << ", " << sd_emad_means[i] << ", "
		//			<< acc.is_saturated<1>() << ", " << sd_emad_saturated[i]
		//			<< " ]"<< std::endl;

		yggr_test_assert(acc.get_result<0>() == mma_means[i]);
		yggr_test_assert(acc.is_saturated<0>() == mma_saturated[i]);
		yggr_test_assert(acc.get_result<1>() == sd_emad_means[i]);
		yggr_test_assert(acc.is_saturated<1>() == sd_emad_saturated[i]);
	}

	std::cout << "!!!test_standard_deviation_of_mma success!!!" << std::endl;
}

void test_mix(void)
{
	typedef 
		yggr::math::moving_avg::moving_avg_accumulator
		<
			yggr::math::moving_avg::make_accumlator_group
			<
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					yggr::math::moving_avg::non_moving_collector<3, yggr::s32>,
					yggr::math::moving_avg::identical_genner<yggr::s32>,
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::mma_accumulator
					<
						yggr::math::moving_avg::non_moving_collector<3, yggr::s32>
					>
				>::type,
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					// (3 - 1)  Three samples generate two absolute mean differences
					yggr::math::moving_avg::non_moving_collector<3 - 1, yggr::s32>, 
					yggr::math::moving_avg::abs_distance<yggr::s32>, // |xi - x_avg|
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::mma_accumulator
					<
						yggr::math::moving_avg::non_moving_collector<3 - 1, yggr::s32>
					>
				>::type,
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					// (3 - 1)  Three samples generate two absolute mean differences
					yggr::math::moving_avg::moving_collector<3 - 1, yggr::s32>, 
					yggr::math::moving_avg::abs_distance<yggr::s32>, // |xi - x_avg|
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::wma_accumulator
					<
						yggr::math::moving_avg::moving_collector<3 - 1, yggr::s32>
					>
				>::type,
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					// (3 - 1)  Three samples generate two absolute mean differences
					yggr::math::moving_avg::non_moving_collector<3 - 1, yggr::s32>, 
					yggr::math::moving_avg::abs_distance<yggr::s32>, // |xi - x_avg|
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::cma_accumulator
					<
						yggr::math::moving_avg::non_moving_collector<3 - 1, yggr::s32>
					>
				>::type
			>::type
		> acc_type;

	int samples[] = { 1, 10, 100, 1000, 10000, 5000, 500 };

	int mma_means[] = { 1, 6, 37, 358, 3572, 4048, 2865 };
	bool mma_saturated[] = { false, false, true, true, true, true, true };

	int mmad_means[] = { 0, 9, 52, 507, 5075, 3251, 3400 };
	bool mmad_saturated[] = { false, false, true, true, true, true, true };

	int wmad_means[] = { 0, 9, 66, 674, 6749, 4166, 2841 };
	bool wmad_saturated[] = { false, false, true, true, true, true, true };

	int cmad_means[] = { 0, 9, 52, 507, 5075, 3251, 3400 };
	bool cmad_saturated[] = { false, false, true, true, true, true, true };
	
	acc_type acc;

	for(int i = 0, isize = sizeof(samples) / sizeof(int); i != isize; ++i)
	{
		acc << samples[i];

		yggr_test_assert(acc.get_result<0>() == mma_means[i]);
		yggr_test_assert(acc.is_saturated<0>() == mma_saturated[i]);

		yggr_test_assert(acc.get_result<1>() == mmad_means[i]);
		yggr_test_assert(acc.is_saturated<1>() == mmad_saturated[i]);
		
		yggr_test_assert(acc.get_result<2>() == wmad_means[i]);
		yggr_test_assert(acc.is_saturated<2>() == wmad_saturated[i]);

		yggr_test_assert(acc.get_result<3>() == cmad_means[i]);
		yggr_test_assert(acc.is_saturated<3>() == cmad_saturated[i]);
	}

	std::cout << "!!!test_mix success!!!" << std::endl;
}

void other_test(void)
{
	typedef 
		yggr::math::moving_avg::moving_avg_accumulator
		<
			yggr::math::moving_avg::make_accumlator_group
			<
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					yggr::math::moving_avg::non_moving_collector<3, yggr::s32>,
					yggr::math::moving_avg::identical_genner<yggr::s32>,
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::mma_accumulator
					<
						yggr::math::moving_avg::non_moving_collector<3, yggr::s32>
					>
				>::type,
				yggr::math::moving_avg::make_accumulator
				<
					yggr::s32,
					// (3 - 1)  Three samples generate two absolute mean differences
					yggr::math::moving_avg::non_moving_collector<3 - 1, yggr::s32>, 
					yggr::math::moving_avg::abs_distance<yggr::s32>, // |xi - x_avg|
					yggr::math::moving_avg::identical_getter<yggr::s32>,
					yggr::math::moving_avg::mma_accumulator
					<
						yggr::math::moving_avg::non_moving_collector<3 - 1, yggr::s32>
					>
				>::type
			>::type
		> acc_type;

	int samples[] = { 1, 10, 100, 1000, 10000, 5000, 500 };

	int mma_means[] = { 1, 6, 37, 358, 3572, 4048, 2865 };
	bool mma_saturated[] = { false, false, true, true, true, true, true };

	int emad_means[] = { 0, 9, 52, 507, 5075, 3251, 3400 };
	bool emad_saturated[] = { false, false, true, true, true, true, true };
	
	acc_type acc;

	for(int i = 0, isize = sizeof(samples) / sizeof(int); i != isize; ++i)
	{
		acc << samples[i];

		yggr_test_assert(acc.get_result_pair<0>().first == mma_means[i]);
		yggr_test_assert((acc.get<0, 0>() + 32) / 64 == mma_means[i]);
		yggr_test_assert(acc.get_result_pair<0>().second == mma_saturated[i]);
		
		yggr_test_assert(acc.get_result_pair<1>().first == emad_means[i]);
		yggr_test_assert((acc.get<1, 0>() + 32) / 64 == emad_means[i]);
		yggr_test_assert(acc.get_result_pair<1>().second == emad_saturated[i]);
	}

	std::cout << "!!!other_test success!!!" << std::endl;
}

int main(int argc, char* argv[])
{
	test_sma();

	test_mma();
	
	test_ema_1_n();
	test_ema_2_np1();
	test_ema_alpha();

	test_cma();
	test_wma();

	test_standard_deviation_of_mma();

	test_mix();
	other_test();

	wait_any_key(argc, argv);
	return 0;
}
