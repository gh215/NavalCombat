#include "pch.h"
#include "CppUnitTest.h"
#include "..\NavalCombat2\naval.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	public:		
		TEST_METHOD(TestShip_hascell)
		{
			Ship ship({ {'A', 1}, {'B', 1}, {'C', 1} });
			Assert::IsTrue(ship.has_cell({ 'A', 1 }));
			Assert::IsFalse(ship.has_cell({ 'D', 1 }));
		}
		TEST_METHOD(TestShip_cellstate)
		{
			Ship ship({ {'A', 1}, {'B', 1}, {'C', 1} });
			Assert::AreEqual(ship.cell_state({ 'A', 1 }), PIPE);

			ship.hit({ 'A', 1 });
			Assert::AreEqual(ship.cell_state({ 'A', 1 }), WOUNDED);
		}
		TEST_METHOD(TestShip_hit)
		{
			Ship ship({ {'A', 1} });
			ship.hit({ 'A', 1 });

			Assert::AreEqual(ship.cell_state({ 'A', 1 }), KILLED);
		}
		TEST_METHOD(TestShip_killedcheck)
		{

			Ship ship({ {'A', 1}, {'B', 1} });
			ship.hit({ 'A', 1 });
			ship.hit({ 'B', 1 });

			Assert::IsTrue(ship.killed);
		}

	};
}
