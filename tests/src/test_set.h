
	TEST_ITEM(int8_t, std::numeric_limits<int8_t>::max() );
	TEST_ITEM(int8_t, int8_t(0) );
	TEST_ITEM(int8_t, std::numeric_limits<int8_t>::min() );
	TEST_ITEM(int16_t, std::numeric_limits<int16_t>::max() );
	TEST_ITEM(int16_t, int16_t(0) );
	
	TEST_ITEM(double, 2.0 );

	TEST_ITEM(int16_t, std::numeric_limits<int16_t>::min() );
	TEST_ITEM(int32_t, std::numeric_limits<int32_t>::max() );
	TEST_ITEM(int32_t, int32_t(0) );
	
	TEST_ITEM(int16_t, int16_t(1234) );

	TEST_ITEM(int32_t, std::numeric_limits<int32_t>::min() );
	TEST_ITEM(int64_t, std::numeric_limits<int64_t>::max() );
	TEST_ITEM(int64_t, int64_t(0) );
	TEST_ITEM(int64_t, std::numeric_limits<int64_t>::min() );

	TEST_ITEM(uint8_t, uint8_t(123) );

	TEST_ITEM(uint8_t, std::numeric_limits<uint8_t>::max() );
	TEST_ITEM(uint8_t, uint8_t(0) );
	TEST_ITEM(uint8_t, std::numeric_limits<uint8_t>::min() );
	TEST_ITEM(uint16_t, std::numeric_limits<uint16_t>::max() );
	TEST_ITEM(uint16_t, uint16_t(0) );

	TEST_ITEM(int16_t, (int16_t(-1234)) );

	TEST_ITEM(uint16_t, std::numeric_limits<uint16_t>::min() );
	TEST_ITEM(uint32_t, std::numeric_limits<uint32_t>::max() );
	TEST_ITEM(uint32_t, uint32_t(0) );
	TEST_ITEM(uint32_t, std::numeric_limits<uint32_t>::min() );
	TEST_ITEM(uint64_t, std::numeric_limits<uint64_t>::max() );
	TEST_ITEM(uint64_t, uint64_t(0) );
	TEST_ITEM(uint64_t, std::numeric_limits<uint64_t>::min() );

	TEST_ITEM(uint16_t, uint16_t(1234) );

	TEST_ITEM(float, 0.0f );
	TEST_ITEM(float, 1.0f );
	TEST_ITEM(float, (-1.0f) );
	TEST_ITEM(float, std::numeric_limits<float>::infinity() );
	TEST_ITEM(float, (-std::numeric_limits<float>::infinity()) );

	TEST_ITEM(double, 0.0 );
	TEST_ITEM(double, 1.0 );
	TEST_ITEM(double, (-1.0) );
	TEST_ITEM(double, std::numeric_limits<double>::infinity() );
	TEST_ITEM(double, (-std::numeric_limits<double>::infinity()) );
	